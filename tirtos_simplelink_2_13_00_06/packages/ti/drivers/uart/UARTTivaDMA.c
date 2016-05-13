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

#include <ti/drivers/ports/ListP.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>

#include <ti/drivers/uart/UARTTivaDMA.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <inc/hw_uart.h>
#include <driverlib/uart.h>
#include <driverlib/udma.h>

/* TODO: Check if we can remove the #if !defined(CCWARE) */
#if !defined(CCWARE)
#include <driverlib/sysctl.h>
#endif

/* DMA can handle transfers of at most 1024 elements */
#define MAXXFERSIZE 1024

/* UARTTivaDMA functions */
void         UARTTivaDMA_close(UART_Handle handle);
int          UARTTivaDMA_control(UART_Handle handle, unsigned int cmd, void *arg);
void         UARTTivaDMA_init(UART_Handle handle);
UART_Handle  UARTTivaDMA_open(UART_Handle handle, UART_Params *params);
int          UARTTivaDMA_read(UART_Handle handle, void *buffer, size_t size);
void         UARTTivaDMA_readCancel(UART_Handle handle);
int          UARTTivaDMA_readPolling(UART_Handle handle, void *buffer, size_t size);
int          UARTTivaDMA_write(UART_Handle handle, const void *buffer, size_t size);
void         UARTTivaDMA_writeCancel(UART_Handle handle);
int          UARTTivaDMA_writePolling(UART_Handle handle, const void *buffer,
                                   size_t size);

/* UART function table for UARTTivaDMA implementation */
const UART_FxnTable UARTTivaDMA_fxnTable = {
    UARTTivaDMA_close,
    UARTTivaDMA_control,
    UARTTivaDMA_init,
    UARTTivaDMA_open,
    UARTTivaDMA_read,
    UARTTivaDMA_readPolling,
    UARTTivaDMA_readCancel,
    UARTTivaDMA_write,
    UARTTivaDMA_writePolling,
    UARTTivaDMA_writeCancel
};

static const uint32_t dataLength[] = {
    UART_CONFIG_WLEN_5, /* UART_LEN_5 */
    UART_CONFIG_WLEN_6, /* UART_LEN_6 */
    UART_CONFIG_WLEN_7, /* UART_LEN_7 */
    UART_CONFIG_WLEN_8  /* UART_LEN_8 */
};

static const uint32_t stopBits[] = {
    UART_CONFIG_STOP_ONE,   /* UART_STOP_ONE */
    UART_CONFIG_STOP_TWO    /* UART_STOP_TWO */
};

static const uint32_t parityType[] = {
    UART_CONFIG_PAR_NONE,   /* UART_PAR_NONE */
    UART_CONFIG_PAR_EVEN,   /* UART_PAR_EVEN */
    UART_CONFIG_PAR_ODD,    /* UART_PAR_ODD */
    UART_CONFIG_PAR_ZERO,   /* UART_PAR_ZERO */
    UART_CONFIG_PAR_ONE     /* UART_PAR_ONE */
};

/* Default UART params */
extern const UART_Params UART_defaultParams;

static void readCancel(UART_Handle handle);
static void writeCancel(UART_Handle handle);

/*
 *  ======== UARTTivaDMA_configDMA ========
 */
