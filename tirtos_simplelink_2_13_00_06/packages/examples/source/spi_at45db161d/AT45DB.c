/*
 * Copyright (c) 2014, Texas Instruments Incorporated
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

/*
 *  ======== AT45DB.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

#include "AT45DB.h"
#include "Board.h"

#define NUM_OBJ 1
#define MAX_PAGE_SIZE 528
#define PIN_LOW  (0)
#define PIN_HIGH ~(0)

/*  This example uses Board_SPI_AT45DB and Board_SPI_CS.
 *  Check Board.h for definitions
 */
static AT45DB_Object at45dbObjects[NUM_OBJ] =
                  {{NULL, Board_SPI_AT45DB, Board_SPI_CS, 0}};

/*
 *  ======== AT45DB_close ========
 */
void AT45DB_close(AT45DB_Handle handle)
{
    Assert_isTrue((handle != NULL), NULL);

    SPI_close(handle->spiHandle);
}

/*
 *  ======== AT45DB_open ========
 */
AT45DB_Handle AT45DB_open(unsigned int index)
{
    Assert_isTrue((index < NUM_OBJ), NULL);

    AT45DB_Handle at45dbHandle;
    unsigned int key;
    at45dbHandle = &(at45dbObjects[index]);

    /* Determine if the device index was already opened */
    key = Hwi_disable();
    if(at45dbHandle->spiHandle) {
        Hwi_restore(key);
        return NULL;
    }
    Hwi_restore(key);

    /* Open SPI driver for AT45DB */
    at45dbHandle->spiHandle = SPI_open(at45dbHandle->boardSPI, NULL);

    /* Read status register to retrieve device page size('0'-528 or '1'-512)*/
    at45dbHandle->pageSize = (AT45DB_PageSize)(0x01 & AT45DB_readStatusRegister(at45dbHandle));

    Log_print1(Diags_USER1,"AT45DB opened", NULL);

    return at45dbHandle;
}

/*
 *  ======== AT45DB_bufferWrite ========
 */
