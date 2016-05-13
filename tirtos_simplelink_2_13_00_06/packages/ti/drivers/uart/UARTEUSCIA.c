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

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>

#include <ti/drivers/uart/UARTEUSCIA.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/family/msp430/ClockFreqs.h>

/* driverlib header files */
#include <eusci_a_uart.h>

extern const UART_Config UART_config[];

/* Prototypes */
void         UARTEUSCIA_close(UART_Handle handle);
int          UARTEUSCIA_control(UART_Handle handle, unsigned int cmd, void *arg);
void         UARTEUSCIA_init(UART_Handle handle);
UART_Handle  UARTEUSCIA_open(UART_Handle handle, UART_Params *params);
int          UARTEUSCIA_read(UART_Handle handle, void *buffer, size_t size);
void         UARTEUSCIA_readCancel(UART_Handle handle);
int          UARTEUSCIA_readPolling(UART_Handle handle, void *buffer,
                                    size_t size);
int          UARTEUSCIA_write(UART_Handle handle, const void *buffer,
                              size_t size);
void         UARTEUSCIA_writeCancel(UART_Handle handle);
int          UARTEUSCIA_writePolling(UART_Handle handle,
                                     const void *buffer, size_t size);

/* UARTEUSCIA function table for UARTEUSCIA implementation */
const UART_FxnTable UARTEUSCIA_fxnTable = {
    UARTEUSCIA_close,
    UARTEUSCIA_control,
    UARTEUSCIA_init,
    UARTEUSCIA_open,
    UARTEUSCIA_read,
    UARTEUSCIA_readPolling,
    UARTEUSCIA_readCancel,
    UARTEUSCIA_write,
    UARTEUSCIA_writePolling,
    UARTEUSCIA_writeCancel,
};

static const uint32_t stopBits[] = {
    EUSCI_A_UART_ONE_STOP_BIT,  /* UART_STOP_ONE */
    EUSCI_A_UART_TWO_STOP_BITS  /* UART_STOP_TWO */
};

static const uint32_t parityType[] = {
    EUSCI_A_UART_NO_PARITY,     /* UART_PAR_NONE */
    EUSCI_A_UART_EVEN_PARITY,   /* UART_PAR_EVEN */
    EUSCI_A_UART_ODD_PARITY,    /* UART_PAR_ODD */
    0,                          /* UART_PAR_ZERO */
    1                           /* UART_PAR_ONE */
};

/* Default UART params */
extern const UART_Params UART_defaultParams;

/*
 *  ======== findBaudDividers ========
 *  This function searchs a given array of different baudrate configurations to
 *  find the first compatible match given the desired buadrate and the currently
 *  configured clock input frequency.
 *
 *  It return the index in if the UARTEUSCIA_BaudrateConfig table if a match was
 *  found, else it will return -1
 */
static int32_t findBaudDividerIndex(UARTEUSCIA_BaudrateConfig const *table,
                                    uint32_t tableSize,
                                    uint32_t baudrate,
                                    uint32_t clockFreq)
{
    int32_t  index;

    Assert_isTrue((tableSize != 0) && (table != NULL), NULL);

    for (index = 0; index < tableSize; index++) {
        if ((table[index].outputBaudrate == baudrate) &&
            (table[index].inputClockFreq == clockFreq)) {
            return (index);
        }
    }
    return (-1);
}

/*
 *  ======== writeSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void writeSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTEUSCIA_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&(object->writeSem)));
}

/*
 *  ======== readSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void readSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTEUSCIA_Object *object = handle->object;

    Semaphore_post(Semaphore_handle(&(object->readSem)));
}

/*
 *  ======== writeData ========
 *  Write and process data to the UARTEUSCIA.
 */
