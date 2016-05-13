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
 *  ======== empty.c ========
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
#include "AT45DBFatFs.h"

#include <string.h>

/* String conversion macro */
#define STR_(n)             #n
#define STR(n)              STR_(n)

/* Drive number used for FatFs */
#define DRIVE_NUM           0

#define TEXT_LENGTH         577

unsigned char textarray[TEXT_LENGTH] = \
"***********************************************************************\n"
"A   T   4   5   D   B   1   6   1   D    N   A   N   D   F  L  A  S  H \n"
"This is some text to be written to the input file if there isn't       \n"
"already an existing file located on the flash device.                  \n"
"If an input file already exists, or if the file was already once       \n"
"generated, then the input file will NOT be modified.                   \n"
"Also if the disk isn't formatted, format it first.                     \n"
"***********************************************************************\n";

const char  inputfile[] = STR(DRIVE_NUM)":input0.txt";

unsigned char readarray[TEXT_LENGTH];     /* Read Buffer to hold text */
AT45DBFatFs_Handle at45dbHandle;  /* handle for AT45DBFatFs */

FIL src; /* Variable for the CIO functions */

Void myTaskFxn (UArg arg0, UArg arg1) {

    FRESULT fresult;

    /* Variables to keep track of the file copy progress */
    unsigned int bytesRead = 0;
    unsigned int bytesWritten = 0;

    /* open AT45DBFatFs*/
    at45dbHandle = AT45DBFatFs_open(DRIVE_NUM);

    /* open input file if it already exists on flash*/
    fresult = f_open(&src, inputfile, FA_WRITE|FA_READ);
    if (fresult != FR_OK) {
        /* Format disk if no existing file-system is found */
        if (fresult == FR_NO_FILESYSTEM) {
            System_printf("Formatting Flash disk\n");
            fresult = f_mkfs(at45dbHandle->driveNumber, 0, FLASH_SECTOR_SIZE );
            if (fresult != FR_OK) {
                AT45DBFatFs_close(at45dbHandle);
                System_abort("Aborting...\n");
            }
        }
        System_printf("Creating a new file \"%s\"...\n", inputfile);

        /* Open new file for both reading and writing */
        fresult = f_open(&src, inputfile, FA_CREATE_NEW|FA_READ|FA_WRITE);
        if (fresult != FR_OK) {
            System_printf("Error: \"%s\" could not be created\n",
                    inputfile);
            System_abort("Aborting...\n");
        }
    }

    /* Write text string to input file */
    f_write(&src, textarray, strlen(textarray), &bytesWritten);

    /* Reset the internal file pointer */
    f_lseek(&src, 0);

    /*  Read from source file */
    fresult = f_read(&src, readarray, f_size(&src), &bytesRead);

    /* Write output */
    System_printf("%s",readarray);

    /* Close the file */
    f_close(&src);

    AT45DBFatFs_close(at45dbHandle);

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