static void UARTTivaDMA_configDMA(UART_Handle handle, bool isWrite)
{
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t                   channelControlOptions;
    unsigned int               key;

    if (isWrite) {
        channelControlOptions = UDMA_SIZE_8 | UDMA_SRC_INC_8 |
                UDMA_DST_INC_NONE | UDMA_ARB_4;

        uDMAChannelControlSet(hwAttrs->txChannelIndex | UDMA_PRI_SELECT,
                channelControlOptions);

        uDMAChannelTransferSet(hwAttrs->txChannelIndex | UDMA_PRI_SELECT,
                UDMA_MODE_BASIC,
                (void *)object->writeBuf,
                (void *)(hwAttrs->baseAddr + UART_O_DR),
                object->writeSize);

        /* A lock is needed because we are accessing shared uDMA registers.*/
        key = Hwi_disable();

        /* Configure channel mapping in the appropriate DMACHMAP register */
        uDMAChannelAssign(hwAttrs->txChannelIndex);

        /* Enable DMA Channel - Set channel bit in ENASET register */
        uDMAChannelEnable(hwAttrs->txChannelIndex);

        Hwi_restore(key);

        /* These two lines added to get TM4C1294XL to work */
        UARTIntClear(hwAttrs->baseAddr, UART_INT_DMATX);
        UARTIntEnable(hwAttrs->baseAddr, UART_INT_DMATX);
    }
    else {
        channelControlOptions = UDMA_SIZE_8 | UDMA_SRC_INC_NONE |
                UDMA_DST_INC_8 | UDMA_ARB_4;

        uDMAChannelControlSet(hwAttrs->rxChannelIndex | UDMA_PRI_SELECT,
                channelControlOptions);

        uDMAChannelTransferSet(hwAttrs->rxChannelIndex | UDMA_PRI_SELECT,
                UDMA_MODE_BASIC,
                (void *)(hwAttrs->baseAddr + UART_O_DR),
                object->readBuf,
                object->readSize);

        /* A lock is needed because we are accessing shared uDMA registers.*/
        key = Hwi_disable();

        /* Configure channel mapping */
        uDMAChannelAssign(hwAttrs->rxChannelIndex);

        /* Enable DMA Channel */
        uDMAChannelEnable(hwAttrs->rxChannelIndex);

        Hwi_restore(key);

        /* These two lines added to get TM4C1294XL to work */
        UARTIntClear(hwAttrs->baseAddr, UART_INT_DMARX);
        UARTIntEnable(hwAttrs->baseAddr, UART_INT_DMARX);
    }

    Log_print1(Diags_USER1,"UART:(%p) DMA transfer enabled", hwAttrs->baseAddr);

    if (isWrite) {
        Log_print3(Diags_USER2,"UART:(%p) DMA transmit, "
                           "txBuf: %p; Count: %d",
                            hwAttrs->baseAddr,
                            (UArg)object->writeBuf,
                            (UArg)object->writeSize);
    }
    else {
        Log_print3(Diags_USER2,"UART:(%p) DMA receive, "
                           "rxBuf: %p; Count: %d",
                            hwAttrs->baseAddr,
                            (UArg)object->readBuf,
                            (UArg)object->readSize);
    }
}

/*
 *  ======== writeSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void writeSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTivaDMA_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&(object->writeSem)));
}

/*
 *  ======== readSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void readSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTivaDMA_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&(object->readSem)));
}

/*
 *  ======== UARTTivaDMA_hwiIntFxn ========
 *  Hwi function that processes the DMA interrupts for the UART.
 */
static void UARTTivaDMA_hwiIntFxn(UArg arg)
{
    uint32_t                   status;
    UARTTivaDMA_Object        *object = ((UART_Handle)arg)->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = ((UART_Handle)arg)->hwAttrs;

    /*
     *  Clear interrupts
     *    UARTIntStatus(base, false) - read the raw interrupt status
     *    UARTIntStatus(base, true)  - read masked interrupt status
     */
    status = UARTIntStatus(hwAttrs->baseAddr, true);

    /*
     *  This block added to get TM4C1294XL to work (otherwise
     *  interrupt keeps firing).
     */
    if (status & UART_INT_DMATX) {
        UARTIntDisable(hwAttrs->baseAddr, UART_INT_DMATX);
        UARTIntClear(hwAttrs->baseAddr, UART_INT_DMATX);
    }

    if (status & UART_INT_DMARX) {
        UARTIntDisable(hwAttrs->baseAddr, UART_INT_DMARX);
        UARTIntClear(hwAttrs->baseAddr, UART_INT_DMARX);
    }

    Log_print2(Diags_USER2, "UART:(%p) Interrupt with mask 0x%x",
               hwAttrs->baseAddr, status);

    /* Read data if characters are available. */
    if (object->readSize && !uDMAChannelIsEnabled(hwAttrs->rxChannelIndex)) {
        object->readCount = object->readSize;
        object->readSize = 0;
        object->readCallback((UART_Handle)arg, object->readBuf,
                object->readCount);

        Log_print2(Diags_USER1, "UART:(%p) Read finished, %d bytes read",
                hwAttrs->baseAddr, object->readCount);
    }

    /* Write completed. */
    if (object->writeSize && !uDMAChannelIsEnabled(hwAttrs->txChannelIndex)) {
        object->writeCount = object->writeSize;
        object->writeSize = 0;
        object->writeCallback((UART_Handle)arg, (uint8_t*)object->writeBuf,
                object->writeCount);

        Log_print2(Diags_USER1, "UART:(%p) Write finished, %d bytes written",
                hwAttrs->baseAddr, object->writeCount);
    }
}