static void writeData(UARTEUSCIA_Object        *object,
                      UARTEUSCIA_HWAttrs const *hwAttrs)
{
    /* If mode is TEXT process the characters */
    if (object->writeDataMode == UART_DATA_TEXT) {
        if (object->writeCR) {
            EUSCI_A_UART_transmitData(hwAttrs->baseAddr, '\r');
            Log_print2(Diags_USER2, "UART:(%p) Wrote 0x%x",
                                     hwAttrs->baseAddr, '\r');
            object->writeSize--;
            object->writeCount++;
            object->writeCR = false;
        }
        else {
            /* Add a return if next character is a newline. */
           if (*(unsigned char *)object->writeBuf == '\n') {
               object->writeSize++;
               object->writeCR = true;
           }
           EUSCI_A_UART_transmitData(hwAttrs->baseAddr,
                                   *(unsigned char *)object->writeBuf);
           Log_print2(Diags_USER2, "UART:(%p) Wrote 0x%x",
                                    hwAttrs->baseAddr,
                                  *(unsigned char *)object->writeBuf);
           object->writeBuf = (unsigned char *)object->writeBuf + 1;
           object->writeSize--;
           object->writeCount++;
        }
    }
    else {
        EUSCI_A_UART_transmitData(hwAttrs->baseAddr,
                *(unsigned char *)object->writeBuf);
        Log_print2(Diags_USER2, "UART:(%p) Wrote 0x%x",
                                 hwAttrs->baseAddr,
                               *(unsigned char *)object->writeBuf);
        object->writeBuf = (unsigned char *)object->writeBuf + 1;
        object->writeSize--;
        object->writeCount++;
    }
}

/*
 *  ======== readData ========
 *  Read and process data from the UARTEUSCIA.
 */
