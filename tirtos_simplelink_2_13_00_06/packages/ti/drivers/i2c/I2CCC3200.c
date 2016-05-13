/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif
#include <ti/drivers/ports/DebugP.h>
#include <ti/drivers/ports/ClockP.h>
#include <ti/drivers/ports/HwiP.h>
#include <ti/drivers/ports/SemaphoreP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

#include <ti/drivers/i2c/I2CCC3200.h>

#include <inc/hw_types.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/i2c.h>

/*
 * Specific I2C CMD MACROs that are not defined in I2C.h are defined here. Their
 * equivalent values are taken from the existing MACROs in I2C.h
 */
#ifndef I2C_MASTER_CMD_BURST_RECEIVE_START_NACK
#define I2C_MASTER_CMD_BURST_RECEIVE_START_NACK  I2C_MASTER_CMD_BURST_SEND_START
#endif

#ifndef I2C_MASTER_CMD_BURST_RECEIVE_STOP
#define I2C_MASTER_CMD_BURST_RECEIVE_STOP        I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP
#endif

#ifndef I2C_MASTER_CMD_BURST_RECEIVE_CONT_NACK
#define I2C_MASTER_CMD_BURST_RECEIVE_CONT_NACK   I2C_MASTER_CMD_BURST_SEND_CONT
#endif

/* Prototypes */
static void I2CCC3200_blockingCallback(I2C_Handle handle, I2C_Transaction *msg,
                                       bool transferStatus);
void        I2CCC3200_close(I2C_Handle handle);
int         I2CCC3200_control(I2C_Handle handle, unsigned int cmd, void *arg);
void        I2CCC3200_init(I2C_Handle handle);
I2C_Handle  I2CCC3200_open(I2C_Handle handle, I2C_Params *params);
bool        I2CCC3200_transfer(I2C_Handle handle, I2C_Transaction *transaction);
static void initHw(I2C_Handle handle);
static int postNotify(unsigned int eventType, uintptr_t eventArg,
                      uintptr_t clientArg);
static void I2CCC3200_primeTransfer(I2CCC3200_Object        *object,
                                    I2CCC3200_HWAttrs const *hwAttrs,
                                    I2C_Transaction         *transaction);

/* I2C function table for I2CCC3200 implementation */
const I2C_FxnTable I2CCC3200_fxnTable = {
    I2CCC3200_close,
    I2CCC3200_control,
    I2CCC3200_init,
    I2CCC3200_open,
    I2CCC3200_transfer
};

static const uint32_t bitRate[] = {
    false,  /*  I2C_100kHz = 0 */
    true    /*  I2C_400kHz = 1 */
};

/* Default I2C params */
extern const I2C_Params I2C_defaultParams;

/* Guard to avoid power constraints getting out of sync */
static volatile bool powerConstraint;

/*
 *  ======== I2CCC3200_close ========
 */
void I2CCC3200_close(I2C_Handle handle)
{
    uintptr_t                 key;
    I2CCC3200_Object         *object = handle->object;
    I2CCC3200_HWAttrs const  *hwAttrs = handle->hwAttrs;

    /* Check to see if a I2C transaction is in progress */
    DebugP_assert(object->headPtr == NULL);

    /* Mask I2C interrupts */
    MAP_I2CMasterIntDisable(hwAttrs->baseAddr);

    /* Disable the I2C Master */
    MAP_I2CMasterDisable(hwAttrs->baseAddr);

    /* Power off the I2C module */
    Power_releaseDependency(PowerCC3200_PERIPH_I2CA0);

    Power_unregisterNotify(&(object->notifyObj));

    if (object->hwiHandle) {
        HwiP_delete(object->hwiHandle);
    }
    if (object->mutex) {
        SemaphoreP_delete(object->mutex);
    }
    /* Destruct the Semaphore */
    if (object->transferComplete) {
        SemaphoreP_delete(object->transferComplete);
    }

    /* Mark the module as available */
    key = HwiP_disable();

    object->isOpen = false;
    powerConstraint = false;

    HwiP_restore(key);

    DebugP_log1("I2C: Object closed 0x%x", hwAttrs->baseAddr);

    return;
}

/*
 *  ======== I2CCC3200_completeTransfer ========
 */
