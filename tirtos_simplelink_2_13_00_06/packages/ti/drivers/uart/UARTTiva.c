/*
 * Copyright (c) 2014-2015, Texas Instruments Incorporated
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
#include <xdc/runtime/Types.h>

#include <ti/drivers/uart/UARTTiva.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <driverlib/uart.h>

#if !defined(CCWARE)
#include <driverlib/sysctl.h>
#endif

/* UARTTiva functions */
void         UARTTiva_close(UART_Handle handle);
int          UARTTiva_control(UART_Handle handle, unsigned int cmd, void *arg);
void         UARTTiva_init(UART_Handle handle);
UART_Handle  UARTTiva_open(UART_Handle handle, UART_Params *params);
int          UARTTiva_read(UART_Handle handle, void *buffer, size_t size);
void         UARTTiva_readCancel(UART_Handle handle);
int          UARTTiva_readPolling(UART_Handle handle, void *buffer,
                size_t size);
int          UARTTiva_write(UART_Handle handle, const void *buffer,
                size_t size);
void         UARTTiva_writeCancel(UART_Handle handle);
int          UARTTiva_writePolling(UART_Handle handle, const void *buffer,
                                   size_t size);

/* Static functions */
static void errorCallback(UART_Handle handle, uintptr_t error);
static Void readBlockingTimeout(UArg arg);
static bool readIsrBinaryBlocking(UART_Handle handle);
static bool readIsrBinaryCallback(UART_Handle handle);
static bool readIsrTextBlocking(UART_Handle handle);
static bool readIsrTextCallback(UART_Handle handle);
static void readSemCallback(UART_Handle handle, void *buffer, size_t count);
static int  readTaskBlocking(UART_Handle handle);
static int  readTaskCallback(UART_Handle handle);
static void writeData(UART_Handle handle);
static void writeSemCallback(UART_Handle handle, void *buffer, size_t count);

/* UART function table for UARTTiva implementation */
const UART_FxnTable UARTTiva_fxnTable = {
    UARTTiva_close,
    UARTTiva_control,
    UARTTiva_init,
    UARTTiva_open,
    UARTTiva_read,
    UARTTiva_readPolling,
    UARTTiva_readCancel,
    UARTTiva_write,
    UARTTiva_writePolling,
    UARTTiva_writeCancel
};

