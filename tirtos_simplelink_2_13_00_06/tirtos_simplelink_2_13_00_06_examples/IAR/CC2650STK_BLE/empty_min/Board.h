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
 *  ======== Board.h ========
 */

#define CC2650ST_0120

/*
 * This is a simple gateway to the real Board.c file
 * Based on your include path to Board.h, the corresponding Board.c will also be included
 */
#if defined (CC2650ST_0100)
    #include <ti/boards/SensorTag/CC26XXST_0100/Board.h>
#elif defined (CC2650ST_0110)
    #include <ti/boards/SensorTag/CC26XXST_0110/Board.h>
#elif defined (CC2650ST_0120)
    #include <ti/boards/SensorTag/CC26XXST_0120/Board.h>
#else
    #error "Must define either 'CC2650EM_7ID', 'CC2650EM_5XD', or 'CC2650EM_4XS'. Please set include path to point to appropriate CC2650EM device"
#endif

/* These #defines allow us to reuse TI-RTOS across other device families */
#define     Board_LED0              Board_LED1

#define     Board_BUTTON0           Board_KEY_LEFT
#define     Board_BUTTON1           Board_KEY_RIGHT

#define     Board_I2C0              Board_I2C
#define     Board_I2C_TMP           Board_I2C0
#define     Board_UART0             Board_UART
#define     Board_AES0              Board_AES
#define     Board_WATCHDOG0         Board_WATCHDOG

#define     Board_initGeneral()     PIN_init(BoardGpioInitTable)
#define     Board_initGPIO()
#define     Board_initI2C()         I2C_init()
#define     Board_initSPI()         SPI_init()
#define     Board_initUART()        UART_init()
#define     Board_initWatchdog()    Watchdog_init()
#define     GPIO_toggle(n)
#define     GPIO_write(n,m)

/* Board specific I2C addresses */
#define     Board_TMP006_ADDR       (0x44)
#define     Board_MPU9250_ADDR      (0x68)
#define     Board_SHT21_ADDR        (0x40)
#define     Board_OPT3001_ADDR      (0x45)
#define     Board_BMP280_ADDR       (0x77)
