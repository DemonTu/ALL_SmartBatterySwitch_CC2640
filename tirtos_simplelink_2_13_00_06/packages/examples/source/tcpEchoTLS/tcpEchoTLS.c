/*
 * Copyright (c) 2014-2015, Texas Instruments Incorporated
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
 *    ======== tcpEchoTLS.c ========
 */

#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>

/* NDK Header files */
#include <sys/socket.h>
#include <ti/ndk/nettools/mytime/mytime.h>

/* Example/Board Header file */
#include "Board.h"

/* CyaSSL Header files */
#include <cyassl/ssl.h>
#include <cyassl/certs_test.h>

#define TCPWORKERSTACKSIZE 14592
#define TCPPACKETSIZE 256
#define NUMTCPWORKERS 3

/* Current time (in seconds) since epoch (i.e. Jan 1, 1970) */
#define CURRENTTIME 1410303972

/*
 *  ======== exitApp ========
 *  Cleans up the SSL context and exits the application
 */
Void exitApp(CYASSL_CTX *ctx)
{
    if (ctx != NULL) {
        CyaSSL_CTX_free(ctx);
        CyaSSL_Cleanup();
    }

     BIOS_exit(-1);
}

/*
 *  ======== tcpWorker ========
 *  Task to handle TCP connection. Can be multiple Tasks running
 *  this function.
 */
Void tcpWorker(UArg arg0, UArg arg1)
{
    int clientfd = 0;
    int bytesRcvd;
    int  bytesSent;
    char buffer[TCPPACKETSIZE];
    CYASSL *ssl = (CYASSL *)arg0;

    clientfd = CyaSSL_get_fd(ssl);
    System_printf("tcpWorker: start clientfd = 0x%x\n", clientfd);

    /* Loop while we receive data */
    while ( (bytesRcvd = CyaSSL_recv(ssl, (char *)buffer, TCPPACKETSIZE, 0)) > 0) {
        bytesSent = CyaSSL_send(ssl, (char *)buffer, bytesRcvd, 0);
        if (bytesSent < 0 || bytesSent != bytesRcvd) {
            System_printf("Error: send failed.\n");
            break;
        }
    }
    System_printf("tcpWorker stop clientfd = 0x%x\n", clientfd);

    CyaSSL_free(ssl);
    close(clientfd);
}

/*
 *  ======== tcpHandler ========
 *  Creates new Task to handle new TCP connections.
 */
Void tcpHandler(UArg arg0, UArg arg1)
{
    int                status;
    int                clientfd;
    int                server;
    struct sockaddr_in localAddr;
    struct sockaddr_in clientAddr;
    int                optval;
    int                optlen = sizeof(optval);
    socklen_t          addrlen = sizeof(clientAddr);
    Task_Handle        taskHandle;
    Task_Params        taskParams;
    Error_Block        eb;
    CYASSL_CTX        *ctx;
    CYASSL            *ssl;

    CyaSSL_Init();

    ctx = CyaSSL_CTX_new(CyaTLSv1_2_server_method());
    if (ctx == NULL) {
        System_printf("tcpHandler: CyaSSL_CTX_new error.\n");
        goto shutdown;
    }

    if (CyaSSL_CTX_load_verify_buffer(ctx, ca_cert_der_2048,
            sizeof(ca_cert_der_2048)/sizeof(char), SSL_FILETYPE_ASN1) !=
            SSL_SUCCESS) {
        System_printf("tcpHandler: Error loading ca_cert_der_2048"
                " please check the cyassl/certs_test.h file.\n");
        goto shutdown;
    }

    if (CyaSSL_CTX_use_certificate_buffer(ctx, server_cert_der_2048,
            sizeof(server_cert_der_2048)/sizeof(char), SSL_FILETYPE_ASN1) !=
            SSL_SUCCESS) {
        System_printf("tcpHandler: Error loading server_cert_der_2048,"
                " please check the cyassl/certs_test.h file.\n");
        goto shutdown;
    }

    if (CyaSSL_CTX_use_PrivateKey_buffer(ctx, server_key_der_2048,
            sizeof(server_key_der_2048)/sizeof(char), SSL_FILETYPE_ASN1) !=
            SSL_SUCCESS) {
        System_printf("tcpHandler: Error loading server_key_der_2048,"
                " please check the cyassl/certs_test.h file.\n");
        goto shutdown;
    }

    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == -1) {
        System_printf("Error: socket not created.\n");
        goto shutdown;
    }

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(arg0);

    status = bind(server, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (status == -1) {
        System_printf("Error: bind failed.\n");
        goto shutdown;
    }

    status = listen(server, NUMTCPWORKERS);
    if (status == -1) {
        System_printf("Error: listen failed.\n");
        goto shutdown;
    }

    optval = 1;
    if (setsockopt(server, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        System_printf("Error: setsockopt failed\n");
        goto shutdown;
    }

    while ((clientfd =
            accept(server, (struct sockaddr *)&clientAddr, &addrlen)) != -1) {

        /* Init the Error_Block */
        Error_init(&eb);
        ssl = CyaSSL_new(ctx);
        if (ssl == NULL) {
            System_printf("Error: CyaSSL_new failed.\n");
            close(clientfd);
            goto shutdown;
        }

        CyaSSL_set_fd(ssl, clientfd);

        /* Initialize the defaults and set the parameters. */
        Task_Params_init(&taskParams);
        taskParams.arg0 = (UArg)ssl;
        taskParams.stackSize = TCPWORKERSTACKSIZE;
        taskHandle = Task_create((Task_FuncPtr)tcpWorker, &taskParams, &eb);
        if (taskHandle == NULL) {
            System_printf("Error: Failed to create new Task\n");
            close(clientfd);
        }

        /* addrlen is a value-result param, must reset for next accept call */
        addrlen = sizeof(clientAddr);
    }

    System_printf("Error: accept failed.\n");

shutdown:
    if (server > 0) {
        close(server);
    }
    exitApp(ctx);
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

    /* CyaSSL library needs time() for validating certificates. */
    MYTIME_init();
    MYTIME_settime(CURRENTTIME);

    System_printf("Starting the TLS/TCP Echo example\nSystem provider is set "
                  "to SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