static const uint32_t dataLength[] = {
    UART_CONFIG_WLEN_5,     /* UART_LEN_5 */
    UART_CONFIG_WLEN_6,     /* UART_LEN_6 */
    UART_CONFIG_WLEN_7,     /* UART_LEN_7 */
    UART_CONFIG_WLEN_8      /* UART_LEN_8 */
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

/*
 *  ======== staticFxnTable ========
 *  This is a function lookup table to simplify the UART driver modes.
 */
static const UARTTiva_FxnSet staticFxnTable[2][2] = {
    {/* UART_MODE_BLOCKING */
        {/* UART_DATA_BINARY */
            .readIsrFxn  = readIsrBinaryBlocking,
            .readTaskFxn = readTaskBlocking
        },
        {/* UART_DATA_TEXT */
            .readIsrFxn  = readIsrTextBlocking,
            .readTaskFxn = readTaskBlocking
        }
    },
    {/* UART_MODE_CALLBACK */
        {/* UART_DATA_BINARY */
            .readIsrFxn  = readIsrBinaryCallback,
            .readTaskFxn = readTaskCallback

        },
        {/* UART_DATA_TEXT */
            .readIsrFxn  = readIsrTextCallback,
            .readTaskFxn = readTaskCallback,
        }
    }
};

/*
 *  ======== UARTTiva_close ========
 */
void UARTTiva_close(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Disable UART and interrupts. */
    UARTIntDisable(hwAttrs->baseAddr, UART_INT_TX | UART_INT_RX | UART_INT_RT |
         UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE);
    UARTDisable(hwAttrs->baseAddr);

    Hwi_destruct(&(object->hwi));

    if (object->state.writeMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&object->writeSem);
    }

    if (object->state.readMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&object->readSem);
        Clock_destruct(&object->timeoutClk);
    }

    object->state.opened = false;

    Log_print1(Diags_USER1, "UART:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== UARTTiva_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int UARTTiva_control(UART_Handle handle, unsigned int cmd, void *arg)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned char              data;
    int                        bufferCount;

    bufferCount = RingBuf_peek(&object->ringBuffer, &data);

    switch (cmd) {
        /* Common UART CMDs */
        case (UART_CMD_PEEK):
            *(int *)arg = (bufferCount) ? data : UART_ERROR;
            Log_print2(Diags_USER1, "UART:(%p) UART_CMD_PEEK: %d",
                hwAttrs->baseAddr, *(uintptr_t*)arg);
            return (UART_STATUS_SUCCESS);

        case (UART_CMD_ISAVAILABLE):
            *(bool *)arg = (bufferCount != 0);
            Log_print2(Diags_USER1, "UART:(%p) UART_CMD_ISAVAILABLE: %d",
                hwAttrs->baseAddr, *(uintptr_t*)arg);
            return (UART_STATUS_SUCCESS);

        case (UART_CMD_GETRXCOUNT):
            *(int *)arg = bufferCount;
            Log_print2(Diags_USER1, "UART:(%p) UART_CMD_GETRXCOUNT: %d",
                hwAttrs->baseAddr, *(uintptr_t*)arg);
            return (UART_STATUS_SUCCESS);

        case (UART_CMD_RXENABLE):
            if (!object->state.rxEnabled) {
                UARTIntEnable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT);
                object->state.rxEnabled = true;
                Log_print1(Diags_USER1, "UART:(%p) UART_CMD_RXENABLE: Enabled",
                    hwAttrs->baseAddr);
                return (UART_STATUS_SUCCESS);
            }
            Log_print1(Diags_USER1, "UART:(%p) UART_CMD_RXENABLE: Already enabled",
                hwAttrs->baseAddr);
            return (UART_STATUS_ERROR);

        case (UART_CMD_RXDISABLE):
            if (object->state.rxEnabled) {
                UARTIntDisable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT);
                object->state.rxEnabled = false;
                Log_print1(Diags_USER1, "UART:(%p) UART_CMD_RXENABLE: Disabled",
                    hwAttrs->baseAddr);
                return (UART_STATUS_SUCCESS);
            }
            Log_print1(Diags_USER1, "UART:(%p) UART_CMD_RXENABLE: Already disabled",
                hwAttrs->baseAddr);
            return (UART_STATUS_ERROR);

        default:
            Log_print2(Diags_USER1, "UART:(%p) UART CMD undefined: %d",
                ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr, cmd);
            return (UART_STATUS_UNDEFINEDCMD);
    }
}

/*
 *  ======== UARTTiva_hwiIntFxn ========
 *  Hwi function that processes UART interrupts.
 *
 *  @param(arg)         The UART_Handle for this Hwi.
 */
static void UARTTiva_hwiIntFxn(UArg arg)
{
    uint32_t                   status;
    UARTTiva_Object           *object = ((UART_Handle)arg)->object;
    UARTTiva_HWAttrs const    *hwAttrs = ((UART_Handle)arg)->hwAttrs;
    bool                       readSuccess = true;

    /* Clear interrupts */
    status = UARTIntStatus(hwAttrs->baseAddr, true);
    UARTIntClear(hwAttrs->baseAddr, status);

    if (status & (UART_INT_RX | UART_INT_RT)) {
        readSuccess = object->readFxns.readIsrFxn((UART_Handle)arg);
    }

    if (status & UART_INT_TX) {
        writeData((UART_Handle)arg);
    }

    if (status & (UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE) ||
        !readSuccess) {
        errorCallback((UART_Handle)arg,
            status & (UART_INT_OE|UART_INT_BE|UART_INT_PE|UART_INT_FE));
    }
}

/*
 *  ======== UARTTiva_init ========
 */