/*
 *  ======== UARTTivaDMA_init ========
 */
void UARTTivaDMA_init(UART_Handle handle)
{
    UARTTivaDMA_Object    *object = handle->object;

    object->opened = false;
}

/*
 *  ======== UARTTivaDMA_open ========
 */
UART_Handle UARTTivaDMA_open(UART_Handle handle, UART_Params *params)
{
    unsigned int               key;
    Types_FreqHz               freq;
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    union {
        Hwi_Params             hwiParams;
        Semaphore_Params       semParams;
    } paramsUnion;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (UART_Params *) &UART_defaultParams;
    }

    /* Timeouts cannot be 0 */
    Assert_isTrue((params->writeTimeout != 0) && (params->readTimeout != 0),
            NULL);

    /* Check that a callback is set */
    Assert_isTrue((params->readMode != UART_MODE_CALLBACK) ||
                  (params->readCallback != NULL), NULL);
    Assert_isTrue((params->writeMode != UART_MODE_CALLBACK) ||
                  (params->writeCallback != NULL), NULL);

    /* Disable preemption while checking if the UART is open. */
    key = Hwi_disable();

    /* Check if the UART is open already with the base addr. */
    if (object->opened == true) {
        Hwi_restore(key);

        Log_warning1("UART:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }

    object->opened = true;
    Hwi_restore(key);

    object->readMode       = params->readMode;
    object->writeMode      = params->writeMode;
    object->readTimeout    = params->readTimeout;
    object->writeTimeout   = params->writeTimeout;
    object->readCallback   = params->readCallback;
    object->writeCallback  = params->writeCallback;
    object->readReturnMode = params->readReturnMode;
    object->readDataMode   = params->readDataMode;
    object->writeDataMode  = params->writeDataMode;
    object->readEcho       = params->readEcho;

    /* Set UART variables to defaults. */
    object->writeBuf = NULL;
    object->readBuf = NULL;
    object->writeCount = 0;
    object->readCount = 0;
    object->writeSize = 0;
    object->readSize = 0;

    /* Disable the UART interrupt. */
    UARTIntDisable(hwAttrs->baseAddr, (UART_INT_TX | UART_INT_RX | UART_INT_RT));

    Hwi_clearInterrupt(hwAttrs->intNum);

    /* Create Hwi object for this UART peripheral. */
    Hwi_Params_init(&(paramsUnion.hwiParams));
    paramsUnion.hwiParams.arg = (UArg)handle;
    paramsUnion.hwiParams.priority = hwAttrs->intPriority;

    Hwi_construct(&(object->hwi), hwAttrs->intNum, UARTTivaDMA_hwiIntFxn,
                  &(paramsUnion.hwiParams), NULL);

    /* Create the semaphores and set callbacks for BLOCKING mode. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        Semaphore_Params_init(&(paramsUnion.semParams));
        paramsUnion.semParams.mode = Semaphore_Mode_BINARY;

        Semaphore_construct(&(object->writeSem), 0, &(paramsUnion.semParams));
        object->writeCallback = &writeSemCallback;
    }

    if (object->readMode == UART_MODE_BLOCKING) {
        Semaphore_Params_init(&(paramsUnion.semParams));
        paramsUnion.semParams.mode = Semaphore_Mode_BINARY;

        Semaphore_construct(&(object->readSem), 0, &(paramsUnion.semParams));
        object->readCallback = &readSemCallback;
    }

    /*
     *  Set the FIFO level to 4/8 empty and 4/8 full.
     *  The UART generates a burst request based on the FIFO trigger
     *  level. The arbitration size should be set to the amount
     *  of data that the FIFO can transfer when the trigger level is reached.
     *  Since arbitration size is a power of 2, we'll set the FIFO levels
     *  to 4_8 so they can match the arbitration size of 4.
     */
    UARTFIFOLevelSet(hwAttrs->baseAddr, UART_FIFO_TX4_8, UART_FIFO_RX4_8);

    BIOS_getCpuFreq(&freq);
    UARTConfigSetExpClk(hwAttrs->baseAddr,
                        freq.lo,
                        params->baudRate,
                        dataLength[params->dataLength] |
                        stopBits[params->stopBits] |
                        parityType[params->parityType]);

    Log_print3(Diags_USER1, "UART:(%p) CPU freq: %d; UART baudrate to %d",
               hwAttrs->baseAddr, freq.lo, params->baudRate);

    UARTDMAEnable(hwAttrs->baseAddr, UART_DMA_TX | UART_DMA_RX);

    UARTEnable(hwAttrs->baseAddr);

    Log_print1(Diags_USER1, "UART:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== UARTTivaDMA_close ========
 */
void UARTTivaDMA_close(UART_Handle handle)
{
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Disable UART. */
    UARTDMADisable(hwAttrs->baseAddr, UART_DMA_TX | UART_DMA_RX);
    UARTDisable(hwAttrs->baseAddr);

    /* Destruct the SYS/BIOS objects. */
    Hwi_destruct(&(object->hwi));

    if (object->writeMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&(object->writeSem));
    }

    if (object->readMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&(object->readSem));
    }

    object->opened = false;

    Log_print1(Diags_USER1, "UART:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== UARTTivaDMA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int UARTTivaDMA_control(UART_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (UART_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== UARTTivaDMA_write ========
 */
int UARTTivaDMA_write(UART_Handle handle, const void *buffer, size_t size)
{
    unsigned int        key;
    UARTTivaDMA_Object *object = handle->object;

    /* Check that there is data to write */
    Assert_isTrue(size != 0, NULL);

    /* The DMA can only handle transfers up to 1024 elements */
    if (size > MAXXFERSIZE) {
        Log_warning1("UART:(%p) Data size too large.",
                ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        return (UART_ERROR);
    }

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->writeSize) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) Could not write data, uart in use.",
                ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        return (UART_ERROR);
    }

    /* Save the data to be written and restore interrupts. */
    object->writeBuf = buffer;
    object->writeCount = 0;
    object->writeSize = size;

    Hwi_restore(key);

    UARTTivaDMA_configDMA(handle, true /* isWrite */);

    /* If writeMode is blocking, block and get the status. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&(object->writeSem)),
                    object->writeTimeout)) {

            /* Cancel the DMA without posting the semaphore */
            writeCancel(handle);
            Log_print2(Diags_USER1,
                    "UART:(%p) Write timed out, %d bytes written",
                    ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                    object->writeCount);

        }
        return (object->writeCount);
    }

    return (0);
}

