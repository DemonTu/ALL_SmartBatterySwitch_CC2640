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
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>

#include <ti/drivers/i2c/I2CEUSCIB.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/msp430/ClockFreqs.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Semaphore.h>

/* driverlib header files */
#include <inc/hw_regaccess.h>
#include <eusci_b_i2c.h>

#define ERROR_INTERRUPTS (EUSCI_B_I2C_NAK_INTERRUPT | \
                          EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT)

#define ALL_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
                        EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | \
                        ERROR_INTERRUPTS)

extern const I2C_Config I2C_config[];

/* Prototypes */
void         I2CEUSCIB_close(I2C_Handle handle);
int          I2CEUSCIB_control(I2C_Handle handle, unsigned int cmd, void *arg);
void         I2CEUSCIB_init(I2C_Handle handle);
I2C_Handle   I2CEUSCIB_open(I2C_Handle handle, I2C_Params *params);
static void  I2CEUSCIB_primeTransfer(I2CEUSCIB_Object        *object,
                                    I2CEUSCIB_HWAttrs const *hwAttrs,
                                    I2C_Transaction *transferMessage);
bool         I2CEUSCIB_transfer(I2C_Handle handle,
                                     I2C_Transaction *transaction);
static void  I2CEUSCIB_blockingCallback(I2C_Handle handle,
                                       I2C_Transaction *msg,
                                       bool transferStatus);
static void  I2CEUSCIB_completeTransfer(I2C_Handle handle);

/* I2C function table for I2CI2CEUSCIB implementation */
const I2C_FxnTable I2CEUSCIB_fxnTable = {
    I2CEUSCIB_close,
    I2CEUSCIB_control,
    I2CEUSCIB_init,
    I2CEUSCIB_open,
    I2CEUSCIB_transfer
};

static const uint32_t bitRate[] = {
    EUSCI_B_I2C_SET_DATA_RATE_100KBPS,  /*  I2C_100kHz = 0 */
    EUSCI_B_I2C_SET_DATA_RATE_400KBPS   /*  I2C_400kHz = 1 */
};

/* Default I2C params */
extern const I2C_Params I2C_defaultParams;

/*
 *  ======== I2CEUSCIB_close ========
 */
void I2CEUSCIB_close(I2C_Handle handle)
{
    I2CEUSCIB_Object        *object = handle->object;
    I2CEUSCIB_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Check to see if a I2C transaction is in progress */
    Assert_isTrue(object->headPtr == NULL, NULL);

    /* Mask I2C interrupts */
    EUSCI_B_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    /* Disable the I2C Master */
    EUSCI_B_I2C_disable(hwAttrs->baseAddr);

    Semaphore_destruct(&(object->mutex));
    if (object->transferMode == I2C_MODE_BLOCKING) {
        Semaphore_destruct(&(object->transferComplete));
    }

    object->isOpen = false;

    Log_print1(Diags_USER1, "I2C: Object closed 0x%x", hwAttrs->baseAddr);

    return;
}

/*
 *  ======== I2CEUSCIB_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int I2CEUSCIB_control(I2C_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (I2C_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== I2CEUSCIB_hwiIntFxn ========
 *  Hwi interrupt handler to service the I2C peripheral
 *
 *  The handler is a generic handler for a I2C object.
 */