void UARTTiva_init(UART_Handle handle)
{
    UARTTiva_Object    *object = handle->object;

    object->state.opened = false;
}

/*
 *  ======== UARTTiva_open ========
 */
UART_Handle UARTTiva_open(UART_Handle handle, UART_Params *params)
{
    unsigned int               key;
    Types_FreqHz               freq;
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    union {
        Hwi_Params             hwiParams;
        Semaphore_Params       semParams;
        Clock_Params           clockParams;
    } paramsUnion;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (UART_Params *) &UART_defaultParams;
    }

    /* Check that a callback is set */
    Assert_isTrue((params->readMode != UART_MODE_CALLBACK) ||
                  (params->readCallback != NULL), NULL);
    Assert_isTrue((params->writeMode != UART_MODE_CALLBACK) ||
                  (params->writeCallback != NULL), NULL);

    key = Hwi_disable();

    if (object->state.opened == true) {
        Hwi_restore(key);

        Log_warning1("UART:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }
    object->state.opened = true;

    Hwi_restore(key);

    object->state.readMode       = params->readMode;
    object->state.writeMode      = params->writeMode;
    object->state.readReturnMode = params->readReturnMode;
    object->state.readDataMode   = params->readDataMode;
    object->state.writeDataMode  = params->writeDataMode;
    object->state.readEcho       = params->readEcho;
    object->readTimeout          = params->readTimeout;
    object->writeTimeout         = params->writeTimeout;
    object->readCallback         = params->readCallback;
    object->writeCallback        = params->writeCallback;
    object->baudRate             = params->baudRate;
    object->stopBits             = params->stopBits;
    object->dataLength           = params->dataLength;
    object->parityType           = params->parityType;
    object->readFxns =
        staticFxnTable[object->state.readMode][object->state.readDataMode];

    /* Set UART variables to defaults. */
    object->writeBuf             = NULL;
    object->readBuf              = NULL;
    object->writeCount           = 0;
    object->readCount            = 0;
    object->writeSize            = 0;
    object->readSize             = 0;

    RingBuf_construct(&object->ringBuffer, hwAttrs->ringBufPtr,
        hwAttrs->ringBufSize);
    Hwi_Params_init(&paramsUnion.hwiParams);
    paramsUnion.hwiParams.arg = (UArg)handle;
    paramsUnion.hwiParams.priority = hwAttrs->intPriority;
    Hwi_construct(&object->hwi, hwAttrs->intNum, UARTTiva_hwiIntFxn,
        &paramsUnion.hwiParams, NULL);

    Semaphore_Params_init(&paramsUnion.semParams);
    paramsUnion.semParams.mode = Semaphore_Mode_BINARY;

    /* If write mode is blocking create a semaphore and set callback. */
    if (object->state.writeMode == UART_MODE_BLOCKING) {
        Semaphore_construct(&object->writeSem, 0, &paramsUnion.semParams);
        object->writeCallback = &writeSemCallback;
    }

    /* If read mode is blocking create a semaphore and set callback. */
    if (object->state.readMode == UART_MODE_BLOCKING) {
        Semaphore_construct(&object->readSem, 0, &(paramsUnion.semParams));
        object->readCallback = &readSemCallback;
        Clock_Params_init(&paramsUnion.clockParams);
        paramsUnion.clockParams.period = 0;
        paramsUnion.clockParams.startFlag = FALSE;
        paramsUnion.clockParams.arg = (UArg)handle;
        Clock_construct(&object->timeoutClk,
                        readBlockingTimeout,
                        object->readTimeout,
                        &paramsUnion.clockParams);
    }
    else {
        object->state.drainByISR = false;
    }


    /* Enable UART and its interrupt. */
    UARTIntClear(hwAttrs->baseAddr, UART_INT_TX | UART_INT_RX | UART_INT_RT);
    UARTEnable(hwAttrs->baseAddr);

    /* Set the FIFO level to 7/8 empty and 4/8 full. */
    UARTFIFOLevelSet(hwAttrs->baseAddr, UART_FIFO_TX1_8, UART_FIFO_RX4_8);
    /* Set flow control */
    UARTFlowControlSet(hwAttrs->baseAddr, hwAttrs->flowControl);

    BIOS_getCpuFreq(&freq);
    UARTConfigSetExpClk(hwAttrs->baseAddr, freq.lo, params->baudRate,
        dataLength[params->dataLength] | stopBits[params->stopBits] |
        parityType[params->parityType]);

    UARTIntEnable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT | UART_INT_OE |
        UART_INT_BE | UART_INT_PE | UART_INT_FE);

    Log_print3(Diags_USER1, "UART:(%p) CPU freq: %d; UART baudrate to %d",
               hwAttrs->baseAddr, freq.lo, params->baudRate);

    Log_print1(Diags_USER1, "UART:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== UARTTiva_read ========
 */
int UARTTiva_read(UART_Handle handle, void *buffer, size_t size)
{
    unsigned int               key;
    UARTTiva_Object           *object = handle->object;

    key = Hwi_disable();

    if ((object->state.readMode == UART_MODE_CALLBACK) && object->readSize) {
        Hwi_restore(key);

        Log_warning1("UART:(%p) Could not read data, uart in use.",
            ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr);
        return (UART_ERROR);
    }

    /* Save the data to be read and restore interrupts. */
    object->readBuf = buffer;
    object->readSize = size;
    object->readCount = size;

    Hwi_restore(key);

    return (object->readFxns.readTaskFxn(handle));
}

/*
 *  ======== UARTTiva_readCancel ========
 */
void UARTTiva_readCancel(UART_Handle handle)
{
    unsigned int               key;
    UARTTiva_Object           *object = handle->object;

    if ((object->state.readMode != UART_MODE_CALLBACK) ||
        (object->readSize == 0)) {
        return;
    }

    key = Hwi_disable();

    object->state.drainByISR = false;
    /*
     * Indicate that what we've currently received is what we asked for so that
     * the existing logic handles the completion.
     */
    object->readSize -= object->readCount;
    object->readCount = 0;

    Hwi_restore(key);

    object->readFxns.readTaskFxn(handle);
}

/*
 *  ======== UARTTiva_readPolling ========
 */
int UARTTiva_readPolling(UART_Handle handle, void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Read characters. */
    while (size) {
        /* Grab data from the RingBuf before getting it from the RX data reg */
        UARTIntDisable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT);
        if (RingBuf_get(&object->ringBuffer, buffer) == -1) {
            *buffer = UARTCharGet(hwAttrs->baseAddr);
        }
        if (object->state.rxEnabled) {
            UARTIntEnable(hwAttrs->baseAddr, UART_INT_RX | UART_INT_RT);
        }

        Log_print2(Diags_USER2, "UART:(%p) Read character 0x%x",
                   hwAttrs->baseAddr, *buffer);
        count++;
        size--;

        if (object->state.readDataMode == UART_DATA_TEXT && *buffer == '\r') {
            /* Echo character if enabled. */
            if (object->state.readEcho) {
                UARTCharPut(hwAttrs->baseAddr, '\r');
            }
            *buffer = '\n';
        }

        /* Echo character if enabled. */
        if (object->state.readDataMode == UART_DATA_TEXT &&
                object->state.readEcho) {
            UARTCharPut(hwAttrs->baseAddr, *buffer);
        }

        /* If read return mode is newline, finish if a newline was received. */
        if (object->state.readDataMode == UART_DATA_TEXT &&
                object->state.readReturnMode == UART_RETURN_NEWLINE &&
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
 *  ======== UARTTiva_write ========
 */
int UARTTiva_write(UART_Handle handle, const void *buffer, size_t size)
{
    unsigned int               key;
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;

    if (!size) {
        return 0;
    }

    key = Hwi_disable();

    if (object->writeCount) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) Could not write data, uart in use.",
            hwAttrs->baseAddr);

        return (UART_ERROR);
    }

    /* Save the data to be written and restore interrupts. */
    object->writeBuf = buffer;
    object->writeSize = size;
    object->writeCount = size;

    Hwi_restore(key);

    if (!(UARTIntStatus(hwAttrs->baseAddr, false) & UART_INT_TX)) {
        writeData(handle);
    }
    if (object->writeCount) {
        UARTIntEnable(hwAttrs->baseAddr, UART_INT_TX);
    }

    /* If writeMode is blocking, block and get the state. */
    if (object->state.writeMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&object->writeSem),
                object->writeTimeout)) {
            /* Semaphore timed out, make the write empty and log the write. */
            UARTIntDisable(hwAttrs->baseAddr, UART_INT_TX);
            object->writeCount = 0;

            Log_print2(Diags_USER1, "UART:(%p) Write timed out, %d bytes written",
                hwAttrs->baseAddr, object->writeCount);
        }
        return (object->writeSize - object->writeCount);
    }

    return (0);
}