static void readData(UARTEUSCIA_Object         *object,
                     UARTEUSCIA_HWAttrs const  *hwAttrs)
{
    uint8_t readIn;

    /* Receive char */
    readIn = EUSCI_A_UART_receiveData(hwAttrs->baseAddr);

    /* If data mode is set to TEXT replace return with a newline. */
    if (object->readDataMode == UART_DATA_TEXT) {
        if (readIn == '\r') {
            /* Echo character if enabled. */
            if (object->readEcho) {
                /* Wait until TX is ready */
                while (!EUSCI_A_UART_getInterruptStatus(hwAttrs->baseAddr,
                            EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
                EUSCI_A_UART_transmitData(hwAttrs->baseAddr, '\r');
            }
            readIn = '\n';
        }
    }

    /* Echo character if enabled. */
    if (object->readEcho) {
        /* Wait until TX is ready */
        while (!EUSCI_A_UART_getInterruptStatus(hwAttrs->baseAddr,
                    EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
        EUSCI_A_UART_transmitData(hwAttrs->baseAddr, readIn);
    }

    Log_print2(Diags_USER2, "UART:(%p) Read 0x%x",
                             hwAttrs->baseAddr, readIn);
    *(unsigned char *)object->readBuf = readIn;
    object->readBuf = (unsigned char *)object->readBuf + 1;
    object->readCount++;
    object->readSize--;

    /* If read return mode is newline, finish if a newline was received. */
    if (object->readReturnMode == UART_RETURN_NEWLINE && readIn == '\n') {
        Log_print1(Diags_USER2, "UART:(%p) Newline received, "
                                "returning",
                                 hwAttrs->baseAddr);
        object->readSize = 0;
    }
}

/*
 *  ======== UARTEUSCIA_hwiIntFxn ========
 *  Hwi function that processes UARTEUSCIA interrupts.
 *
 *  writeData and readData are in-lined functions that will read/write data
 *  with processing if enabled.  When the read or write is finished they will
 *  post the semaphore or make the callback and log the transfer.
 *
 *  @param(UARTEUSCIAObject)       The UARTEUSCIA_Object for this Hwi.
 */
void UARTEUSCIA_hwiIntFxn(UArg index)
{
    uint8_t                      status;
    UARTEUSCIA_Object           *object = (&UART_config[index])->object;
    UARTEUSCIA_HWAttrs const    *hwAttrs = (&UART_config[index])->hwAttrs;

    /* Clear interrupts */
    status = EUSCI_A_UART_getInterruptStatus(hwAttrs->baseAddr,
             EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG |
             EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG);

    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) {
        /* Read data if characters are available. */
        if (object->readSize) {
            readData(object, hwAttrs);
            if (!object->readSize) {
                /* Reset the read buffer so we can pass it back */
                object->readBuf = (unsigned char *)object->readBuf - object->readCount;
                object->readCallback((UART_Handle)&UART_config[index], object->readBuf,
                                            object->readCount);
                Log_print2(Diags_USER1, "UART:(%p) Read finished, "
                                        "%d bytes read",
                                         hwAttrs->baseAddr, object->readCount);
                /* Disable RX interrupt until we do a new read */
                EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
                        EUSCI_A_UART_RECEIVE_INTERRUPT);
            }
        }
    }

    if (status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) {
        /* Write if there are characters to be written. */
        if (object->writeSize > 0) {
            writeData(object, hwAttrs);
            if (object->writeSize == 0) {
                /* Reset the write buffer so we can pass it back */
                object->writeBuf = (unsigned char *)object->writeBuf - object->writeCount;
                object->writeCallback((UART_Handle)&UART_config[index],
                                             (uint8_t*)object->writeBuf,
                                             object->writeCount);
                Log_print2(Diags_USER1, "UART:(%p) Write finished, "
                                        "%d bytes written",
                                         hwAttrs->baseAddr, object->writeCount);

                /* Disable TX interrupt until we do a new write */
                EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
                        EUSCI_A_UART_TRANSMIT_INTERRUPT);
            }
        }
    }
}

/*
 *  ======== UARTEUSCIA_init ========
 */
void UARTEUSCIA_init(UART_Handle handle)
{
    UARTEUSCIA_Object *object = handle->object;

    object->isOpen = false;
}

/*
 *  ======== UARTEUSCIA_open ========
 */
UART_Handle UARTEUSCIA_open(UART_Handle handle, UART_Params *params)
{
    unsigned int                key;
    uint32_t                    clockFreq;
    int32_t                     baudrateIndex;
    EUSCI_A_UART_initParam      eusciUartParams;
    Semaphore_Params            semParams;
    UARTEUSCIA_Object          *object = handle->object;
    UARTEUSCIA_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (UART_Params *) &UART_defaultParams;
    }

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

    /* Get the UART clock input frequency */
    switch (hwAttrs->clockSource) {
        case EUSCI_A_UART_CLOCKSOURCE_ACLK:
            clockFreq = ClockFreqs_getFrequency(ClockFreqs_Clock_ACLK);
            Log_print1(Diags_USER1, "ClockFreqs_getFrequency ACLK: %d", clockFreq);
            break;

        case EUSCI_A_UART_CLOCKSOURCE_SMCLK:
            clockFreq = ClockFreqs_getFrequency(ClockFreqs_Clock_SMCLK);
            Log_print1(Diags_USER1, "ClockFreqs_getFrequency SMCLK: %d", clockFreq);
            break;


        default:
            Log_error1("UART:(%p) Error determining clock source",
                        hwAttrs->baseAddr);
            return (NULL);
    }

    /* Find a valid baudrate configuration for EUSCI_A_UART_initAdvance() */
    baudrateIndex = findBaudDividerIndex(hwAttrs->baudrateLUT,
                                         hwAttrs->numBaudrateEntries,
                                         params->baudRate,
                                         clockFreq);
    if (baudrateIndex == -1) {
        Log_error3("UART:(%p) unable to find a valid buadrate %d "
                   "configuration at clock input clock freq %d",
                   hwAttrs->baseAddr,
                   params->baudRate,
                   clockFreq);
        return (NULL);
    }

    /* Check that a callback is set */
    Assert_isTrue((params->readMode != UART_MODE_CALLBACK) ||
                  (params->readCallback != NULL), NULL);
    Assert_isTrue((params->writeMode != UART_MODE_CALLBACK) ||
                  (params->writeCallback != NULL), NULL);

    /* Disable preemption while checking if the UART is open. */
    key = Hwi_disable();

    /* Check if the UARTEUSCIA is open already with the base addr. */
    if (object->isOpen) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }

    /* UART is now open */
    object->isOpen = true;
    Hwi_restore(key);

    /* Save paramters */
    object->writeBuf = NULL;
    object->readBuf = NULL;
    object->writeCount = 0;
    object->readCount = 0;
    object->writeSize = 0;
    object->readSize = 0;
    object->writeCR = false;

    /* If write mode is blocking create a semaphore and set callback. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        Semaphore_Params_init(&semParams);
        semParams.mode = Semaphore_Mode_BINARY;
        Semaphore_construct(&(object->writeSem), 0, &semParams);
        object->writeCallback = &writeSemCallback;
    }

    /* If read mode is blocking create a semaphore and set callback. */
    if (object->readMode == UART_MODE_BLOCKING) {
        Semaphore_Params_init(&semParams);
        semParams.mode = Semaphore_Mode_BINARY;
        Semaphore_construct(&(object->readSem), 0, &semParams);
        object->readCallback = &readSemCallback;
    }

    eusciUartParams.selectClockSource = hwAttrs->clockSource;
    eusciUartParams.clockPrescalar = hwAttrs->baudrateLUT[baudrateIndex].prescalar;
    eusciUartParams.firstModReg = hwAttrs->baudrateLUT[baudrateIndex].hwRegUCBRFx;
    eusciUartParams.secondModReg = hwAttrs->baudrateLUT[baudrateIndex].hwRegUCBRSx;
    eusciUartParams.parity = parityType[params->parityType];
    eusciUartParams.msborLsbFirst = hwAttrs->bitOrder;
    eusciUartParams.numberofStopBits = stopBits[params->stopBits];
    eusciUartParams.uartMode = EUSCI_A_UART_MODE;
    eusciUartParams.overSampling = hwAttrs->baudrateLUT[baudrateIndex].sampling;
    if (STATUS_FAIL == EUSCI_A_UART_init(hwAttrs->baseAddr, &eusciUartParams)) {
        Log_error3("UART:(%p) initAdvance() failure "
                   "CPU freq: %d; "
                   "UART baudrate to %d",
                    hwAttrs->baseAddr,
                    clockFreq,
                    params->baudRate);
        return (NULL);
    }

    /* Enable UART and disable its interrupts. */
    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_TRANSMIT_INTERRUPT);
    EUSCI_A_UART_enable(hwAttrs->baseAddr);

    Log_print1(Diags_USER1, "UART:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== UARTEUSCIA_close ========
 */
void UARTEUSCIA_close(UART_Handle handle)
{
    UARTEUSCIA_Object           *object = handle->object;
    UARTEUSCIA_HWAttrs const    *hwAttrs = handle->hwAttrs;

    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_TRANSMIT_INTERRUPT);
    EUSCI_A_UART_disable(hwAttrs->baseAddr);

    if (object->writeMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&(object->writeSem));
    }

    if (object->readMode == UART_MODE_BLOCKING) {
        Semaphore_destruct(&(object->readSem));
    }

    object->isOpen = false;

    Log_print1(Diags_USER1, "UART:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== UARTEUSCIA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int UARTEUSCIA_control(UART_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (UART_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== UARTEUSCIA_write ========
 */
int UARTEUSCIA_write(UART_Handle handle, const void *buffer, size_t size)
{
    unsigned int                key;
    UARTEUSCIA_Object          *object = handle->object;
    UARTEUSCIA_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* Check that there is data to write */
    Assert_isTrue(size != 0, NULL);

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->writeSize != 0) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) Could not write data, uart in use.",
                      hwAttrs->baseAddr);
        return (UART_ERROR);
    }

    /* Save the data to be written and restore interrupts. */
    object->writeBuf = buffer;
    object->writeSize = size;
    object->writeCount = 0;

    Hwi_restore(key);

    /* Enabling TX interrupt will trigger the Hwi which will handle the write */
    EUSCI_A_UART_enableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_TRANSMIT_INTERRUPT);

    /* If writeMode is blocking, block and get the status. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&object->writeSem),
                    object->writeTimeout)) {
            /* Semaphore timed out, make the write empty and log the write. */
            EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
                    EUSCI_A_UART_TRANSMIT_INTERRUPT);
            object->writeSize = 0;

            Log_print2(Diags_USER1, "UART:(%p) Write timed out, "
                                    "%d bytes written",
                                     hwAttrs->baseAddr,
                                     object->writeCount);

        }
        return (object->writeCount);
    }

    /* UART_MODE_CALLBACK returns 0 */
    return (0);
}