/*
 *  ======== UARTTivaDMA_writePolling ========
 */
int UARTTivaDMA_writePolling(UART_Handle handle, const void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Write characters. */
    while (size) {
        if (object->writeDataMode == UART_DATA_TEXT && *buffer == '\n') {
            UARTCharPut(hwAttrs->baseAddr, '\r');
            count++;
        }
        UARTCharPut(hwAttrs->baseAddr, *buffer);

        Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
                   ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                   *buffer);
        buffer++;
        count++;
        size--;
    }

    Log_print2(Diags_USER1,
            "UART:(%p) Write polling finished, %d bytes written",
            ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
            count);

    return (count);
}

/*
 *  ======== UARTTivaDMA_writeCancel ========
 */
void UARTTivaDMA_writeCancel(UART_Handle handle)
{
    UARTTivaDMA_Object        *object = handle->object;

    writeCancel(handle);
    object->writeCallback(handle, (uint8_t*)object->writeBuf, object->writeCount);

    Log_print2(Diags_USER1, "UART:(%p) Write canceled, %d bytes written",
               ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
            object->writeCount);
}

/*
 *  ======== UARTTivaDMA_read ========
 */
int UARTTivaDMA_read(UART_Handle handle, void *buffer, size_t size)
{
    unsigned int        key;
    UARTTivaDMA_Object *object = handle->object;

    if (size > MAXXFERSIZE) {
        Log_warning1("UART:(%p) Data size too large.",
                ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        return (UART_ERROR);
    }

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->readSize) {
        Hwi_restore(key);

        Log_warning1("UART:(%p) Could not read data, uart in use.",
                ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);
        return (UART_ERROR);
    }

    /* Save the data to be read and restore interrupts. */
    object->readBuf = buffer;
    object->readSize = size;
    object->readCount = 0;

    Hwi_restore(key);

    /* Start the DMA transfer */
    UARTTivaDMA_configDMA(handle, false /* isWrite */);

    /* If readMode is blocking, block and get the status. */
    if (object->readMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&(object->readSem)),
                            object->readTimeout)) {
            /* Cancel the DMA without posting the semaphore */
            readCancel(handle);
            Log_print2(Diags_USER1, "UART:(%p) Read timed out, %d bytes read",
                    ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                    object->readCount);

        }
        return (object->readCount);
    }

    return (0);
}