/*
 *  ======== UARTTiva_writeCancel ========
 */
void UARTTiva_writeCancel(UART_Handle handle)
{
    unsigned int             key;
    UARTTiva_Object         *object = handle->object;
    UARTTiva_HWAttrs const  *hwAttrs = handle->hwAttrs;
    unsigned int             written;

    key = Hwi_disable();

    /* Return if there is no write. */
    if (!object->writeCount) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent writing and restore interrupts. */
    written = object->writeCount;
    object->writeCount = 0;
    UARTIntDisable(hwAttrs->baseAddr, UART_INT_TX);

    Hwi_restore(key);

    /* Reset the write buffer so we can pass it back */
    object->writeCallback(handle, (void *)object->writeBuf,
        object->writeSize - written);

    Log_print2(Diags_USER1, "UART:(%p) Write canceled, %d bytes written",
        hwAttrs->baseAddr, object->writeSize - written);
}

/*
 *  ======== UARTTiva_writePolling ========
 */
int UARTTiva_writePolling(UART_Handle handle, const void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Write characters. */
    while (size) {
        if (object->state.writeDataMode == UART_DATA_TEXT && *buffer == '\n') {
            UARTCharPut(hwAttrs->baseAddr, '\r');
            count++;
        }
        UARTCharPut(hwAttrs->baseAddr, *buffer);

        Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
            hwAttrs->baseAddr, *buffer);
        buffer++;
        count++;
        size--;
    }

    Log_print2(Diags_USER1, "UART:(%p) Write polling finished, %d bytes written",
        hwAttrs->baseAddr, count);

    return (count);
}