/*
 *  ======== UARTEUSCIA_writePolling ========
 */
int UARTEUSCIA_writePolling(UART_Handle handle, const void *buffer, size_t size)
{
    UARTEUSCIA_Object           *object = handle->object;
    UARTEUSCIA_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Save the data to be written and restore interrupts. */
    object->writeBuf = buffer;
    object->writeSize = size;
    object->writeCount = 0;

    while (object->writeSize) {
        /* Wait until we can TX a byte */
        while (!EUSCI_A_UART_getInterruptStatus(hwAttrs->baseAddr,
                    EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG));
        writeData(object, hwAttrs);
    }

    Log_print2(Diags_USER1, "UART:(%p) Write polling finished, "
                            "%d bytes written",
                             hwAttrs->baseAddr,
                             object->writeCount);


    return (object->writeCount);
}

/*
 *  ======== UARTEUSCIA_writeCancel ========
 */
void UARTEUSCIA_writeCancel(UART_Handle handle)
{
    unsigned int                key;
    UARTEUSCIA_Object          *object = handle->object;
    UARTEUSCIA_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* Disable interrupts to avoid writing data while changing state. */
    key = Hwi_disable();

    /* Return if there is no write. */
    if (!object->writeSize) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent writing and restore interrupts. */
    object->writeSize = 0;
    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_TRANSMIT_INTERRUPT);
    Hwi_restore(key);

    /* Reset the write buffer so we can pass it back */
    object->writeBuf = (unsigned char *)object->writeBuf - object->writeCount;
    object->writeCallback(handle, (uint8_t*)object->writeBuf,
                                 object->writeCount);

    Log_print2(Diags_USER1, "UART:(%p) Write canceled, "
                            "%d bytes written",
                             hwAttrs->baseAddr,
                             object->writeCount);
}

