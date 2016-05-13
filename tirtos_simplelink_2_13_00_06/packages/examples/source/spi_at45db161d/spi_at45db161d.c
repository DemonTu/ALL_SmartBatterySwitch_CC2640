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
 *  ======== spi_at45db161d.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include "Board.h"
#include "AT45DB.h"
#include <string.h>

#define PAGE_SIZE 512

unsigned char textarray[PAGE_SIZE] = \
"***********************************************************************\n"
"A   T   4   5   D   B   1   6   1   D    N   A   N   D   F  L  A  S  H \n"
"01234567890123456789012345678901234567890123456789012345678901234567890\n"
"This is some text to be written to the external flash device.          \n"
"This text is written to page 0 starting at the first byte.             \n"
"After a successful write, this text will be recovered from the flash.  \n"
"***********************************************************************";

unsigned char readBuf[PAGE_SIZE];

Void myTaskFxn (UArg arg0, UArg arg1)
{
    AT45DB_Handle at45dbHandle;
    AT45DB_Transaction at45dbTransaction;

    /* Open AT45DB */
    at45dbHandle = AT45DB_open(0);
    if (at45dbHandle == NULL) {
        System_abort("Error opening AT45DB161D");
    }

    /* Initialize at45dbTransaction */
    at45dbTransaction.address.page = 0;
    at45dbTransaction.address.byte = 0;
    at45dbTransaction.data_size = PAGE_SIZE;
    at45dbTransaction.data = textarray;

    if(!AT45DB_pageWriteThroughBuffer(at45dbHandle, &at45dbTransaction) ) {
       System_printf("Page write unsuccessful");
    }
    /* set transaction data to read buffer */
    at45dbTransaction.data = readBuf;

    if(AT45DB_readMainMemoryPage(at45dbHandle, &at45dbTransaction) ) {
        /* Print contents of read buffer */
        System_printf("%s", readBuf);
    }
    else {
        System_printf("Page read unsuccessful");
    }
    /* Close At45DB */
    AT45DB_close(at45dbHandle);

    BIOS_exit(0);
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions. */
    Board_initGeneral();
    Board_initGPIO();
    Board_initSPI();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the example\nSystem provider is set to SysMin. "
                  "Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