/*
 *  ======== errorCallback ========
 *  Generic log function for when unexpected events occur.
 */
static void errorCallback(UART_Handle handle, uintptr_t error)
{
    if (error & UART_INT_OE) {
        Log_print1(Diags_USER2, "UART:(%p): OVERRUN ERROR",
            ((UARTTiva_HWAttrs const *)handle->hwAttrs)->baseAddr);
    }
    if (error & UART_INT_BE) {
        Log_print1(Diags_USER2, "UART:(%p): BREAK ERROR",
            ((UARTTiva_HWAttrs const *)handle->hwAttrs)->baseAddr);
    }
    if (error & UART_INT_PE) {
        Log_print1(Diags_USER2, "UART:(%p): PARITY ERROR",
            ((UARTTiva_HWAttrs const *)handle->hwAttrs)->baseAddr);
    }
    if (error & UART_INT_FE) {
        Log_print1(Diags_USER2, "UART:(%p): FRAMING ERROR",
            ((UARTTiva_HWAttrs const *)handle->hwAttrs)->baseAddr);
    }
}

/*
 *  ======== readBlockingTimeout ========
 */
static Void readBlockingTimeout(UArg arg)
{
    UARTTiva_Object *object = ((UART_Handle)arg)->object;
    object->state.bufTimeout = true;
    Semaphore_post(Semaphore_handle(&object->readSem));
}

/*
 *  ======== readIsrBinaryBlocking ========
 *  Function that is called by the ISR
 */
