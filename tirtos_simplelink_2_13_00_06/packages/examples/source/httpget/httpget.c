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
 *  ======== httpget.c ========
 *  A HTTP Client example application that GETs weather info.
 */

#include <string.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/net/http/httpcli.h>

#include "Board.h"

/*
 *  USER STEP: change to api.openweathermap.org IP & PORT. Or to your proxy
 *  IP and PORT.
 */
#define IP    "192.91.66.132"
#define PORT  80

/* USER STEP: change city name (cannot have whitespaces) */
#define CITYNAME "Santa%20Barbara,US"
#define HOSTNAME "api.openweathermap.org"
#define REQUEST_URI "/data/2.5/weather?q=" CITYNAME "&units=imperial"

#define TCPHANDLERSTACK 2048

/*
 *  ======== error ========
 */
void error(char *errString, int code)
{
    System_printf("Error! code = %d, desc = %s\n", code, errString);
    BIOS_exit(code);
}

/*
 *  ======== printTemperature ========
 */
void printWeather(char *data, int size)
{
    char *pStart;
    char *pEnd;

    pStart = strstr(data, "temp\":");
    if (pStart != NULL) {
        /* Skip 'temp\":' characters */
        pStart += 6;
        pEnd = strstr(pStart, ",");
        if (pEnd != NULL) {
            *pEnd = 0;

            System_printf("Temperature : %sF\n", pStart);

            *pEnd = ',';
        }
    }

    pStart = strstr(data, "description\":");
    if (pStart != NULL) {
        /* Skip 'description\":' characters */
        pStart += 13;
        pEnd = strstr(pStart, ",");
        if (pEnd != NULL) {
            *pEnd = 0;

            System_printf("Weather Condition : %s\n", pStart);

            *pEnd = ',';
        }
    }
}

/*
 *  ======== httpTask ========
 */
void httpTask(unsigned int arg0, unsigned int arg1)
{
    bool moreFlag = false;
    char data[512];
    int ret;
    struct sockaddr_in addr;

    HTTPCli_Struct cli;
    HTTPCli_Field fields[2] = {
        { HTTPCli_FIELD_NAME_HOST, HOSTNAME },
        { NULL, NULL }
    };

    System_printf("\nGetting weather information for %s\n", CITYNAME);
    System_flush();

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr);

    HTTPCli_construct(&cli);

    ret = HTTPCli_connect(&cli, (struct sockaddr *)&addr, 0, NULL);
    if (ret < 0) {
        error("httpTask: connect failed", ret);
    }

    HTTPCli_setRequestFields(&cli, fields);

    ret = HTTPCli_sendRequest(&cli, HTTPCli_METHOD_GET, REQUEST_URI, false);
    if (ret < 0) {
        error("httpTask: send failed", ret);
    }

    ret = HTTPCli_getResponseStatus(&cli);
    if (ret != HTTP_OK) {
        error("httpTask: cannot get status", ret);
    }

    ret = HTTPCli_getResponseField(&cli, data, sizeof(data), &moreFlag);
    if (ret != HTTPCli_FIELD_ID_END) {
        error("httpTask: response field processing failed", ret);
    }

    do {
       ret = HTTPCli_readResponseBody(&cli, data, sizeof(data), &moreFlag);
       if (ret < 0) {
           error("httpTask: response body processing failed", ret);
       }

       printWeather(data, ret);
    }
    while (moreFlag);

    System_flush();
    HTTPCli_disconnect(&cli);
    HTTPCli_destruct(&cli);
}

/*
 *  ======== netIPAddrHook ========
 *  This function is called when IP Addr is added/deleted
 */
void netIPAddrHook(unsigned int IPAddr, unsigned int IfIdx, unsigned int fAdd)
{
    static Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;

    if (fAdd && !taskHandle) {
        Error_init(&eb);

        Task_Params_init(&taskParams);
        taskParams.stackSize = TCPHANDLERSTACK;
        taskParams.priority = 1;
        taskHandle = Task_create((Task_FuncPtr)httpTask, &taskParams, &eb);
        if (taskHandle == NULL) {
            error("netIPAddrHook: Failed to create HTTP client Task\n", 1);
        }
    }
}

/*
 *  ======== main ========
 */
int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initEMAC();

    System_printf("Starting the HTTP GET example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
