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
 *  ======== simpleTask.c ========
 *  This example exercises just runs a task every millisecond
 */

 /*  ----------------------------------- XDC module Headers           */
#include <xdc/std.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>

/*  ----------------------------------- BIOS module Headers           */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/ipc/Notify.h>

/*  ----------------------------------- UIA module Headers           */
#include <ti/uia/events/UIABenchmark.h>

#include <xdc/cfg/global.h>

/* Must match the #define in the target source code */
#define SHUTDOWN 10

/* Buffer that contains the bits to be reversed */
Char buffer[128];

Bool shutdownFlag = FALSE;

/*
 *  ======== main ========
 */
Void main()
{
    Int status = Ipc_S_SUCCESS;

    Log_print0(Diags_USER1, "stairstep example started.");

    /* Call Ipc_start() */
    do {
        status = Ipc_start();
    } while (status != Ipc_S_SUCCESS);

    BIOS_start();


    return;
}

/*
 *  ======== reverseBits ========
 *  Send and receive messages
 */
Void reverseBits(Char *buf, SizeT len)
{
    Int  i;
    Int  j;
    Char h;

    for (i = 0; i < len; i++) {

        for(h = j = 0; j < 8; j++) {
            h = (h << 1) + (buf[i] & 1);
            buf[i] >>= 1;
        }
    }

    return;
}

/*
 *  ======== clockFxn ========
 *  Runs every N ms (look in the simpleTask.cfg.xs file).
 */
Void clockFxn(UArg arg)
{
    /* Kick of Task thread */
    Semaphore_post(sem);
}

/*
 *  ======== notifyCallbackFxn ========
 */
Void notifyCallbackFxn (UInt16 procId, UInt16 lineId, UInt32 eventNo,
           UArg arg, UInt32 payload)
{
    Log_print0(Diags_USER1, "Received shutdown request");
    shutdownFlag = TRUE;
}

/*
 *  ======== taskLoad ========
 */
Void task(UArg arg1, UArg arg2)
{
    UInt count = 1;
    Int status = Ipc_S_SUCCESS;
    UInt16 remoteProcId = MultiProc_getId("HOST");

    do {
        status = Ipc_attach(remoteProcId);
    } while (status < 0);

    Notify_registerEvent(remoteProcId, 0, SHUTDOWN,
                         (Notify_FnNotifyCbck)notifyCallbackFxn, 0);


    while (shutdownFlag == FALSE) {
        Semaphore_pend(sem, BIOS_WAIT_FOREVER);

        /* Benchmark how long it takes to flip all the bits */
        Log_write1(UIABenchmark_start, (xdc_IArg)"Reverse");
        reverseBits(buffer, sizeof(buffer));
        Log_write1(UIABenchmark_stop, (xdc_IArg)"Reverse");

        Log_print1(Diags_USER1, "count = %d", count++);
    }

    /* Start shutdown process */
    Notify_unregisterEvent(remoteProcId, 0, SHUTDOWN,
                           (Notify_FnNotifyCbck)notifyCallbackFxn, 0);

    do {
        status = Ipc_detach(remoteProcId);
    } while (status < 0);

    Ipc_stop();
}