/*
 *  ======== UARTEUSCIA_read ========
 */
int UARTEUSCIA_read(UART_Handle handle, void *buffer, size_t size)
{
    unsigned int                key;
    UARTEUSCIA_Object          *object = handle->object;
    UARTEUSCIA_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->readSize) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) Could not read data, uart in use.",
                      hwAttrs->baseAddr);
        return (UART_ERROR);
    }

    /* Save the data to be read and restore interrupts. */
    object->readBuf = buffer;
    object->readSize = size;
    object->readCount = 0;

    Hwi_restore(key);

    /* Enabling RX interrupt will trigger the Hwi which will handle the read */
    EUSCI_A_UART_enableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_RECEIVE_INTERRUPT);

    /* If readMode is blocking, block and get the status. */
    if (object->readMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&object->readSem),
                    object->readTimeout)) {
            /* Semaphore timed out, make the read empty and log the read. */
            EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
                    EUSCI_A_UART_RECEIVE_INTERRUPT);
            object->readSize = 0;
            Log_print2(Diags_USER1, "UART:(%p) Read timed out, "
                                    "%d bytes read",
                                     hwAttrs->baseAddr,
                                     object->readCount);
        }
        return (object->readCount);
    }

    /* UART_MODE_CALLBACK returns 0 */
    return (0);
}

/*
 *  ======== UARTEUSCIA_readPolling ========
 */
int UARTEUSCIA_readPolling(UART_Handle handle, void *buffer, size_t size)
{
    UARTEUSCIA_Object           *object = handle->object;
    UARTEUSCIA_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Save the data to be read and restore interrupts. */
    object->readBuf = buffer;
    object->readSize = size;
    object->readCount = 0;

    while (object->readSize) {
        /* Wait until we have RX a byte */
        while (!EUSCI_A_UART_getInterruptStatus(hwAttrs->baseAddr,
                    EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG));
        readData(object, hwAttrs);
     }

    Log_print2(Diags_USER1, "UART:(%p) Read polling finished, "
                            "%d bytes read",
                             hwAttrs->baseAddr,
                             object->readCount);

    return (object->readCount);
}

/*
 *  ======== UARTEUSCIA_readCancel ========
 */
void UARTEUSCIA_readCancel(UART_Handle handle)
{
    unsigned int                key;
    UARTEUSCIA_Object          *object = handle->object;
    UARTEUSCIA_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* Disable interrupts to avoid reading data while changing state. */
    key = Hwi_disable();

    /* Return if there is no read. */
    if (!object->readSize) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent reading and restore interrupts. */
    object->readSize = 0;
    EUSCI_A_UART_disableInterrupt(hwAttrs->baseAddr,
            EUSCI_A_UART_RECEIVE_INTERRUPT);
    Hwi_restore(key);

    /* Reset the read buffer so we can pass it back */
    object->readBuf = (unsigned char *)object->readBuf - object->readCount;
    object->readCallback(handle, object->readBuf, object->readCount);

    Log_print2(Diags_USER1, "UART:(%p) Read canceled, "
                            "%d bytes read",
                             hwAttrs->baseAddr,
                             object->readCount);
}
