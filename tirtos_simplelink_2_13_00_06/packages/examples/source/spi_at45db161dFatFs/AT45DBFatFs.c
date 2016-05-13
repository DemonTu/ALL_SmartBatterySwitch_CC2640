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
 *  ======== AT45DBFatFs.c ========
 */

/* XDCtools Header files */
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>

#include <string.h>
#include "AT45DBFatFs.h"

#define NUM_DRVS 1

/*
 * Array of AT45DBFatFs_Objects according to the number of drives specified
 * with a NUM_DRVS definition
 */
static AT45DBFatFs_Object at45dbfatfsObjects[NUM_DRVS] =
{{NULL, DRIVE_NOT_MOUNTED, NULL, STA_NOINIT}};


/* FatFs disk I/O functions */
DSTATUS             AT45DBFatFs_diskInitialize(BYTE drv);
DRESULT             AT45DBFatFs_diskIOctrl(BYTE drv, BYTE ctrl, void *buf);
DRESULT             AT45DBFatFs_diskRead(BYTE drv, BYTE *buf, DWORD sector,
                                         BYTE count);
DSTATUS             AT45DBFatFs_diskStatus(BYTE drv);
DRESULT             AT45DBFatFs_diskWrite(BYTE drv, const BYTE *buf, DWORD sector,
                                          BYTE count);


DSTATUS AT45DBFatFs_diskInitialize(unsigned char drv) {

    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);
    handle->diskState &= ~STA_NOINIT;

    return (handle->diskState);
}

DRESULT AT45DBFatFs_diskIOctrl(BYTE drv, BYTE ctrl, void *buf) {

    DRESULT res = RES_ERROR;
    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);
       volatile char ready = 0;

       if (handle->diskState & STA_NOINIT) {
        return (RES_NOTRDY);
    }

    switch (ctrl) {
        case GET_SECTOR_COUNT:
            /* Get number of sectors on the disk (unsigned int) */
            *(unsigned int*)buf = (unsigned int)AT45DB161D_NUMBER_OF_PAGES;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            /* Get sectors on the disk (WORD) */
            *(WORD*)buf = FLASH_SECTOR_SIZE;
                Log_print2(Diags_USER2, "AT45DBFatFs: disk IO control: sector "
                                        "size: %d @drive number %d",
                                         *(WORD*)buf, handle->driveNumber);
            res = RES_OK;
            break;

        case CTRL_SYNC:
            /* Make sure that data has been written */
            ready = AT45DB_readStatusRegister(handle->at45dbHandle);
            if (ready & 0x80) {
                Log_print1(Diags_USER2, "AT45DBFatFs: disk IO control: control "
                        "sync: ready @drive number %d",
                        handle->driveNumber);
                res = RES_OK;
            }
            else {
                Log_print1(Diags_USER2, "AT45DBFatFs: disk IO control: control "
                        "sync: not ready @drive number %d",
                        handle->driveNumber);
                res = RES_NOTRDY;
                res = RES_OK;
            }
            break;

        default:
            Log_print1(Diags_USER2, "AT45DBFatFs: disk IO control: parameter "
                    "error @drive number %d", handle->driveNumber);
            res = RES_OK;
            break;
    }

    return (res);
}

DRESULT AT45DBFatFs_diskRead(BYTE drv, BYTE *buf, DWORD sector, BYTE count) {

    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);
    AT45DB_Transaction at45dbTransaction;

    if (!count) {
       Log_print1(Diags_USER1, "SDSPI: disk read: 0 sectors to read"
                  "@ drive number %d", handle->driveNumber);
        return (RES_PARERR);
    }

    if (handle->diskState & STA_NOINIT) {
        Log_error1("AT45DBFatFs: disk read: disk not initialized"
                  "@ drive number %d", handle->driveNumber);
        return (RES_NOTRDY);
    }

    at45dbTransaction.address.page = (unsigned int)sector;
    at45dbTransaction.address.byte = 0;
    at45dbTransaction.data_size = FLASH_SECTOR_SIZE;
    at45dbTransaction.data = buf;

    /* Single block read */
    if (count == 1) {
        if (AT45DB_readMainMemoryPage(handle->at45dbHandle, &at45dbTransaction)) {
            count = 0;
        }
    }
    /* Multiple block read */
    else {
            do {
                if (AT45DB_readMainMemoryPage(handle->at45dbHandle, &at45dbTransaction)) {
                    buf += FLASH_SECTOR_SIZE;
                    (at45dbTransaction.address.page)++;
                }
            } while (--count);
    }

    return (count ? RES_ERROR : RES_OK);
}

