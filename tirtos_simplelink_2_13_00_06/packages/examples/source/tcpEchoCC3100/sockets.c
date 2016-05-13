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
 *  ======== sockets.c ========
 */

#include <stdbool.h>
#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* SimpleLink Wi-Fi Host Driver Header files */
#include <simplelink.h>
#include <osi.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/WiFi.h>

/* Example/Board Header file */
#include "Board.h"

#if defined(MSP430WARE) || defined(MSP432WARE)
#define SPI_BIT_RATE    2000000
#else
#define SPI_BIT_RATE    14000000
#endif

bool deviceConnected = false;
bool ipAcquired = false;

/*
 *  ======== SimpleLinkWlanEventHandler ========
 *  SimpleLink Host Driver callback for handling WLAN connection or
 *  disconnection events.
 */
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pArgs)
{
    switch (pArgs->Event) {
        case SL_WLAN_CONNECT_EVENT:
            deviceConnected = true;
            break;

        case SL_WLAN_DISCONNECT_EVENT:
            deviceConnected = false;
            break;

        default:
            break;
    }
}

/*
 *  ======== SimpleLinkNetAppEventHandler ========
 *  SimpleLink Host Driver callback for asynchoronous IP address events.
 */
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pArgs)
{
    switch (pArgs->Event) {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
            ipAcquired = true;
            break;

        default:
            break;
    }
}

/*
 *  ======== setStationMode ========
 *  Sets the SimpleLink Wi-F in station mode and enables DHCP client
 */
void setStationMode(void) {
    int           mode;
    int           response;
    unsigned char param;

    mode = sl_Start(0, 0, 0);
    if (mode < 0) {
        System_abort("Could not initialize SimpleLink Wi-Fi");
    }

    /* Change network processor to station mode */
    if (mode != ROLE_STA) {
        sl_WlanSetMode(ROLE_STA);

        /* Restart network processor */
        sl_Stop(BIOS_WAIT_FOREVER);
        mode = sl_Start(0, 0, 0);
        if (mode < 0) {
            System_abort("Failed to set SimpleLink Wi-Fi to Station mode");
        }
    }

    sl_WlanDisconnect();

    /* Set auto connect policy */
    response = sl_WlanPolicySet(SL_POLICY_CONNECTION,
            SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL, 0);
    if (response < 0) {
        System_abort("Failed to set connection policy to auto");
    }

    /* Enable DHCP client */
    param = 1;
    response = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &param);
    if(response < 0) {
        System_abort("Could not enable DHCP client");
    }

    sl_Stop(BIOS_WAIT_FOREVER);

    /* Set connection variables to initial values */
    deviceConnected = false;
    ipAcquired = false;
}

/*
 *  ======== socketsShutDown ========
 *  Generic routine, defined to close down the WiFi in this case.
 */
void socketsShutDown(void *handle)
{
    sl_Stop(BIOS_WAIT_FOREVER);

    WiFi_close((WiFi_Handle)handle);
}

/*
 *  ======== socketsStartUp ========
 *  Generic routine, in this case defined to open the WiFi and await a
 *  connection, using Smart Config if the appropriate button is pressed.
 */
void *socketsStartUp(void)
{
    uint32_t           currButton;
    uint32_t           prevButton = 1;
    WiFi_Params        wifiParams;
    WiFi_Handle        handle;

    SlNetCfgIpV4Args_t ipV4;
    uint8_t            len = sizeof(ipV4);
    uint8_t            dhcpIsOn;
    int                result;

    /*
     * Board_LED1 is used as a connection indicator.  It will blink until a
     * connection is establish with the AP.
     */
    GPIO_write(Board_LED1, Board_LED_OFF);

    /* Open WiFi driver */
    WiFi_Params_init(&wifiParams);
    wifiParams.bitRate = SPI_BIT_RATE;
    handle = WiFi_open(Board_WIFI, Board_SPI_CC3100, NULL, &wifiParams);
    if (handle == NULL) {
        System_abort("WiFi driver failed to open.");
    }

    /* Set the CC3100 into station mode for this example */
    setStationMode();

    /* Host driver starts the network processor */
    result = sl_Start(NULL, NULL, NULL);
    if (result < 0) {
        System_abort("Could not intialize SimpleLink Wi-Fi");
    }

    /*
     * Wait for SimpleLink to connect to an AP. If connecting to the AP for
     * the first time, press Board_BUTTON0 to start SmartConfig.
     */
    while ((deviceConnected != true) || (ipAcquired != true)) {
        /*
         *  This could be done with GPIO interrupts, but for simplicity polling
         *  is used to check the button.
         */
        currButton = GPIO_read(Board_BUTTON0);
        if((currButton == 0) && (prevButton != 0)) {
            /*
             *  Starts the SmartConfig process which allows the user to tell
             *  the CC3100 which AP to connect to.
             *  Downloads available here: http://www.ti.com/tool/wifistarter
             */
            sl_WlanSmartConfigStart(NULL, SMART_CONFIG_CIPHER_NONE, NULL, NULL,
                    NULL, NULL, NULL, NULL);
        }
        prevButton = currButton;
        Task_sleep(50);
    }

    GPIO_write(Board_LED1, Board_LED_ON);

    /* Print IP address */
    sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO, &dhcpIsOn, &len,
            (unsigned char *)&ipV4);
    System_printf(
            "CC3100 has connected to an AP and acquired an IP address.\n");
    System_printf("IP Address: %d.", SL_IPV4_BYTE(ipV4.ipV4, 3));
    System_printf("%d.", SL_IPV4_BYTE(ipV4.ipV4, 2));
    System_printf("%d.", SL_IPV4_BYTE(ipV4.ipV4, 1));
    System_printf("%d", SL_IPV4_BYTE(ipV4.ipV4, 0));
    System_flush();

    /* pass back the handle to our WiFi device */
    return ((void *)handle);
}