bool AT45DB_bufferWrite(AT45DB_Handle handle, AT45DB_Transaction* transaction) {

    SPI_Transaction opcodeTransaction;
    SPI_Transaction dataTransaction;
    uint8_t txBuffer[4];
    uint8_t rxBuffer[4];
    uint8_t dummyBuffer[MAX_PAGE_SIZE];
    uint8_t ready = 0;

    /* Check if device is ready */
    while(!ready) {
        ready = 0x80 & AT45DB_readStatusRegister(handle);
    }

    /* Initialize txBuffer for with opcode and address data */
    txBuffer[0] = 0x84;
    switch(handle->pageSize) {
        case PAGE_SIZE_528:
            txBuffer[2] = (uint8_t)(0x0300 & transaction->address.byte);
            txBuffer[3] = (uint8_t)(0x00FF & transaction->address.byte);
            break;
        case PAGE_SIZE_512:
            txBuffer[2] = (uint8_t)(0x0100 & transaction->address.byte);
            txBuffer[3] = (uint8_t)(0x00FF & transaction->address.byte);
    }

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 4;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Initialize data transaction structure */
    dataTransaction.count = transaction->data_size;
    dataTransaction.txBuf = (Ptr)(transaction->data);
    dataTransaction.rxBuf = (Ptr)dummyBuffer;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Initiate SPI transfer of data */
    if(!SPI_transfer(handle->spiHandle, &dataTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return true;

}

/*
 *  ======== AT45DB_bufferRead ========
 */
bool AT45DB_bufferRead(AT45DB_Handle handle, AT45DB_Transaction* transaction) {

    SPI_Transaction opcodeTransaction;
    SPI_Transaction dataTransaction;
    uint8_t txBuffer[4];
    uint8_t rxBuffer[4];
    uint8_t dummyBuffer[MAX_PAGE_SIZE];
    uint8_t ready = 0;

    /* Check if device is ready */
    while(!ready) {
        ready = 0x80 & AT45DB_readStatusRegister(handle);
    }

    /* Initialize txBuffer for with opcode and address data */
    txBuffer[0] = 0xD1;
    switch(handle->pageSize) {
        case PAGE_SIZE_528:
            txBuffer[2] = (uint8_t)(0x0300 & transaction->address.byte);
            txBuffer[3] = (uint8_t)(0x00FF & transaction->address.byte);
            break;
        case PAGE_SIZE_512:
            txBuffer[2] = (uint8_t)(0x0100 & transaction->address.byte);
            txBuffer[3] = (uint8_t)(0x00FF & transaction->address.byte);
    }

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 4;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Initialize data transaction structure */
    dataTransaction.count = transaction->data_size;
    dataTransaction.rxBuf = (Ptr)(transaction->data);
    dataTransaction.txBuf = (Ptr)dummyBuffer;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Initiate SPI transfer of data */
    if(!SPI_transfer(handle->spiHandle, &dataTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return true;
}

/*
 *  ======== AT45DB_bufferToPage ========
 */
bool AT45DB_bufferToPage(AT45DB_Handle handle, unsigned int page) {

    SPI_Transaction opcodeTransaction;
    uint8_t txBuffer[4];
    uint8_t rxBuffer[4];
    uint8_t ready = 0;

    /* Check if device is ready */
    while(!ready) {
        ready = 0x80 & AT45DB_readStatusRegister(handle);
    }

    /* Initialize txBuffer for with opcode and address data */
    txBuffer[0] = 0x83;
    switch(handle->pageSize) {
        case PAGE_SIZE_528:
            txBuffer[1] = (uint8_t) (page >> 6);
            txBuffer[2] = (uint8_t) (page << 2);
            break;
        case PAGE_SIZE_512:
            txBuffer[1] = (uint8_t) (page >> 7);
            txBuffer[2] = (uint8_t) (page << 1);
    }

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 4;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return true;
}

/*
 *  ======== AT45DB_readMainMemoryPage ========
 */
bool AT45DB_readMainMemoryPage(AT45DB_Handle handle, AT45DB_Transaction* transaction) {

    SPI_Transaction opcodeTransaction;
    SPI_Transaction dataTransaction;
    uint8_t txBuffer[8];
    uint8_t rxBuffer[8];
    uint8_t dummyBuffer[MAX_PAGE_SIZE];
    uint8_t ready = 0;

    /* Check if device is ready */
    while(!ready) {
        ready = 0x80 & AT45DB_readStatusRegister(handle);
    }

    /* Initialize txBuffer for with opcode and address data */
    txBuffer[0] = 0xD2;
    switch(handle->pageSize) {
        case PAGE_SIZE_528:
            txBuffer[1] = (uint8_t) ((transaction->address.page) >> 6);
            txBuffer[2] = (uint8_t) (((transaction->address.page) << 2) | ((transaction->address.byte) >> 8));
            txBuffer[3] = 0xFF & (transaction->address.byte);
            break;
        case PAGE_SIZE_512:
            txBuffer[1] = (uint8_t) ((transaction->address.page) >> 7);
            txBuffer[2] = (uint8_t) (((transaction->address.page) << 1) | ((transaction->address.byte) >> 8));
            txBuffer[3] = 0xFF & (transaction->address.byte);
    }

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 8;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Initialize data transaction structure */
    dataTransaction.count = transaction->data_size;
    dataTransaction.txBuf = (Ptr)dummyBuffer;
    dataTransaction.rxBuf = (Ptr)(transaction->data);

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Initiate SPI transfer of data */
    if(!SPI_transfer(handle->spiHandle, &dataTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return true;
}

/*
 *  ======== AT45DB_readStatusRegister ========
 */
uint8_t AT45DB_readStatusRegister(AT45DB_Handle handle) {

    SPI_Transaction opcodeTransaction;
    SPI_Transaction dataTransaction;
    uint8_t txBuffer = 0xD7;
    uint8_t rxBuffer;
    uint8_t dummyBuffer;
    uint8_t status;

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 1;
    opcodeTransaction.txBuf = (Ptr)&txBuffer;
    opcodeTransaction.rxBuf = (Ptr)&rxBuffer;

    /* Initialize data transaction structure */
    dataTransaction.count = 1;
    dataTransaction.txBuf = (Ptr)&dummyBuffer;
    dataTransaction.rxBuf = (Ptr)&status;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
    }
    /* Initiate SPI transfer of data */
    if(!SPI_transfer(handle->spiHandle, &dataTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
     }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return status;
}

/*
 *  ======== AT45DB_config ========
 */
void AT45DB_config(AT45DB_Handle handle){

    SPI_Transaction opcodeTransaction;
    uint8_t txBuffer[4] = {0x3D, 0x2A, 0x80, 0xA6};
    uint8_t rxBuffer[4];

    /* Initialize opcode transaction structure */
    opcodeTransaction.count = 4;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {

        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);
}

/*
 *  ======== AT45DB_pageErase ========
 */
bool AT45DB_pageErase(AT45DB_Handle handle, unsigned int page)
{
    SPI_Transaction opcodeTransaction;
    uint8_t txBuffer[4];
    uint8_t rxBuffer[4];
    uint8_t ready = 0;

    /* Check if device is ready */
    while(!ready) {
        ready = 0x80 & AT45DB_readStatusRegister(handle);
    }

    /* Initialize txBuffer for with opcode and address data */
    txBuffer[0] = 0x81;
    switch(handle->pageSize) {
        case PAGE_SIZE_528:
            txBuffer[1] = (uint8_t) (page >> 6);
            txBuffer[2] = (uint8_t) (page << 2);
            break;
        case PAGE_SIZE_512:
            txBuffer[1] = (uint8_t) (page >> 7);
            txBuffer[2] = (uint8_t) (page << 1);
    }

    /* Initialize master SPI transaction structure */
    opcodeTransaction.count = 4;
    opcodeTransaction.txBuf = (Ptr)txBuffer;
    opcodeTransaction.rxBuf = (Ptr)rxBuffer;

    /* Hold SPI chip select low */
    GPIO_write(handle->boardCS, PIN_LOW);

    /* Initiate SPI transfer of opcode */
    if(!SPI_transfer(handle->spiHandle, &opcodeTransaction)) {
        Log_print1(Diags_USER1, "Unsuccessful SPI transfer", NULL);
        return false;
    }
    /* Release SPI chip select */
    GPIO_write(handle->boardCS, PIN_HIGH);

    return true;
}

/*
 *  ======== AT45DB_pageWriteThroughBuffer ========
 */
bool AT45DB_pageWriteThroughBuffer(AT45DB_Handle handle, AT45DB_Transaction* transaction)
{
    /* Write to buffer */
    if(!AT45DB_bufferWrite(handle, transaction) ) {
        Log_print1(Diags_USER1, "UnSuccessful SPI transfer", NULL);
        return false;
    }
    /* Copy from buffer to main memory page */
    if(!AT45DB_bufferToPage(handle, transaction->address.page) ) {
        Log_print1(Diags_USER1, "UnSuccessful SPI transfer", NULL);
        return false;
    }
    return true;
}