/*
 *  ======== UARTTivaDMA_readPolling ========
 */
int UARTTivaDMA_readPolling(UART_Handle handle, void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Read characters. */
    while (size) {
        *buffer = UARTCharGet(hwAttrs->baseAddr);
        Log_print2(Diags_USER2, "UART:(%p) Read character 0x%x",
                   hwAttrs->baseAddr, *buffer);
        count++;
        size--;

        if (object->readDataMode == UART_DATA_TEXT && *buffer == '\r') {
            /* Echo character if enabled. */
            if (object->readEcho) {
                UARTCharPut(hwAttrs->baseAddr, '\r');
            }
            *buffer = '\n';
        }

        /* Echo character if enabled. */
        if (object->readEcho) {
            UARTCharPut(hwAttrs->baseAddr, *buffer);
        }

        /* If read return mode is newline, finish if a newline was received. */
        if (object->readReturnMode == UART_RETURN_NEWLINE &&
            *buffer == '\n') {
            return (count);
        }

        buffer++;
    }

    Log_print2(Diags_USER1, "UART:(%p) Read polling finished, %d bytes read",
               hwAttrs->baseAddr, count);

    return (count);
}

/*
 *  ======== UARTTivaDMA_readCancel ========
 */
void UARTTivaDMA_readCancel(UART_Handle handle)
{
    UARTTivaDMA_Object        *object = handle->object;

    readCancel(handle);
    object->readCallback(handle, object->readBuf, object->readCount);

    Log_print2(Diags_USER1, "UART:(%p) Read canceled, %d bytes read",
               ((UARTTivaDMA_HWAttrs const *)(handle->hwAttrs))->baseAddr,
            object->readCount);
}

/*
 *  ======== readCancel ========
 */
static void readCancel(UART_Handle handle)
{
    unsigned int               key;
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t                   remainder;
    int                        bytesTransferred;

    /* Disable interrupts to avoid reading data while changing state. */
    key = Hwi_disable();

    /* Return if there is no read. */
    if (!object->readSize) {
        Hwi_restore(key);
        return;
    }

    /* Set channel bit in the ENACLR register */
    uDMAChannelDisable(hwAttrs->rxChannelIndex);

    remainder = uDMAChannelSizeGet(hwAttrs->rxChannelIndex);
    bytesTransferred = object->readSize - remainder;

    /* Set size = 0 to prevent reading and restore interrupts. */
    object->readSize = 0;
    object->readCount = bytesTransferred;
    Hwi_restore(key);
}

/*
 *  ======== writeCancel ========
 */
static void writeCancel(UART_Handle handle)
{
    unsigned int               key;
    UARTTivaDMA_Object        *object = handle->object;
    UARTTivaDMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    uint32_t                   remainder;
    int                        bytesTransferred;

    /* Disable interrupts to avoid writing data while changing state. */
    key = Hwi_disable();

    /* Set channel bit in the ENACLR register */
    uDMAChannelDisable(hwAttrs->txChannelIndex);

    remainder = uDMAChannelSizeGet(hwAttrs->txChannelIndex);
    bytesTransferred = object->writeSize - remainder;

    /* Return if there is no write. */
    if (!object->writeSize) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent writing and restore interrupts. */
    object->writeSize = 0;
    object->writeCount = bytesTransferred;
    Hwi_restore(key);
}
