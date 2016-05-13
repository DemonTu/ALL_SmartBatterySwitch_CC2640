/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
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
 *  ======== spiloopback.c ========
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

#define SPI_MSG_LENGTH    26

/* Allocate buffers in .dma section of memory for concerto devices */
#ifdef MWARE
#pragma DATA_SECTION(masterRxBuffer, ".dma");
#pragma DATA_SECTION(masterTxBuffer, ".dma");
#pragma DATA_SECTION(slaveRxBuffer, ".dma");
#pragma DATA_SECTION(slaveTxBuffer, ".dma");
#endif

unsigned char masterRxBuffer[SPI_MSG_LENGTH];
unsigned char masterTxBuffer[SPI_MSG_LENGTH] = "Hello, this is master SPI";

unsigned char slaveRxBuffer[SPI_MSG_LENGTH];
unsigned char slaveTxBuffer[SPI_MSG_LENGTH] =  "Hello, this is slave SPI";
/* *  ======== slaveTaskFxn ========
 *  Task function for slave task.
 *
 *  This task runs on a higher priority, since slave
 *  has to be ready for master. Slave SPI sends a
 *  message to master and also receives message from
 *  master. Task for this function is created
 *  statically. See the project's .cfg file.
 */
Void slaveTaskFxn (UArg arg0, UArg arg1)
{
    SPI_Handle slaveSpi;
    SPI_Params slaveSpiParams;
    SPI_Transaction slaveTransaction;
    bool transferOK;

    /* Initialize SPI handle with slave mode */
    SPI_Params_init(&slaveSpiParams);
    slaveSpiParams.mode = SPI_SLAVE;
    slaveSpi = SPI_open(Board_SPI1, &slaveSpiParams);
    if (slaveSpi == NULL) {
        System_abort("Error initializing SPI\n");
    }
    else {
        System_printf("SPI initialized\n");
    }

    /* Initialize slave SPI transaction structure */
    slaveTransaction.count = SPI_MSG_LENGTH;
    slaveTransaction.txBuf = (Ptr)slaveTxBuffer;
    slaveTransaction.rxBuf = (Ptr)slaveRxBuffer;

    /* Initiate SPI transfer */
    transferOK = SPI_transfer(slaveSpi, &slaveTransaction);

    if(transferOK) {
        /* Print contents of slave receive buffer */
        System_printf("Slave: %s\n", slaveRxBuffer);
    }
    else {
        System_printf("Unsuccessful slave SPI transfer");
    }

    /* Deinitialize SPI */
    SPI_close(slaveSpi);
}
/*
 *  ======== masterTaskFxn ========
 *  Task function for master task.
 *
 *  This task runs at a lower priority after the slave
 *  task to ensure it is ready for a transaction.
 *  Master SPI sends a message to slave and also
 *  receives message from slave. Task for this function
 *  is created statically. See the project's .cfg
 *  file.
 */
Void masterTaskFxn (UArg arg0, UArg arg1)
{
    SPI_Handle masterSpi;
    SPI_Transaction masterTransaction;
    bool transferOK;

    /* Initialize SPI handle as default master */
    masterSpi = SPI_open(Board_SPI0, NULL);
    if (masterSpi == NULL) {
        System_abort("Error initializing SPI\n");
    }
    else {
        System_printf("SPI initialized\n");
    }

    /* Initialize master SPI transaction structure */
    masterTransaction.count = SPI_MSG_LENGTH;
    masterTransaction.txBuf = (Ptr)masterTxBuffer;
    masterTransaction.rxBuf = (Ptr)masterRxBuffer;

    /* Initiate SPI transfer */
    transferOK = SPI_transfer(masterSpi, &masterTransaction);

    if(transferOK) {
        /* Print contents of master receive buffer */
        System_printf("Master: %s\n", masterRxBuffer);
    }
    else {
        System_printf("Unsuccessful master SPI transfer");
    }

    /* Deinitialize SPI */
    SPI_close(masterSpi);

    System_printf("Done\n");

    System_flush();
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

    System_printf("Starting the SPI loop-back example\nSystem provider is set to"
                  " SysMin. Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();
    System_printf("This example requires external wires to be connected to the "
                  "header pins. Please see the Getting Started Guide for "
                  "details.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