static bool readIsrBinaryBlocking(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    int                        readIn;

    readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    while (readIn != -1) {
        if (RingBuf_put(&object->ringBuffer, (unsigned char)readIn) == -1) {
            Log_print1(Diags_USER1, "UART:(%p) Ring buffer full!!",
                hwAttrs->baseAddr);
            return (false);
        }
        Log_print2(Diags_USER1, "UART:(%p) buffered '0x%02x'",
            hwAttrs->baseAddr, (unsigned char)readIn);

        if (object->state.callCallback) {
            object->state.callCallback = false;
            object->readCallback(handle, NULL, 0);
        }
        readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    }
    return (true);
}

/*
 *  ======== readIsrBinaryCallback ========
 */
static bool readIsrBinaryCallback(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    int                        readIn;
    bool                       ret = true;

    readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    while (readIn != -1) {
        if (RingBuf_put(&object->ringBuffer, (unsigned char)readIn) == -1) {
            Log_print1(Diags_USER1, "UART:(%p) Ring buffer full!!",
                hwAttrs->baseAddr);
            ret = false;
            break;
        }
        Log_print2(Diags_USER1, "UART:(%p) buffered '0x%02x'",
            hwAttrs->baseAddr, (unsigned char)readIn);
        readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    }

    /*
     * Check and see if a UART_read in callback mode told use to continue
     * servicing the user buffer...
     */
    if (object->state.drainByISR) {
        readTaskCallback(handle);
    }

    return (ret);
}

/*
 *  ======== readIsrTextBlocking ========
 */
static bool readIsrTextBlocking(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    int                        readIn;

    readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    while (readIn != -1) {
        if (readIn == '\r') {
            /* Echo character if enabled. */
            if (object->state.readEcho) {
                UARTCharPut(hwAttrs->baseAddr, '\r');
            }
            readIn = '\n';
        }
        if (RingBuf_put(&object->ringBuffer, (unsigned char)readIn) == -1) {
            Log_print1(Diags_USER1, "UART:(%p) Ring buffer full!!",
                hwAttrs->baseAddr);
            return (false);
        }
        Log_print2(Diags_USER1, "UART:(%p) buffered '0x%02x'",
            hwAttrs->baseAddr, (unsigned char)readIn);

        if (object->state.readEcho) {
            UARTCharPut(hwAttrs->baseAddr, (unsigned char)readIn);
        }
        if (object->state.callCallback) {
            object->state.callCallback = false;
            object->readCallback(handle, NULL, 0);
        }
        readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    }
    return (true);
}

/*
 *  ======== readIsrTextCallback ========
 */
static bool readIsrTextCallback(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    int                        readIn;
    bool                       ret = true;

    readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    while (readIn != -1) {
        if (readIn == '\r') {
            /* Echo character if enabled. */
            if (object->state.readEcho) {
                UARTCharPut(hwAttrs->baseAddr, '\r');
            }
            readIn = '\n';
        }
        if (RingBuf_put(&object->ringBuffer, (unsigned char)readIn) == -1) {
            Log_print1(Diags_USER1, "UART:(%p) Ring buffer full!!",
                hwAttrs->baseAddr);
            ret = false;
            break;
        }
        Log_print2(Diags_USER1, "UART:(%p) buffered '0x%02x'",
            hwAttrs->baseAddr, (unsigned char)readIn);

        if (object->state.readEcho) {
            UARTCharPut(hwAttrs->baseAddr, (unsigned char)readIn);
        }
        readIn = UARTCharGetNonBlocking(hwAttrs->baseAddr);
    }

    /*
     * Check and see if a UART_read in callback mode told use to continue
     * servicing the user buffer...
     */
    if (object->state.drainByISR) {
        readTaskCallback(handle);
    }

    return (ret);
}

/*
 *  ======== readSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void readSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTiva_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&object->readSem));
}

/*
 *  ======== readTaskBlocking ========
 */