static void I2CCC3200_completeTransfer(I2C_Handle handle)
{
    /* Get the pointer to the object */
    I2CCC3200_Object *object = handle->object;

    DebugP_log1("I2C:(%p) ISR Transfer Complete",
                ((I2CCC3200_HWAttrs const *)(handle->hwAttrs))->baseAddr);

    /*
     * Perform callback in a HWI context, thus any tasks or SWIs
     * made ready to run won't start until the interrupt has
     * finished
     */
    object->transferCallbackFxn(handle, object->currentTransaction,
                                !((bool)object->mode));

    /* See if we need to process any other transactions */
    if (object->headPtr == object->tailPtr) {
        /* No other transactions need to occur */
        object->currentTransaction = NULL;
        object->headPtr = NULL;

        if (powerConstraint) {
            /* release constraint since transaction is done */
            Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);
            powerConstraint = false;
        }

        DebugP_log1("I2C:(%p) ISR No other I2C transaction in queue",
                    ((I2CCC3200_HWAttrs const *)(handle->hwAttrs))->baseAddr);
    }
    else {
        /* Another transfer needs to take place */
        object->headPtr = object->headPtr->nextPtr;

        DebugP_log2("I2C:(%p) ISR Priming next I2C transaction (%p) from queue",
                    ((I2CCC3200_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                    (uintptr_t)object->headPtr);

        I2CCC3200_primeTransfer(object, (I2CCC3200_HWAttrs const *)handle->hwAttrs,
                                object->headPtr);
    }
}

/*
 *  ======== I2CCC3200_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int I2CCC3200_control(I2C_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (I2C_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== I2CCC3200_hwiFxn ========
 *  Hwi interrupt handler to service the I2C peripheral
 *
 *  The handler is a generic handler for a I2C object.
 */
static void I2CCC3200_hwiFxn(uintptr_t arg)
{
    /* Get the pointer to the object and hwAttrs */
    I2CCC3200_Object         *object = ((I2C_Handle)arg)->object;
    I2CCC3200_HWAttrs const  *hwAttrs = ((I2C_Handle)arg)->hwAttrs;
    uint32_t                  errStatus;

    /* Get the interrupt status of the I2C controller */
    errStatus = MAP_I2CMasterErr(hwAttrs->baseAddr);

    /* Clear interrupt source to avoid additional interrupts */
    MAP_I2CMasterIntClear(hwAttrs->baseAddr);

    /* Check for I2C Errors */
    if ((errStatus == I2C_MASTER_ERR_NONE) || (object->mode == I2CCC3200_ERROR)) {
        /* No errors, now check what we need to do next */
        switch (object->mode) {
            /*
             * ERROR case is OK because if an Error is detected, a STOP bit is
             * sent; which in turn will call another interrupt. This interrupt
             * call will then post the transferComplete semaphore to unblock the
             * I2C_transfer function
             */
            case I2CCC3200_ERROR:
            case I2CCC3200_IDLE_MODE:
                I2CCC3200_completeTransfer((I2C_Handle) arg);
                break;

            case I2CCC3200_WRITE_MODE:
                /* Decrement write Counter */
                object->writeCountIdx--;

                /* Check if more data needs to be sent */
                if (object->writeCountIdx) {
                    DebugP_log3("I2C:(%p) ISR I2CCC3200_WRITE_MODE: Data to write: "
                                "0x%x; To slave: 0x%x",
                                hwAttrs->baseAddr,
                                *(object->writeBufIdx),
                                object->currentTransaction->slaveAddress);

                    /* Write data contents into data register */
                    MAP_I2CMasterDataPut(hwAttrs->baseAddr, *(object->writeBufIdx));
                    object->writeBufIdx++;

                    if ((object->writeCountIdx < 2) && !(object->readCountIdx)) {
                        /* Everything has been sent, nothing to receive */
                        /* Next state: Idle mode */
                        object->mode = I2CCC3200_IDLE_MODE;

                        /* Send last byte with STOP bit */
                        MAP_I2CMasterControl(hwAttrs->baseAddr,
                                             I2C_MASTER_CMD_BURST_SEND_FINISH);

                        DebugP_log1("I2C:(%p) ISR I2CCC3200_WRITE_MODE: ACK received; "
                                    "Writing w/ STOP bit",
                                     hwAttrs->baseAddr);
                    }
                    else {
                        /*
                         * Either there is more date to be transmitted or some
                         * data needs to be received next
                         */
                        MAP_I2CMasterControl(hwAttrs->baseAddr,
                                             I2C_MASTER_CMD_BURST_SEND_CONT);

                        DebugP_log1("I2C:(%p) ISR I2CCC3200_WRITE_MODE: ACK received; "
                                    "Writing", hwAttrs->baseAddr);
                    }
                }

                /* At this point, we know that we need to receive data */
                else {
                    /*
                     * We need to check after we are done transmitting data, if
                     * we need to receive any data.
                     * In a corner case when we have only one byte transmitted
                     * and no data to receive, the I2C will automatically send
                     * the STOP bit. In other words, here we only need to check
                     * if data needs to be received. If so, how much.
                     */
                    if (object->readCountIdx) {
                        /* Next state: Receive mode */
                        object->mode = I2CCC3200_READ_MODE;

                        /* Switch into Receive mode */
                        MAP_I2CMasterSlaveAddrSet(hwAttrs->baseAddr,
                                                  object->currentTransaction->slaveAddress,
                                                  true);

                        if (object->readCountIdx > 1) {
                            /* Send a repeated START */
                            MAP_I2CMasterControl(hwAttrs->baseAddr,
                                                 I2C_MASTER_CMD_BURST_RECEIVE_START);

                            DebugP_log1("I2C:(%p) ISR I2CCC3200_WRITE_MODE: -> "
                                        "I2CCC3200_READ_MODE; Reading w/ RESTART and ACK",
                                        hwAttrs->baseAddr);
                        }
                        else {
                            /*
                             * Send a repeated START with a NACK since it's the
                             * last byte to be received.
                             * I2C_MASTER_CMD_BURST_RECEIVE_START_NACK is
                             * is locally defined because there is no macro to
                             * receive data and send a NACK after sending a
                             * start bit (0x00000003)
                             */
                            MAP_I2CMasterControl(hwAttrs->baseAddr,
                                                 I2C_MASTER_CMD_BURST_RECEIVE_START_NACK);

                            DebugP_log1("I2C:(%p) ISR I2CCC3200_WRITE_MODE: -> "
                                        "I2CCC3200_READ_MODE; Reading w/ RESTART and NACK",
                                        hwAttrs->baseAddr);
                        }
                    }
                    else {
                        /* Done with all transmissions */
                        object->mode = I2CCC3200_IDLE_MODE;
                        /*
                         * No more data needs to be received, so follow up with
                         * a STOP bit
                         * Again, there is no equivalent macro (0x00000004) so
                         * I2C_MASTER_CMD_BURST_RECEIVE_STOP is used.
                         */
                        MAP_I2CMasterControl(hwAttrs->baseAddr,
                                             I2C_MASTER_CMD_BURST_RECEIVE_STOP);

                        DebugP_log1("I2C:(%p) ISR I2CCC3200_WRITE_MODE: -> "
                                    "I2CCC3200_IDLE_MODE; Sending STOP bit",
                                    hwAttrs->baseAddr);
                    }
                }
                break;

            case I2CCC3200_READ_MODE:
                /* Save the received data */
                *(object->readBufIdx) = MAP_I2CMasterDataGet(hwAttrs->baseAddr);

                DebugP_log2("I2C:(%p) ISR I2CCC3200_READ_MODE: Read data byte: 0x%x",
                            hwAttrs->baseAddr, *(object->readBufIdx));

                object->readBufIdx++;

                /* Check if any data needs to be received */
                object->readCountIdx--;
                if (object->readCountIdx) {
                    if (object->readCountIdx > 1) {
                        /* More data to be received */
                        MAP_I2CMasterControl(hwAttrs->baseAddr,
                                             I2C_MASTER_CMD_BURST_RECEIVE_CONT);

                        DebugP_log1("I2C:(%p) ISR I2CCC3200_READ_MODE: Reading w/ ACK",
                                    hwAttrs->baseAddr);
                    }
                    else {
                        /*
                         * Send NACK because it's the last byte to be received
                         * There is no NACK macro equivalent (0x00000001) so
                         * I2C_MASTER_CMD_BURST_RECEIVE_CONT_NACK is used
                         */
                        MAP_I2CMasterControl(hwAttrs->baseAddr,
                                             I2C_MASTER_CMD_BURST_RECEIVE_CONT_NACK);

                        DebugP_log1("I2C:(%p) ISR I2CCC3200_READ_MODE: Reading w/ NACK",
                                    hwAttrs->baseAddr);
                    }
                }
                else {
                    /* Next state: Idle mode */
                    object->mode = I2CCC3200_IDLE_MODE;

                    /*
                     * No more data needs to be received, so follow up with a
                     * STOP bit
                     * Again, there is no equivalent macro (0x00000004) so
                     * I2C_MASTER_CMD_BURST_RECEIVE_STOP is used
                     */
                    MAP_I2CMasterControl(hwAttrs->baseAddr,
                                         I2C_MASTER_CMD_BURST_RECEIVE_STOP);

                    DebugP_log1("I2C:(%p) ISR I2CCC3200_READ_MODE: -> I2CCC3200_IDLE_MODE; "
                                "Sending STOP bit", hwAttrs->baseAddr);
                }

                break;

            default:
                object->mode = I2CCC3200_ERROR;
                break;
        }

    }
    else {
        /* Some sort of error happened! */
        object->mode = I2CCC3200_ERROR;

        if (errStatus & (I2C_MASTER_ERR_ARB_LOST | I2C_MASTER_ERR_ADDR_ACK)) {
            I2CCC3200_completeTransfer((I2C_Handle) arg);
        }
        else {
        /* Try to send a STOP bit to end all I2C communications immediately */
        /*
         * I2C_MASTER_CMD_BURST_SEND_ERROR_STOP -and-
         * I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP
         * have the same values
         */
            MAP_I2CMasterControl(hwAttrs->baseAddr,
                                 I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            I2CCC3200_completeTransfer((I2C_Handle) arg);
        }

        DebugP_log2("I2C:(%p) ISR I2C Bus fault (Status Reg: 0x%x)",
                    hwAttrs->baseAddr, errStatus);
    }

    return;
}

/*
 *  ======== I2CCC3200_init ========
 */
void I2CCC3200_init(I2C_Handle handle)
{
    /* Mark the object as available */
    ((I2CCC3200_Object *)(handle->object))->isOpen = false;
}

/*
 *  ======== I2CCC3200_open ========
 */
I2C_Handle I2CCC3200_open(I2C_Handle handle, I2C_Params *params)
{
    uintptr_t                 key;
    I2CCC3200_Object         *object = handle->object;
    I2CCC3200_HWAttrs const  *hwAttrs = handle->hwAttrs;
    SemaphoreP_Params         semParams;
    HwiP_Params               hwiParams;

    /* Determine if the device index was already opened */
    key = HwiP_disable();
    if(object->isOpen == true){
        HwiP_restore(key);
        return (NULL);
    }

    /* Mark the handle as being used */
    object->isOpen = true;
    HwiP_restore(key);

    /* No power constraints on startup */
    powerConstraint = false;

    /* Store the I2C parameters */
    if (params == NULL) {
        /* No params passed in, so use the defaults */
        params = (I2C_Params *) &I2C_defaultParams;
    }

    /* Save parameters */
    object->transferMode = params->transferMode;
    object->transferCallbackFxn = params->transferCallbackFxn;
    object->bitRate = params->bitRate;

    /* Power on the I2C module */
    Power_setDependency(PowerCC3200_PERIPH_I2CA0);

    Power_registerNotify(&(object->notifyObj), PowerCC3200_AWAKE_LPDS,
            postNotify, (uint32_t)handle);

    HwiP_Params_init(&hwiParams);
    hwiParams.arg = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum,
                                    I2CCC3200_hwiFxn,
                                    &hwiParams);
    if (object->hwiHandle == NULL) {
        I2CCC3200_close(handle);
        return (NULL);
    }

    /*
     * Create threadsafe handles for this I2C peripheral
     * Semaphore to provide exclusive access to the I2C peripheral
     */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    object->mutex = SemaphoreP_create(1, &semParams);
    if (object->mutex == NULL) {
        I2CCC3200_close(handle);
        return (NULL);
    }

    /*
     * Store a callback function that posts the transfer complete
     * semaphore for synchronous mode
     */
    if (object->transferMode == I2C_MODE_BLOCKING) {
        /*
         * Semaphore to cause the waiting task to block for the I2C
         * to finish
         */
        object->transferComplete = SemaphoreP_create(0, &semParams);
        if (object->transferComplete == NULL) {
            I2CCC3200_close(handle);
            return (NULL);
        }

        /* Store internal callback function */
        object->transferCallbackFxn = I2CCC3200_blockingCallback;
    }
    else {
        /* Check to see if a callback function was defined for async mode */
        DebugP_assert(object->transferCallbackFxn != NULL);
    }

    /* Specify the idle state for this I2C peripheral */
    object->mode = I2CCC3200_IDLE_MODE;

    /* Clear the head pointer */
    object->headPtr = NULL;
    object->tailPtr = NULL;

    DebugP_log1("I2C: Object created 0x%x", hwAttrs->baseAddr);

    /* Set the I2C configuration */
    initHw(handle);

    /* Return the address of the handle */
    return (handle);
}

/*
 *  ======== I2CCC3200_primeTransfer =======
 */
static void I2CCC3200_primeTransfer(I2CCC3200_Object  *object,
                                    I2CCC3200_HWAttrs const *hwAttrs,
                                    I2C_Transaction *transaction)
{
    /* Store the new internal counters and pointers */
    object->currentTransaction = transaction;

    object->writeBufIdx = transaction->writeBuf;
    object->writeCountIdx = transaction->writeCount;

    object->readBufIdx = transaction->readBuf;
    object->readCountIdx = transaction->readCount;

    DebugP_log2("I2C:(%p) Starting transaction to slave: 0x%x",
                hwAttrs->baseAddr,
                object->currentTransaction->slaveAddress);

    /* Start transfer in Transmit mode */
    if (object->writeCountIdx) {
        /* Specify the I2C slave address */
        MAP_I2CMasterSlaveAddrSet(hwAttrs->baseAddr,
                                  object->currentTransaction->slaveAddress,
                                  false);

        /* Update the I2C mode */
        object->mode = I2CCC3200_WRITE_MODE;

        DebugP_log3("I2C:(%p) I2CCC3200_IDLE_MODE: Data to write: 0x%x; To Slave: 0x%x",
                    hwAttrs->baseAddr, *(object->writeBufIdx),
                    object->currentTransaction->slaveAddress);

        /* Write data contents into data register */
        MAP_I2CMasterDataPut(hwAttrs->baseAddr, *((object->writeBufIdx)++));

        /* Start the I2C transfer in master transmit mode */
        MAP_I2CMasterControl(hwAttrs->baseAddr, I2C_MASTER_CMD_BURST_SEND_START);

        DebugP_log1("I2C:(%p) I2CCC3200_IDLE_MODE: -> I2CCC3200_WRITE_MODE; Writing w/ START",
                    hwAttrs->baseAddr);
    }

    /* Start transfer in Receive mode */
    else {
        /* Specify the I2C slave address */
        MAP_I2CMasterSlaveAddrSet(hwAttrs->baseAddr,
                                  object->currentTransaction->slaveAddress,
                                  true);

        /* Update the I2C mode */
        object->mode = I2CCC3200_READ_MODE;

        if (object->readCountIdx < 2) {
            /* Start the I2C transfer in master receive mode */
            MAP_I2CMasterControl(hwAttrs->baseAddr,
                                 I2C_MASTER_CMD_BURST_RECEIVE_START_NACK);

            DebugP_log1("I2C:(%p) I2CCC3200_IDLE_MODE: -> I2CCC3200_READ_MODE; Reading w/ "
                        "NACK", hwAttrs->baseAddr);
        }
        else {
            /* Start the I2C transfer in master receive mode */
            MAP_I2CMasterControl(hwAttrs->baseAddr,
                                 I2C_MASTER_CMD_BURST_RECEIVE_START);

            DebugP_log1("I2C:(%p) I2CCC3200_IDLE_MODE: -> I2CCC3200_READ_MODE; Reading w/ ACK",
                        hwAttrs->baseAddr);
        }
    }
}

/*
 *  ======== I2CCC3200_transfer ========
 */
bool I2CCC3200_transfer(I2C_Handle handle, I2C_Transaction *transaction)
{
    uintptr_t                 key;
    bool                      ret = false;
    I2CCC3200_Object         *object = handle->object;
    I2CCC3200_HWAttrs const  *hwAttrs = handle->hwAttrs;

    /* Check if anything needs to be written or read */
    if ((!transaction->writeCount) && (!transaction->readCount)) {
        /* Nothing to write or read */
        return (ret);
    }

    if (object->transferMode == I2C_MODE_CALLBACK) {
        /* Check if a transfer is in progress */
        key = HwiP_disable();
        if (object->headPtr) {
            /* Transfer in progress */

            /*
             * Update the message pointed by the tailPtr to point to the next
             * message in the queue
             */
            object->tailPtr->nextPtr = transaction;

            /* Update the tailPtr to point to the last message */
            object->tailPtr = transaction;

            /* I2C is still being used */
            HwiP_restore(key);
            return (true);
        }
        else {
            /* Store the headPtr indicating I2C is in use */
            object->headPtr = transaction;
            object->tailPtr = transaction;
        }
        HwiP_restore(key);
    }

    // TODO: This block of code was copied from I2C26XX.c.
    // It seems like powerConstraint should be checked inside
    // the mutex.
    if (!powerConstraint) {
        /* set constraints to guarantee transaction */
        Power_setConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);
        powerConstraint = true;
    }

    /* Acquire the lock for this particular I2C handle */
    SemaphoreP_pend(object->mutex, SemaphoreP_WAIT_FOREVER);

    /*
     * I2CCC3200_primeTransfer is a longer process and
     * protection is needed from the I2C interrupt
     */
    HwiP_disableInterrupt(hwAttrs->intNum);
    I2CCC3200_primeTransfer(object, hwAttrs, transaction);
    HwiP_enableInterrupt(hwAttrs->intNum);

    if (object->transferMode == I2C_MODE_BLOCKING) {
        DebugP_log1("I2C:(%p) Pending on transferComplete semaphore",
                    hwAttrs->baseAddr);
        /*
         * Wait for the transfer to complete here.
         * It's OK to block from here because the I2C's Hwi will unblock
         * upon errors
         */
        SemaphoreP_pend(object->transferComplete, SemaphoreP_WAIT_FOREVER);

        if (powerConstraint) {
            /* release constraint since transaction is done */
            Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);
            powerConstraint = false;
        }

        DebugP_log1("I2C:(%p) Transaction completed",
                    hwAttrs->baseAddr);

        /* Hwi handle has posted a 'transferComplete' check for Errors */
        if (object->mode == I2CCC3200_IDLE_MODE) {
            DebugP_log1("I2C:(%p) Transfer OK", hwAttrs->baseAddr);
            ret = true;
        }
    }
    else {
        /* Always return true if in Asynchronous mode */
        ret = true;
    }

    /* Release the lock for this particular I2C handle */
    SemaphoreP_post(object->mutex);

    /* Return the number of bytes transfered by the I2C */
    return (ret);
}

