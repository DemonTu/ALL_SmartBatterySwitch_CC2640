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
 *    ======== udpEchoHooksIPv6.c ========
 *    Contains non-BSD sockets code: NDK Hooks and IPv6 set up code.
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* NDK Header files */
#include <netmain.h>
#include <stkmain.h>

#define UDPPORT 1000
#define UDPHANDLERSTACK 1280

/* Prototypes */
Void echoFxn(UArg arg0, UArg arg1);

/*
 *  ======== IPv6DADStatus ========
 *  IPv6 initialization callback function
 */
static void IPv6DADStatus(IP6N Address, unsigned short dev_index,
        unsigned char Status)
{
    char strIPAddress[40];

    /* Convert the IP Address to String Format. */
    inet_ntop(AF_INET6, &Address, strIPAddress, 40);

    /* Print the status of the address. */
    System_printf("Address: %s on device %d is %s\n", strIPAddress, dev_index,
            (Status == 1) ? "UNIQUE" : "DUPLICATE");

    System_flush();

    return;
}

/*
 *  ======== netOpenHook ========
 *  NDK network open hook used to initialize IPv6
 */
void netOpenHook()
{
    Task_Handle taskHandle;
    Task_Params taskParams;
    Error_Block eb;
    int status;
    int dev_index = 1;

    /* Make sure Error_Block is initialized */
    Error_init(&eb);

    status = IPv6InterfaceInit(dev_index, IPv6DADStatus);
    if (status < 0) {
        System_printf("Error %d: failed to add IPv6 interface\n", status);
    }

    /*
     *  Create the Task that handles UDP "connections."
     *  arg0 will be the port that this task listens to. arg1 is the family,
     *  set for IPv6 in this example.
     */
    Task_Params_init(&taskParams);
    taskParams.stackSize = UDPHANDLERSTACK;
    taskParams.priority = 1;
    taskParams.arg0 = UDPPORT;
    taskParams.arg1 = AF_INET6;
    taskHandle = Task_create((Task_FuncPtr)echoFxn, &taskParams, &eb);
    if (taskHandle == NULL) {
        System_printf("Error: Failed to create udpHandler Task\n");
    }
}

/*
 *  ======== netCloseHook ========
 *  NDK network close hook used to de-initialize IPv6
 */
void netCloseHook()
{
    int status = 0;
    int dev_index = 1;

    /* Enter the kernel Mode. */
    llEnter ();
    status = IPv6InterfaceDeInit(dev_index);
    llExit ();

    /* Were we able to deinitialize the stack? */
    if (status < 0) {
        System_printf(
                "Error: Unable to de-initialize the IPv6 stack on device %d\n",
                dev_index);
    }
    else {
        System_printf("IPv6 stack has been deinitialized on %d\n", dev_index);
    }
}
