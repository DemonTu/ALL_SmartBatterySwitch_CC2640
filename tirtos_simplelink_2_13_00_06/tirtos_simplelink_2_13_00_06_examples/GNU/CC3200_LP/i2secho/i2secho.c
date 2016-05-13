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

/*
 *  ======== uartecho.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* Kernel Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2S.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2s/I2SCC3200DMA.h>

/* Example/Board Header files */
#include "Board.h"
#include "ti_codec.h"


unsigned short RawSoundData[10240];
Semaphore_Params semWaterMark;
Semaphore_Handle semWaterMarkHandle;

I2S_Handle i2sHandle;
I2C_Handle i2cHandle;

/*
 *  ======== echoFxn ========
 *  Task for this function is created statically. See the project's .cfg file.
 */
Void rxEchoFxn(UArg arg0, UArg arg1)
{
    unsigned short *ptr = RawSoundData;
    unsigned int i = 0;
    I2SCC3200DMA_SerialPinParams I2SCC3200DMA_serialParams;
    I2C_Params i2cParams;
    I2S_Params i2sParams;
    I2S_BufDesc bufReadDesc[3];
    I2S_BufDesc *pReadDesc = NULL;
    Error_Block eb;

    Error_init(&eb);

    bufReadDesc[0].bufPtr = ptr;
    bufReadDesc[0].bufSize = 1024;

    ptr += 512;
    bufReadDesc[1].bufPtr = ptr;
    bufReadDesc[1].bufSize = 1024;

    ptr += 512;
    bufReadDesc[2].bufPtr = ptr;
    bufReadDesc[2].bufSize = 1024;

    Semaphore_Params_init(&semWaterMark);
    semWaterMark.mode = Semaphore_Mode_BINARY;
    semWaterMarkHandle = Semaphore_create(0,&semWaterMark, &eb);

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2cHandle = I2C_open(Board_I2C0, &i2cParams);
    if (i2cHandle == NULL) {
        System_abort("Error Initializing I2C\n");
    }
    else {
        System_printf("I2C Initialized!\n");
    }

    /* Configure Audio Codec */
    ConfigureAudioCodec(CODEC_I2S_WORD_LEN_24);

    I2S_Params_init(&i2sParams);
    I2SCC3200DMA_Params_init(&I2SCC3200DMA_serialParams);
    i2sParams.customParams = (I2SCC3200DMA_SerialPinParams *)&I2SCC3200DMA_serialParams;
    i2sParams.operationMode = I2S_OPMODE_TX_RX_SYNC;
    i2sHandle = I2S_open(Board_I2S0, &i2sParams);

    I2S_readIssue(i2sHandle, &bufReadDesc[0]);
    I2S_readIssue(i2sHandle, &bufReadDesc[1]);
    I2S_readIssue(i2sHandle, &bufReadDesc[2]);

    Semaphore_post(semWaterMarkHandle);
    Task_yield();
    i= 2;

    while (1) {
        ptr += 512;
        if (i == 10) {
            ptr = RawSoundData;
            i=0;
        }

        I2S_readReclaim(i2sHandle,&pReadDesc);
        pReadDesc->bufPtr = ptr;
        pReadDesc->bufSize = 1024;
        I2S_readIssue(i2sHandle, pReadDesc);
        i++;
    }
}

Void txEchoFxn(UArg arg0, UArg arg1)
{
    unsigned short *ptr = RawSoundData;
    unsigned int i = 0;
    I2S_BufDesc bufDesc[3];
    I2S_BufDesc *pDesc = NULL;

    bufDesc[0].bufPtr = ptr;
    bufDesc[0].bufSize = 1024;

    ptr += 512;
    bufDesc[1].bufPtr = ptr;
    bufDesc[1].bufSize = 1024;

    ptr += 512;
    bufDesc[2].bufPtr = ptr;
    bufDesc[2].bufSize = 1024;

    Semaphore_pend(semWaterMarkHandle, BIOS_WAIT_FOREVER);

    I2S_writeIssue(i2sHandle, &bufDesc[0]);
    I2S_writeIssue(i2sHandle, &bufDesc[1]);
    I2S_writeIssue(i2sHandle, &bufDesc[2]);

    i = 2;
    while (1) {
        i++;
        ptr += 512;
        if (i == 10) {
            ptr = RawSoundData;
            i= 0;
        }

        I2S_writeReclaim(i2sHandle, &pDesc);
        if (pDesc ==  NULL)
            while (1);

        pDesc->bufPtr = ptr;
        pDesc->bufSize = 1024;
        I2S_writeIssue(i2sHandle, pDesc);
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initI2C();
    Board_initI2S();

#if 0 /* disable GPIO until we decide how to deal with LED/I2S pin conflict */
    Board_initGPIO();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);
#endif

    /* This example has logging and many other debug capabilities enabled */
    System_printf("This example does not attempt to minimize code or data "
                  "footprint\n");
    System_flush();

    System_printf("Starting the I2S Echo example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in "
                  "ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
