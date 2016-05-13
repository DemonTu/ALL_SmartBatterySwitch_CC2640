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
 *  ======== httpsget.c ========
 *  A HTTP Client with TLS example application that GETs stock info.
 */

#include <string.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/ndk/nettools/mytime/mytime.h>
#include <ti/net/http/httpcli.h>
#include <cyassl/ssl.h>

#include "Board.h"

/* USER STEP: read readme.txt for steps to generated certificate.h */
#include "certificate.h"

/*
 *  USER STEP: change to download.finance.yahoo.com's IP and PORT. Or to
 *  your proxy IP and PORT.
 */
#define IP    "192.91.66.132"
#define PORT  80

/* USER STEP: change stock ticker symbol */
#define TICKER "TXN"
#define HOSTNAME "download.finance.yahoo.com:443"
#define REQUEST_URI "/d/quotes.csv?s=" TICKER "&f=sl1d1t1c1&e=.csv"

#define TCPHANDLERSTACK 16384

/* USER STEP: update to current time (in secs) since epoch (i.e. Jan 1, 1970) */
#define CURRENTTIME 1425324179

/*
 *  ======== error ========
 */
void error(char *errString, int code)
{
    System_printf("Error! code = %d, desc = %s\n", code, errString);
    System_flush();
    BIOS_exit(code);
}

/*
 *  ======== httpsTask ========
 */
void httpsTask(unsigned int arg0, unsigned int arg1)
{
    bool moreFlag = false;
    char data[128];
    int ret;
    struct sockaddr_in addr;

    CYASSL_CTX *ctx;
    HTTPCli_SecureParams sparams;
    HTTPCli_Struct cli;
    HTTPCli_Field fields[2] = {
        { HTTPCli_FIELD_NAME_HOST, HOSTNAME },
        { NULL, NULL }
    };

    System_printf("\nGetting stock information for ticker: %s\n", TICKER);
    System_flush();

    /* CyaSSL library needs time() for validating certificates. */
    MYTIME_init();
    MYTIME_settime(CURRENTTIME);

    CyaSSL_Init();

    ctx = CyaSSL_CTX_new(CyaTLSv1_2_client_method());
    if (ctx == NULL) {
        error("httpsTask: cannot create cyassl context", -1);
    }

    if (CyaSSL_CTX_load_verify_buffer(ctx, ca_cert, sizeof_ca_cert,
            SSL_FILETYPE_ASN1) != SSL_SUCCESS) {
        error("httpsTask: error loading ca certificate", -1);
    }
    sparams.ctx = ctx;
    HTTPCli_setSecureParams(&sparams);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &addr.sin_addr);

    /* USER STEP: Comment the line if not using proxy */
    HTTPCli_setProxy((struct sockaddr *)&addr);

    HTTPCli_construct(&cli);

    HTTPCli_setRequestFields(&cli, fields);

    ret = HTTPCli_connect(&cli, (struct sockaddr *)&addr, HTTPCli_TYPE_TLS,
            NULL);
    if (ret < 0) {
        error("httpsTask: connect failed", ret);
    }

    ret = HTTPCli_sendRequest(&cli, HTTPCli_METHOD_GET, REQUEST_URI, false);
    if (ret < 0) {
        error("httpsTask: send failed", ret);
    }

    ret = HTTPCli_getResponseStatus(&cli);
    if (ret != HTTP_OK) {
        error("httpsTask: cannot get status", ret);
    }

    ret = HTTPCli_getResponseField(&cli, data, sizeof(data), &moreFlag);
    if (ret != HTTPCli_FIELD_ID_END) {
        error("httpsTask: response field processing failed", ret);
    }

    do {
        ret = HTTPCli_readResponseBody(&cli, data, sizeof(data), &moreFlag);
        if (ret < 0) {
            error("httpsTask: response body processing failed", ret);
        }
        else if (ret) {
            if (ret < sizeof(data)) {
                data[ret] = '\0';
            }

            System_printf("%s", data);
        }
    }
    while (moreFlag);

    System_flush();

    HTTPCli_disconnect(&cli);
    HTTPCli_destruct(&cli);

    CyaSSL_CTX_free(ctx);
    CyaSSL_Cleanup();
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
        taskHandle = Task_create((Task_FuncPtr)httpsTask, &taskParams, &eb);
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

    System_printf("Starting the HTTPS GET example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
