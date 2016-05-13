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
 *  @file       AT45DBFatFs.h
 *
 *  @brief      FatFs Driver for AT45DB161D SPI Nand Flash
 *
 *  The AT45DBFatFs header file should be included in an application as follows:
 *  @code
 *  #include <AT45DBFatFs.h>
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef AT45DBFATFS_H_
#define AT45DBFATFS_H_

#undef DIR

#include <ti/sysbios/fatfs/ff.h>
#include <ti/sysbios/fatfs/diskio.h>

#include "AT45DB.h"

#define FLASH_SECTOR_SIZE           512
#define DRIVE_NOT_MOUNTED           ~0

/* Typedefs */
typedef struct AT45DBFatFs_Object {
    AT45DB_Handle at45dbHandle;  /*!< Handle used by AT45DB driver */
    BYTE          driveNumber;   /*!< Drive number used by FatFs */
    FATFS         fileSystem;    /*!< FATFS data object */
    DSTATUS       diskState;     /*!< Disk status */
}AT45DBFatFs_Object;

typedef AT45DBFatFs_Object *AT45DBFatFs_Handle;

/*!
 *  @brief  Opens the AT45DBFatFs object
 *
 *  This function opens the AT45DBFatFs object
 *
 *  @param  drv   drive number for FatFs
 *
 *  @return  A AT45DBFatFs_Handle
 *
 */
AT45DBFatFs_Handle AT45DBFatFs_open(unsigned int drv);

/*!
 *  @brief  Closes the AT45DBFatFs object
 *
 *  This function closes the AT45DBFatFs object
 *
 *  @param  handle   A AT45DBFatFs_Handle
 *
 */
void AT45DBFatFs_close(AT45DBFatFs_Handle handle);


#endif /* AT45DBFATFS_H_ */