void I2CEUSCIB_hwiIntFxn(UArg index)
{
    unsigned int            key;
    uint8_t                  intStatus;
    I2CEUSCIB_Object        *object = (&I2C_config[index])->object;
    I2CEUSCIB_HWAttrs const *hwAttrs = (&I2C_config[index])->hwAttrs;

    /* Get the interrupt status of the I2C controller */
    intStatus = EUSCI_B_I2C_getInterruptStatus(hwAttrs->baseAddr,ALL_INTERRUPTS);

    Log_print1(Diags_USER2, "Status Reg: 0x%x", intStatus);

    /* Clear interrupt source */
    EUSCI_B_I2C_clearInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    /* Check for I2C Errors */
    if ((intStatus & ERROR_INTERRUPTS) || (object->mode == I2CEUSCIB_ERROR)) {
        /* Some sort of error happened! */
        object->mode = I2CEUSCIB_ERROR;

        /* Try to send a STOP bit to end all I2C communications immediately
         *
         * EUSCI_B_I2C_masterReceiveMultiByteStop only sends a STOP condition
         * which is needed here
         */
        EUSCI_B_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

        Log_print2(Diags_USER1, "I2C:(%p) ISR I2C Bus fault (Status Reg: 0x%x)",
                                 hwAttrs->baseAddr,
                                 intStatus);
    }

    /* No errors, now check what we need to do next */
    switch (object->mode) {

        /*
         * ERROR case is OK because if an Error is detected, a STOP bit is
         * sent; which in turn will call another interrupt. This interrupt
         * call will then post the transferComplete semaphore to unblock the
         * I2C_transfer function
         */
        case I2CEUSCIB_ERROR:
        case I2CEUSCIB_IDLE_MODE:
            I2CEUSCIB_completeTransfer((I2C_Handle)&I2C_config[index]);
            break;

        case I2CEUSCIB_WRITE_MODE:
            if (object->writeCountIdx) {

                /* If we're on the last byte to write and have data to read */
                if ((object->writeCountIdx == 1) && (!object->readCountIdx)) {
                    /* Next state: Idle mode */
                    object->mode = I2CEUSCIB_IDLE_MODE;

                    /* Send last byte with STOP bit */
                    EUSCI_B_I2C_masterSendMultiByteFinish(hwAttrs->baseAddr,
                                                          *(object->writeBufIdx));

                    EUSCI_B_I2C_clearInterrupt(hwAttrs->baseAddr,
                                               EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

                    Log_print2(Diags_USER2, "I2C:(%p) ISR I2CEUSCIB_WRITE_MODE:"
                                            "Data to write: 0x%x;"
                                            "Writing w/ STOP bit",
                                             hwAttrs->baseAddr,
                                            *(object->writeBufIdx));

                    I2CEUSCIB_completeTransfer((I2C_Handle)&I2C_config[index]);
                }
                else {
                    /* Write data contents into data register */
                    EUSCI_B_I2C_masterSendMultiByteNext(hwAttrs->baseAddr,
                                                    *(object->writeBufIdx));

                    Log_print3(Diags_USER2,
                            "I2C:(%p) ISR I2CEUSCIB_WRITE_MODE: "
                            "Data to write: 0x%x; "
                            "To slave: 0x%x",
                            hwAttrs->baseAddr,
                            *(object->writeBufIdx),
                            object->currentTransaction->slaveAddress);

                    object->writeBufIdx++;
                    object->writeCountIdx--;
                }
            }
            /*
             * Done writing data to the I2C slave. If no data needed to be read,
             * the ISR would not get here as it would have finished the I2C
             * transfer in the logical check above.
             */
            else {
                /* Next state: Receive mode */
                object->mode = I2CEUSCIB_READ_MODE;

                if (object->readCountIdx > 1) {
                    /* Start the I2C transfer in master receive mode */
                    EUSCI_B_I2C_masterReceiveStart(hwAttrs->baseAddr);
                    Log_print1(Diags_USER2, "I2C:(%p) I2CEUSCIB_WRITE_MODE: -> I2CEUSCIB_READ_MODE; "
                                    "Reading w/ RESTART",
                                     hwAttrs->baseAddr);
                }
                else {
                /*
                 * Start the I2C transfer in master receive mode by sending a START
                 * and STOP bit since its the last byte
                 */
                    EUSCI_B_I2C_masterReceiveStart(hwAttrs->baseAddr);
                    while ( EUSCI_B_I2C_masterIsStartSent(hwAttrs->baseAddr) ) ;
                    EUSCI_B_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

                    Log_print1(Diags_USER2, "I2C:(%p) I2CEUSCIB_WRITE_MODE: -> I2CEUSCIB_READ_MODE; "
                                    "Reading w/ RESTART and STOP",
                                     hwAttrs->baseAddr);
                }
            }
            break; /* I2CEUSCIB_WRITE_MODE */

        case I2CEUSCIB_READ_MODE:
            /*
             * Data has been read from RXBUF, but the next byte is
             * already been shifted
             */
            object->readCountIdx--;

            if (object->readCountIdx == 1) {
                key = Hwi_disable();

                EUSCI_B_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);
                *(object->readBufIdx) =
                    EUSCI_B_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);
                Hwi_restore(key);

                Log_print2(Diags_USER2, "I2C:(%p) ISR I2CEUSCIB_READ_MODE: "
                                        "Read data byte: 0x%x ",
                                         hwAttrs->baseAddr,
                                        *(object->readBufIdx));
                object->readBufIdx++;
            }
            else if (object->readCountIdx > 1) {
                *(object->readBufIdx) =
                    EUSCI_B_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                Log_print2(Diags_USER2, "I2C:(%p) ISR I2CEUSCIB_READ_MODE: "
                                        "Read data byte: 0x%x ",
                                         hwAttrs->baseAddr,
                                        *(object->readBufIdx));
                object->readBufIdx++;
            }
            else {
                /* Next state: Idle mode */
                object->mode = I2CEUSCIB_IDLE_MODE;

                *(object->readBufIdx) =
                    EUSCI_B_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                Log_print2(Diags_USER2, "I2C:(%p) ISR I2CEUSCIB_READ_MODE: "
                                    "Read data byte: 0x%x ",
                                     hwAttrs->baseAddr,
                                    *(object->readBufIdx));

                I2CEUSCIB_completeTransfer((I2C_Handle)&I2C_config[index]);
            }
            break; /* I2CEUSCIB_READ_MODE */

        default:
            object->mode = I2CEUSCIB_ERROR;
            break;
    }

    return;
}

