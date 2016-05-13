/*
 * Copyright (c) 2012, Texas Instruments Incorporated
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
 * */
/*
 *  ======== loader.c ========
 */

/* Standard headers */
#include <ti/syslink/Std.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Syslink headers */
#include <ti/syslink/utils/Trace.h>
#include <ti/syslink/utils/OsalPrint.h>
#include <ti/syslink/utils/String.h>
#include <ti/syslink/SysLink.h>
#include <ti/ipc/Notify.h>

/* App header */
#include "procMgrApp.h"

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */

#define MAXSLAVES 3

/* Must match the #define in the target source code */
#define SHUTDOWN 10

/** ============================================================================
 *  Globals
 *  ============================================================================
 */
Int     loader_numProcessors = 0;
String  loader_filePath[MAXSLAVES];
UInt16  loader_procId[MAXSLAVES];

/*!
 *  @brief  Function to execute the startup for loader sample application
 *
 *  @sa
 */
Int loader_startup (Void)
{
    Int i;
    Int status = 0;

    for (i = 0; i < loader_numProcessors; i++) {
        Osal_printf ("Loading and starting procId [%d] with [%s]\n",
                    loader_procId[i] ,
                    loader_filePath[i] );
        status = ProcMgrApp_startup (loader_procId[i],
                                    loader_filePath[i]);
        Osal_printf ("ProcMgrApp_startup status [%d]\n", status);
    }

    return (status);
}

/*
 *
 */
Int loader_execute (Void)
{
    Int status = 0;
    Char ch;

    Osal_printf ("\nOpen DVT and start getting events! [hit enter to shutdown the demo]\n");
    ch = getchar();

    return (status);
}

/*!
 *  @brief  Function to execute the shutdown for loader sample application
 *
 *  @sa
 */
Int loader_shutdown (Void)
{
    Int i;
    Int status = 0;

    for (i = 0; i < loader_numProcessors; i++) {
        status = Notify_sendEvent(loader_procId[i], 0, SHUTDOWN, 0, FALSE);
        Osal_printf ("Notify_sendEvent status [%d]\n", status);
    }

    for (i = 0; i < loader_numProcessors; i++) {
        /* shutdown the logic one by one for each processor */
        status = ProcMgrApp_shutdown(loader_procId[i]);
        Osal_printf ("ProcMgrApp_shutdown status [%d]\n", status);
    }

    return (status);
}

int main (int argc, char ** argv)
{
    Int status             = 0;
    Int i, j;

    /*
     *  Must be at least one set of procId and outfile. So argc cannot be odd
     */
    if ((argc < 3) || (argc / 2 * 2 == argc) || (argc > 7)) {
        Osal_printf ("uiaDaemon <procId> <outfile> [<procId> <outfile>] [<procId> <outfile>]\n");
        return (-1);
    }

    for (i = 0, j = 1 ; i < argc/2; i++, j = j + 2) {
        loader_procId[i] = strtol (argv [j], NULL, 16);
        loader_filePath[i] = argv [j + 1];
        loader_numProcessors++;
    }

    Osal_printf ("Running on %d processors\n", loader_numProcessors);

    Osal_printf ("Calling SysLink_setup\n");
    SysLink_setup ();

    Osal_printf ("Calling loader_startup\n");
    status = loader_startup ();

    Osal_printf ("Calling loader_execute\n");
    status = loader_execute ();

    Osal_printf ("Calling loader_shutdown\n");
    status = loader_shutdown ();

    Osal_printf ("Calling SysLink_destroy\n");
    SysLink_destroy ();

    return (0);
}

#if defined (__cplusplus)
extern "C" {
#endif /* defined (__cplusplus) */