DSTATUS AT45DBFatFs_diskStatus(unsigned char drv) {
    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);
    return (handle->diskState);
}

DRESULT AT45DBFatFs_diskWrite(BYTE drv, const BYTE *buf, DWORD sector, BYTE count) {

    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);
    AT45DB_Transaction at45dbTransaction;

    if (!count) {
        Log_print1(Diags_USER1, "AT45DBFatFs: disk write: 0 sectors to write"
                  "@ drive number %d", handle->driveNumber);
        return (RES_PARERR);
    }
    if (handle->diskState & STA_NOINIT) {
        Log_error1("AT45DBFatFs: disk write: disk not initialized"
                 "@ drive number %d", handle->driveNumber);

        return (RES_NOTRDY);
    }
    if (handle->diskState & STA_PROTECT) {
        Log_error1("AT45DBFatFs: disk write: disk protected"
                "@ drive number %d", handle->driveNumber);

        return (RES_WRPRT);
    }

    at45dbTransaction.address.page = (unsigned int)sector;
    at45dbTransaction.address.byte = 0;
    at45dbTransaction.data_size = FLASH_SECTOR_SIZE;
    at45dbTransaction.data = (unsigned char*)buf;

    /* Single block write */
    if (count == 1) {
        if (AT45DB_pageWriteThroughBuffer(handle->at45dbHandle,
                &at45dbTransaction)) {

            count = 0;
        }
    }
    /* Multiple block write */
    else {
        /* WRITE_MULTIPLE_BLOCK */
            do {
                if (AT45DB_pageWriteThroughBuffer(handle->at45dbHandle,
                        &at45dbTransaction)) {

                    at45dbTransaction.data += FLASH_SECTOR_SIZE;
                    (at45dbTransaction.address.page)++;
                }

            } while (--count);
    }

    return (count ? RES_ERROR : RES_OK);
}

void AT45DBFatFs_close(AT45DBFatFs_Handle handle) {

    FRESULT                         fresult;
    DRESULT                         dresult;
    unsigned int key;

    /* Unmount the FatFs drive */
    fresult = f_mount(handle->driveNumber, NULL);
    if (fresult != FR_OK) {
        Log_print1(Diags_USER1, "AT45DBFatFs: Could not unmount FatFs volume "
                "@drive number %d", handle->driveNumber);
    }

    /* Unregister the disk_*() functions */
    dresult = disk_unregister(handle->driveNumber);
    if (dresult != RES_OK) {
        Log_print1(Diags_USER1, "AT45DBFatFs: Error unregistering disk functions "
                "@ drive number %d", handle->driveNumber);
    }
    AT45DB_close(handle->at45dbHandle);

    Log_print1(Diags_USER1,"AT45DBFatFs closed @ drive number %d", handle->driveNumber);

    key = Hwi_disable();
    handle->driveNumber = DRIVE_NOT_MOUNTED;
    Hwi_restore(key);

}

AT45DBFatFs_Handle AT45DBFatFs_open(BYTE drv) {

    DRESULT                         dresult;
    FRESULT                         fresult;
    unsigned int                    key;

    if(drv > NUM_DRVS) {
        return (NULL);
    }
    AT45DBFatFs_Handle handle = &(at45dbfatfsObjects[drv]);

    /* Determine if the device was already opened */
    key = Hwi_disable();
    if (handle->driveNumber != DRIVE_NOT_MOUNTED) {
        Hwi_restore(key);
        return (NULL);
    }
    /* Mark as being used */
    handle->driveNumber = drv;
    Hwi_restore(key);

    handle->at45dbHandle = AT45DB_open(drv);
    if(handle->at45dbHandle == NULL) {
        return (NULL);
    }

    /* Register the new disk_*() functions */
    dresult = disk_register(handle->driveNumber,
                            AT45DBFatFs_diskInitialize,
                            AT45DBFatFs_diskStatus,
                            AT45DBFatFs_diskRead,
                            AT45DBFatFs_diskWrite,
                            AT45DBFatFs_diskIOctrl);

    /* Check for drive errors */
    if (dresult != RES_OK) {
        Log_error1("disk functions not registered @ drive number %d",
                handle->driveNumber);

        AT45DBFatFs_close(handle);
        return NULL;
    }

    fresult = f_mount(handle->driveNumber, &(handle->fileSystem));
    if (fresult != FR_OK) {
       Log_error1("AT45DBFatFs drive not mounted @ drive number %d",
               handle->driveNumber);

       AT45DBFatFs_close(handle);
       return NULL;
    }

    Log_print1(Diags_USER1,"AT45DBFatFs opened @ drive number %d", handle->driveNumber);
    return handle;
}
