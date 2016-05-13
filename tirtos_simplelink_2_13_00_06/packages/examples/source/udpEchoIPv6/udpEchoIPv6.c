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
 *    ======== udpEcho.c ========
 *    Contains BSD sockets code.
 */

#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

#include <stdio.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>

/* NDK BSD support */
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

/* Example/Board Header file */
#include "Board.h"

#define UDPPACKETSIZE 256

/*
 *  ======== echoFxn ========
 *  Echoes UDP messages.
 *
 */
Void echoFxn(UArg arg0, UArg arg1)
{
    int                     bytesRcvd;
    int                     bytesSent;
    int                     status;
    int                     server;
    char                    port[8];
    fd_set                  readSet;
    struct addrinfo        *results = NULL;
    struct addrinfo        *currAddr = NULL;
    struct addrinfo         hints;
    struct sockaddr_storage clientAddr;
    socklen_t               addrlen;
    char                    buffer[UDPPACKETSIZE];

    /* Initialize local variables */
    server = -1;
    memset(&hints, 0, sizeof(hints));
    memset(&clientAddr, 0, sizeof(clientAddr));

    /*
     *  Use getaddrinfo for generic socket set up code. The family (AF_INET or
     *  AF_INET6) is passed in via arg1 and dictates whether the socket is an
     *  IPv4 or IPv6 socket.
     */
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = (int)arg1;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    sprintf(port, "%d", arg0);
    status = getaddrinfo(NULL, port, &hints, &results);
    if (status != 0) {
        System_printf("Error: getaddrinfo failed (%d).\n", status);
        return;
    }

    /*
     *  Cycle through the list of address structs returned from getaddrinfo,
     *  trying each until we succeed:
     */
    for (currAddr = results; currAddr != NULL; currAddr = currAddr->ai_next) {
        server = socket(currAddr->ai_family, currAddr->ai_socktype,
                currAddr->ai_protocol);
        if (server == -1) {
            System_printf("Error: socket not created.\n");
            goto nextaddr;
        }

        status = bind(server, currAddr->ai_addr, currAddr->ai_addrlen);
        if (status == -1) {
            System_printf("Error: bind failed.\n");
            goto nextaddr;
        }

        /* We have a valid socket set up */
        break;

nextaddr:
        if (server > 0) {
            close(server);
        }
        server = -1;
    }

    freeaddrinfo(results);

    if (server == -1) {
        System_printf("Error: could not create a valid socket\n");
        goto shutdown;
    }

    do {
        /*
         *  readSet and addrlen are value-result arguments, which must be reset
         *  in between each select() and recvfrom() call
         */
        FD_ZERO(&readSet);
        FD_SET(server, &readSet);
        addrlen = sizeof(clientAddr);

        /* Wait forever for the reply */
        status = select(server + 1, &readSet, NULL, NULL, NULL);
        if (status > 0) {
            if (FD_ISSET(server, &readSet)) {
                bytesRcvd = recvfrom(server, buffer, UDPPACKETSIZE, 0,
                        (struct sockaddr *)&clientAddr, &addrlen);

                if (bytesRcvd > 0) {
                    bytesSent = sendto(server, buffer, bytesRcvd, 0,
                            (struct sockaddr *)&clientAddr, addrlen);
                    if (bytesSent < 0 || bytesSent != bytesRcvd) {
                        System_printf("Error: sendto failed.\n");
                        goto shutdown;
                    }
                }
            }
        }
    } while (status > 0);

shutdown:
    if (server > 0) {
        close(server);
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

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the UDP Echo example\nSystem provider is set to "
                  "SysMin. Halt the target to view any SysMin contents in"
                  " ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
