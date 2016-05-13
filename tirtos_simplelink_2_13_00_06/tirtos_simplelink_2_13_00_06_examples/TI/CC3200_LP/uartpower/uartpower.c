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
 *  ======== uartpower.c ========
 *  UART test using power.
 *  This test writes data to the UART and then sleeps for 5 seconds,
 *  allowing the processor to enter Sleep mode.
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/UART.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Example/Board Header files */
#include "Board.h"

#define NLOOPS 200


Task_Handle writer;

int doTransfers = 1;
int count = 0;
int badCount = 0;

static void blinkLED(uint32_t led, uint32_t count);
Void writerFxn(UArg a0, UArg a1);

const char dummyBuffer[] = "abcdefghijklmnopqrstuvwxyz\r\n";
const char sleepMsg[] = "\fGoing to sleep for 5 seconds...\r\n";

/*
 *  ======== blinkLED ========
 */
static void blinkLED(uint32_t led, uint32_t count)
{
    int i;

    for (i = 0; i < 500000 * (count * 2); i++) {
        if ((i % 500000) == 0) {
            GPIO_toggle(led);
        }
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Initialize the board and UART pins and clocks */
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();

    System_printf("Starting the UART power example!\n");
    System_flush();

    /*
     *  This function enables the Power policy function to run
     *  in the idle loop. Alternatively, you can set the field
     *  'enablePolicy' in the Power_config structure (CC3200_LP.c)
     *  to 1.
     */
    Power_enablePolicy();

    GPIO_write(Board_LED0, Board_LED_ON);

    /* Enable interrupts and start SYS/BIOS */
    BIOS_start();

    return (0);
}

/*
 *  ======== writerFxn ========
 */
Void writerFxn(UArg a0, UArg a1)
{
    UART_Handle uart0;
    UART_Params uartParams;
    int i;

    /* Create a UART with the parameters below. */
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;

    uart0 = UART_open(Board_UART0, &uartParams);
    if (uart0 == NULL) {
        System_abort("Error creating UART!");
    }

    /* Stop the receive to go into low power state */
    UART_control(uart0, UART_CMD_RXDISABLE, NULL);

    /* Write to UART0 */
    for (;;) {
        for (i = 0; i < NLOOPS; i++) {
            UART_write(uart0, (const void *)dummyBuffer, sizeof(dummyBuffer));
        }

        UART_write(uart0, (const void *)sleepMsg, sizeof(sleepMsg));

        /* Sleep for 5 seconds */
        Task_sleep(5000);

        /* Blink the LED to indicate that we woke up. */
        blinkLED(Board_LED0, 2);
    }
}
