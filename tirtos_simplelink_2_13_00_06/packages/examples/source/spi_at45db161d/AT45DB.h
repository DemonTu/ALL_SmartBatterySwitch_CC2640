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
/** ============================================================================
 *  @file       AT45DB.h
 *
 *  @brief      Driver for AT45DB161D SPI Nand Flash
 *
 *  The AT45DB header file should be included in an application as follows:
 *  @code
 *  #include <AT45DB.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __AT45DB_H
#define __AT45DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* TI-RTOS Header files */
#include <ti/drivers/SPI.h>

#define AT45DB161D_NUMBER_OF_PAGES    4096

/* Typedefs */
typedef enum PAGE_SIZES {
    PAGE_SIZE_528 = 0,
    PAGE_SIZE_512
}AT45DB_PageSize;

typedef struct AT45DB_Object {
    SPI_Handle      spiHandle;/* Handle for SPI object */
    unsigned int    boardSPI; /* Board SPI in Board.h */
    unsigned int    boardCS;  /* Board chip select in Board.h */
    AT45DB_PageSize pageSize; /* Indicates page size (512 or 528 bytes) */
}AT45DB_Object;

typedef struct AT45DB_Address {
    unsigned int page;      /* Page address */
    unsigned int byte;      /* Byte offset */
}AT45DB_Address;

typedef struct AT45DB_Transaction {
    uint8_t     *data;      /* Transaction data buffer */
    size_t       data_size; /* Size of transaction data in bytes */
    AT45DB_Address address; /* Address object for transaction */
}AT45DB_Transaction;

/*!
 *  @brief A handle that is returned from a AT45DB_open() call.
 */
typedef AT45DB_Object *AT45DB_Handle;

/*!
 *  @brief  Read one of the SRAM data buffers (Buffer 1)
 *
 *  This function reads one of the SRAM data buffers
 *
 *  @param  handle       A AT45DB_Handle
 *  @param  transaction  A AT45DB_Transaction
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_bufferRead(AT45DB_Handle handle, AT45DB_Transaction* transaction);

/*!
 *  @brief  Copies the content of SRAM buffer to page in Main memory (Buffer 1)
 *
 *  This copies the content of the buffer to specified page in main memory.
 *  it uses the built in erase where the part first erases the page in
 *  main memory before copying the content of the buffer to it.
 *
 *  @param  handle    A AT45DB_Handle
 *  @param  page      Main memory page address
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_bufferToPage(AT45DB_Handle handle, unsigned int page);

/*!
 *  @brief  Write to one of the SRAM data buffers (Buffer 1)
 *
 *  This function writes to one of the SRAM data buffers
 *
 *  @param  handle       A AT45DB_Handle
 *  @param  transaction  A AT45DB_Transaction
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_bufferWrite(AT45DB_Handle handle, AT45DB_Transaction* transaction);

/*!
 *  @brief  Closes the AT45DB object
 *
 *  This function closes the AT45DB object
 *
 *  @param  handle   A AT45DB_Handle
 *
 */
extern void AT45DB_close(AT45DB_Handle handle);

/*!
 *  @brief  One time config of device page-size to 512 bytes
 *
 *  Configures the page-size of the AT45DB device from default 528 bytes
 *      to 512 bytes (NOTE:CAN'T BE REVERSED)
 *
 *  @param  handle   A AT45DB_Handle
 *
 */
extern void AT45DB_config(AT45DB_Handle handle);

/*!
 *  @brief  Opens the AT45DB object
 *
 *  This function opens the AT45DB object
 *
 *  @param  handle   A AT45DB_Handle
 *
 */
extern AT45DB_Handle AT45DB_open(unsigned int index);

/*!
 *  @brief  Erases page in Main memory
 *
 *  This function erases a specified page in main memory.
 *
 *  @param  handle    A AT45DB_Handle
 *  @param  page      Main memory page address
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_pageErase(AT45DB_Handle handle, unsigned int page);

/*!
 *  @brief  Writes to page in Main memory through buffer
 *
 *  This function is a combination of the bufferWrite and bufferToPage
 *  functions. It writes to a page in main memory by first writing to
 *  buffer and then copying the contents of the buffer to the specified
 *  page in main memory.
 *
 *  @param  handle       A AT45DB_Handle
 *  @param  transaction  A AT45DB_Transaction
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_pageWriteThroughBuffer(AT45DB_Handle handle, AT45DB_Transaction* transaction);

/*!
 *  @brief  Reads page in Main memory
 *
 *  This function reads a specified page in main memory
 *
 *  @param  handle       A AT45DB_Handle
 *  @param  transaction  A AT45DB_Transaction
 *  @return true if successful; else false
 *
 */
extern bool AT45DB_readMainMemoryPage(AT45DB_Handle handle, AT45DB_Transaction* transaction);

/*!
 *  @brief  Reads device status register
 *
 *  This function reads the device status register
 *
 *  @param  handle       A AT45DB_Handle
 *  @return true if successful; else false
 *
 */
extern uint8_t AT45DB_readStatusRegister(AT45DB_Handle handle);

//TODO uint8_t *AT45DB_readManufacturerandDeviceID(AT45DB_Handle handle);

#endif /* __AT45DB_H */