/*
 *  ======== I2CCC3200_blockingCallback ========
 */
static void I2CCC3200_blockingCallback(I2C_Handle handle,
                                     I2C_Transaction *msg,
                                     bool transferStatus)
{
    I2CCC3200_Object  *object = handle->object;

    DebugP_log1("I2C:(%p) posting transferComplete semaphore",
                ((I2CCC3200_HWAttrs const  *)(handle->hwAttrs))->baseAddr);

    /* Indicate transfer complete */
    SemaphoreP_post(object->transferComplete);
}

/*
 *  ======== initHw ========
 */
static void initHw(I2C_Handle handle)
{
    ClockP_FreqHz            freq;
    I2CCC3200_Object        *object = handle->object;
    I2CCC3200_HWAttrs const *hwAttrs = handle->hwAttrs;

    ClockP_getCpuFreq(&freq);
    MAP_I2CMasterInitExpClk(hwAttrs->baseAddr, freq.lo,
                            bitRate[object->bitRate]);

    /* Clear any pending interrupts */
    MAP_I2CMasterIntClear(hwAttrs->baseAddr);

    /* Enable the I2C Master for operation */
    MAP_I2CMasterEnable(hwAttrs->baseAddr);

    /* Unmask I2C interrupts */
    MAP_I2CMasterIntEnable(hwAttrs->baseAddr);
}

/*
 *  ======== postNotify ========
 *  This functions is called to notify the SPI driver of an ongoing transition
 *  out of LPDS mode.
 *  clientArg should be pointing to a hardware module which has already
 *  been opened.
 */
static int postNotify(unsigned int eventType,
        uintptr_t eventArg, uintptr_t clientArg)
{
    uint32_t ulRegVal;

    /* Take I2C hardware semaphore */
    ulRegVal = HWREG(0x400F7000);
    ulRegVal = (ulRegVal & ~0x3) | 0x1;
    HWREG(0x400F7000) = ulRegVal;

    /* Reconfigure the hardware when returning from LPDS */
    initHw((I2C_Handle)clientArg);

    return (Power_NOTIFYDONE);
}