/*
 *  ======== I2CEUSCIB_completeTransfer =======
 */
static void I2CEUSCIB_completeTransfer(I2C_Handle handle)
{
    I2CEUSCIB_Object    *object = handle->object;

    Log_print1(Diags_USER2, "I2C:(%p) ISR Transfer Complete",
               ((I2CEUSCIB_HWAttrs const  *)(handle->hwAttrs))->baseAddr);

    /*
     * Perform callback in a HWI context, thus any tasks or SWIs
     * made ready to run won't start until the interrupt has
     * finished
     */
    object->transferCallbackFxn(handle,
                                object->currentTransaction,
                                !((bool)object->mode));

    /* See if we need to process any other transactions */
    if (object->headPtr == object->tailPtr) {
        /* No other transactions need to occur */
        object->currentTransaction = NULL;
        object->headPtr = NULL;

        Log_print1(Diags_USER2,"I2C:(%p) ISR No other I2C transaction in queue",
                   ((I2CEUSCIB_HWAttrs const  *)(handle->hwAttrs))->baseAddr);
    }
    else {
        /* Another transfer needs to take place */
        object->headPtr = object->headPtr->nextPtr;

        Log_print2(Diags_USER2, "I2C:(%p) ISR Priming next I2C transaction "
                                "(%p) from queue",
                   ((I2CEUSCIB_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                   (UArg)object->headPtr);

        I2CEUSCIB_primeTransfer(object,
                               (I2CEUSCIB_HWAttrs const *)(handle->hwAttrs),
                               object->headPtr);
    }

    return;
}

/*
 *  ======== I2CEUSCIB_init ========
 */
void I2CEUSCIB_init(I2C_Handle handle)
{
    /* Mark the object as available */
    ((I2CEUSCIB_Object *)(handle->object))->isOpen = false;
}

/*
 *  ======== I2CEUSCIB_open ========
 */
I2C_Handle I2CEUSCIB_open(I2C_Handle handle, I2C_Params *params)
{
    unsigned int                key;
    uint32_t                    clockFreq;
    EUSCI_B_I2C_initMasterParam eusciI2cParams;
    Semaphore_Params            semParams;
    I2CEUSCIB_Object           *object = handle->object;
    I2CEUSCIB_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Determine if the device index was already opened */
    key = Hwi_disable();
    if (object->isOpen == true) {
        Hwi_restore(key);
        return (NULL);
    }
    /* Mark the handle as being used */
    object->isOpen = true;
    Hwi_restore(key);

    /* Store the I2C parameters */
    if (params == NULL) {
        /* No params passed in, so use the defaults */
        params = (I2C_Params *) &I2C_defaultParams;
    }

    /* Save parameters */
    object->transferMode = params->transferMode;
    object->transferCallbackFxn = params->transferCallbackFxn;

    /*
     * Create threadsafe handles for this I2C peripheral
     * Semaphore to provide exclusive access to the I2C peripheral
     */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    Semaphore_construct(&(object->mutex), 1, &semParams);

    /*
     * Store a callback function that posts the transfer complete
     * semaphore for synchronous mode
     */
    if (object->transferMode == I2C_MODE_BLOCKING) {
        /*
         * Semaphore to cause the waiting task to block for the I2C
         * to finish
         */
        Semaphore_construct(&(object->transferComplete), 0, &semParams);

        /* Store internal callback function */
        object->transferCallbackFxn = I2CEUSCIB_blockingCallback;
    }
    else {
        /* Check to see if a callback function was defined for async mode */
        Assert_isTrue(object->transferCallbackFxn != NULL, NULL);
    }

    /* Get the I2C clock input frequency */
    switch (hwAttrs->clockSource) {
        case EUSCI_B_I2C_CLOCKSOURCE_ACLK:
            clockFreq = ClockFreqs_getFrequency(ClockFreqs_Clock_ACLK);
            Log_print1(Diags_USER1, "ClockFreqs_getFrequency ACLK: %d", clockFreq);
            break;

        case EUSCI_B_I2C_CLOCKSOURCE_SMCLK:
            clockFreq = ClockFreqs_getFrequency(ClockFreqs_Clock_SMCLK);
            Log_print1(Diags_USER1, "ClockFreqs_getFrequency SMCLK: %d", clockFreq);
            break;

        default:
            Log_error0("I2C: Error determining clock source");
            I2CEUSCIB_close(handle);
            return (NULL);
    }

    /* Specify the idle state for this I2C peripheral */
    object->mode = I2CEUSCIB_IDLE_MODE;

    /* Clear the head pointer */
    object->headPtr = NULL;
    object->tailPtr = NULL;

    Log_print1(Diags_USER1, "I2C: Object created 0x%x", hwAttrs->baseAddr);

    eusciI2cParams.selectClockSource = hwAttrs->clockSource;
    eusciI2cParams.i2cClk = clockFreq;
    eusciI2cParams.dataRate = bitRate[params->bitRate];
    EUSCI_B_I2C_initMaster(hwAttrs->baseAddr, &eusciI2cParams);

    /* Clear any pending interrupts */
    EUSCI_B_I2C_clearInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    /* Enable I2CEUSCIB Module to start operations */
    EUSCI_B_I2C_enable(hwAttrs->baseAddr);

    /* Unmask I2CEUSCIB interrupts */
    EUSCI_B_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== I2CEUSCIB_primeTransfer =======
 */
static void I2CEUSCIB_primeTransfer(I2CEUSCIB_Object        *object,
                                    I2CEUSCIB_HWAttrs const *hwAttrs,
                                    I2C_Transaction         *transaction)
{
    /* Store the new internal counters and pointers */
    object->currentTransaction = transaction;

    object->writeBufIdx = transaction->writeBuf;
    object->writeCountIdx = transaction->writeCount;

    object->readBufIdx = transaction->readBuf;
    object->readCountIdx = transaction->readCount;

    Log_print2(Diags_USER1, "I2C:(%p) Starting transaction to slave: 0x%x",
                             hwAttrs->baseAddr,
                             object->currentTransaction->slaveAddress);

    /* Specify the I2C slave address */
    EUSCI_B_I2C_setSlaveAddress(hwAttrs->baseAddr,
                             object->currentTransaction->slaveAddress);

    /* Start transfer in Transmit mode */
    if (object->writeCountIdx) {
        EUSCI_B_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_TRANSMIT_MODE);

        /* Update the I2C mode */
        object->mode = I2CEUSCIB_WRITE_MODE;

        /* Start the I2C transfer by sending the start bit */
        EUSCI_B_I2C_masterSendStart(hwAttrs->baseAddr);

        Log_print1(Diags_USER2, "I2C:(%p) I2CEUSCIB_IDLE_MODE: -> I2CEUSCIB_WRITE_MODE; "
                                "Writing w/ START",
                                 hwAttrs->baseAddr);
    }
    /* Start transfer in Receive mode */
    else {
        /* Update the I2C mode */
        object->mode = I2CEUSCIB_READ_MODE;

        if (object->readCountIdx > 1) {
            /* Start the I2C transfer in master receive mode */
            EUSCI_B_I2C_masterReceiveStart(hwAttrs->baseAddr);

            Log_print1(Diags_USER2, "I2C:(%p) I2CEUSCIB_IDLE_MODE: -> I2CEUSCIB_READ_MODE; "
                                    "Reading w/ ACK",
                                     hwAttrs->baseAddr);
        }
        else {
            /*
             * Start the I2C transfer in master receive mode by sending a START
             * and STOP bit
             */
            EUSCI_B_I2C_masterReceiveStart(hwAttrs->baseAddr);
            while (EUSCI_B_I2C_masterIsStartSent(hwAttrs->baseAddr));
            EUSCI_B_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

            Log_print1(Diags_USER2, "I2C:(%p) I2CEUSCIB_IDLE_MODE: -> I2CEUSCIB_READ_MODE; "
                                    "Reading w/ NACK",
                                     hwAttrs->baseAddr);
        }
    }
}

/*
 *  ======== I2CEUSCIB_transfer ========
 */
bool I2CEUSCIB_transfer(I2C_Handle handle,
                              I2C_Transaction *transaction)
{
    unsigned int             key;
    bool                     ret = false;
    I2CEUSCIB_Object        *object = handle->object;
    I2CEUSCIB_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Check if anything needs to be written or read */
    if ((!transaction->writeCount) && (!transaction->readCount)) {
        /* Nothing to write or read */
        return (ret);
    }

    if (object->transferMode == I2C_MODE_CALLBACK) {
        /* Check if a transfer is in progress */
        key = Hwi_disable();
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
            Hwi_restore(key);
            return (true);
        }
        else {
            /* Store the headPtr indicating I2C is in use */
            object->headPtr = transaction;
            object->tailPtr = transaction;
        }
        Hwi_restore(key);
    }

    /* Acquire the lock for this particular I2C handle */
    Semaphore_pend(Semaphore_handle(&(object->mutex)), BIOS_WAIT_FOREVER);

    /*
     * I2CEUSCIB_primeTransfer is a longer process and
     * protection is needed from the I2C interrupt
     */
    EUSCI_B_I2C_disableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);
    I2CEUSCIB_primeTransfer(object, hwAttrs, transaction);
    EUSCI_B_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_INTERRUPTS);

    if (object->transferMode == I2C_MODE_BLOCKING) {
        Log_print1(Diags_USER1,"I2C:(%p) Pending on transferComplete semaphore",
                                hwAttrs->baseAddr);
        /*
         * Wait for the transfer to complete here.
         * It's OK to block from here because the I2C's Hwi will unblock
         * upon errors
         */
        Semaphore_pend(Semaphore_handle(&(object->transferComplete)),
                BIOS_WAIT_FOREVER);

        Log_print1(Diags_USER1, "I2C:(%p) Transaction completed",
                                 hwAttrs->baseAddr);

        /* Hwi handle has posted a 'transferComplete' check for Errors */
        if (object->mode == I2CEUSCIB_IDLE_MODE) {
            Log_print1(Diags_USER1, "I2C:(%p) Transfer OK", hwAttrs->baseAddr);
            ret = true;
        }
    }
    else {
        /* Always return true if in Asynchronous mode */
        ret = true;
    }

    /* Release the lock for this particular I2C handle */
    Semaphore_post(Semaphore_handle(&(object->mutex)));

    /* Return the number of bytes transfered by the I2C */
    return (ret);
}

/*
 *  ======== I2CEUSCIB_blockingCallback ========
 */
static void I2CEUSCIB_blockingCallback(I2C_Handle handle,
                                      I2C_Transaction *msg,
                                      bool transferStatus)
{
    I2CEUSCIB_Object        *object = handle->object;

    Log_print1(Diags_USER1, "I2C:(%p) posting transferComplete semaphore",
               ((I2CEUSCIB_HWAttrs const *)(handle->hwAttrs))->baseAddr);

    /* Indicate transfer complete */
    Semaphore_post(Semaphore_handle(&(object->transferComplete)));
}