static int readTaskBlocking(UART_Handle handle)
{
    UARTTiva_Object         *object = handle->object;
    unsigned char            readIn;
    unsigned char           *buffer = object->readBuf;

    object->state.bufTimeout = false;
    Semaphore_pend(Semaphore_handle(&object->readSem), BIOS_NO_WAIT);
    Clock_start(Clock_handle(&object->timeoutClk));

    while (object->readCount) {
        if (RingBuf_get(&object->ringBuffer, &readIn) < 0) {
            object->state.callCallback = true;
            Semaphore_pend(Semaphore_handle(&object->readSem),
                BIOS_WAIT_FOREVER);
            if (object->state.bufTimeout == true) {
                break;
            }
            RingBuf_get(&object->ringBuffer, &readIn);
        }

        Log_print2(Diags_USER1, "UART:(%p) read '0x%02x'",
            ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
            (unsigned char)readIn);

        *buffer = readIn;
        buffer++;
        /* In blocking mode, readCount doesn't not need a lock */
        object->readCount--;

        if (object->state.readDataMode == UART_DATA_TEXT &&
                object->state.readReturnMode == UART_RETURN_NEWLINE &&
                readIn == '\n') {
            break;
        }
    }

    Clock_stop(Clock_handle(&object->timeoutClk));
    return (object->readSize - object->readCount);
}

/*
 *  ======== readTaskCallback ========
 *  This function is called the first time by the UART_read task and tries to
 *  get all the data it can get from the ringBuffer. If it finished, it will
 *  perform the user supplied callback. If it didn't finish, the ISR must handle
 *  the remaining data. By setting the drainByISR flag, the UART_read function
 *  handed over the responsibility to get the remaining data to the ISR.
 */
static int readTaskCallback(UART_Handle handle)
{
    unsigned int               key;
    UARTTiva_Object           *object = handle->object;
    unsigned char              readIn;
    unsigned char             *bufferEnd;
    bool                       makeCallback = false;

    object->state.drainByISR = false;
    bufferEnd = (unsigned char*)object->readBuf + object->readSize;

    while (object->readCount) {
        if (RingBuf_get(&object->ringBuffer, &readIn) < 0) {
            break;
        }

        Log_print2(Diags_USER1, "UART:(%p) read '0x%02x'",
            ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
            (unsigned char)readIn);

        *(unsigned char *)(bufferEnd -
            object->readCount * sizeof(unsigned char)) = readIn;

        key = Hwi_disable();

        object->readCount--;

        Hwi_restore(key);

        if ((object->state.readDataMode == UART_DATA_TEXT) &&
            (object->state.readReturnMode == UART_RETURN_NEWLINE) &&
            (readIn == '\n')) {
            makeCallback = true;
            break;
        }
    }

    if (!object->readCount || makeCallback) {
        /* Nested UART_reads from within callbacks are NOT supported!!! */
        object->readCallback(handle, object->readBuf,
            object->readSize - object->readCount);
        object->readSize = 0;
    }
    else {
        object->state.drainByISR = true;
    }

    return (0);
}

/*
 *  ======== writeData ========
 */
static void writeData(UART_Handle handle)
{
    UARTTiva_Object           *object = handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned char             *writeOffset;

    writeOffset = (unsigned char *)object->writeBuf +
        object->writeSize * sizeof(unsigned char);
    while (object->writeCount) {
        if (!UARTCharPutNonBlocking(hwAttrs->baseAddr,
                *(writeOffset - object->writeCount))) {
            /* TX FIFO is FULL */
            break;
        }
        if ((object->state.writeDataMode == UART_DATA_TEXT) &&
            (*(writeOffset - object->writeCount) == '\n')) {
            UARTCharPut(hwAttrs->baseAddr, '\r');
        }
        object->writeCount--;
    }

    if (!object->writeCount) {
        UARTIntDisable(hwAttrs->baseAddr, UART_INT_TX);

        object->writeCallback(handle, (void *)object->writeBuf,
            object->writeSize);

        Log_print2(Diags_USER1, "UART:(%p) Write finished, %d bytes written",
            hwAttrs->baseAddr, object->writeSize - object->writeCount);
    }
}

/*
 *  ======== writeSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void writeSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTiva_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&object->writeSem));
}
