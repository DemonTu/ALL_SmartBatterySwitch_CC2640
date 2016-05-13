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
 *  ======== message.c ========
 *  Single-image message example on a multiprocessor system
 *
 *  This is an example program that uses MessageQ to pass a message
 *  from one processor to another.
 *
 *  Each processor creates its own MessageQ first and then will try to open
 *  a remote processor's MessageQ.
 */

#include <xdc/std.h>
#include <string.h>

/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>

/*  ----------------------------------- IPC module Headers           */
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MessageQ.h>
#include <ti/ipc/HeapBufMP.h>
#include <ti/ipc/MultiProc.h>

/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- UIA module Headers         */
#include <ti/uia/events/UIABenchmark.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>

#define HEAP_NAME   "myHeapBuf"
#define HEAPID      0
#define NUMLOOPS    10

Char localQueueName[6];
Char nextQueueName[6];

/*
 *  ======== tsk0_func ========
 *  This function is executed only on CORE0.
 *  It allocates a message and ping-pongs the message around the processors.
 *  A local message queue is created and a remote message queue is opened.
 *  Messages are sent to the remote message queue and retrieved from the
 *  local MessageQ.
 */
Void tsk0_func(UArg arg0, UArg arg1)
{
    MessageQ_Msg     msg;
    MessageQ_Handle  messageQ;
    MessageQ_QueueId remoteQueueId;
    Int              status;
    UInt16           msgId = 0;

    /* Create the local message queue */
    messageQ = MessageQ_create(localQueueName, NULL);
    if (messageQ == NULL) {
        System_abort("MessageQ_create failed\n" );
    }

    /* Open the remote message queue. Spin until it is ready. */
    do {
        status = MessageQ_open(nextQueueName, &remoteQueueId);
    } while (status < 0);

    /* Allocate a message to be ping-ponged around the processors */
    msg = MessageQ_alloc(HEAPID, sizeof(MessageQ_MsgHeader));
    if (msg == NULL) {
       System_abort("MessageQ_alloc failed\n" );
    }

    /*
     *  Send the message to the next processor and wait for a message
     *  from the previous processor.
     */
    while (TRUE) {
        /* Increment...the remote side will check this */
        msgId++;
        MessageQ_setMsgId(msg, msgId);

        Log_write1(UIABenchmark_start, (xdc_IArg)"Msg Round Trip");
        /* send the message to the remote processor */
        status = MessageQ_put(remoteQueueId, msg);
        if (status < 0) {
           System_abort("MessageQ_put had a failure/error\n");
        }

        /* Get a message */
        status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
        if (status < 0) {
           System_abort("This should not happen since timeout is forever\n");
        }
        Log_write1(UIABenchmark_stop, (xdc_IArg)"Msg Round Trip");

        if (MessageQ_getMsgId(msg) != msgId) {
            System_printf("The test failed MessageQ_getMsgId(msg) = %d, msgId = %d\n",
                          MessageQ_getMsgId(msg), msgId);
            System_exit(0);
        }

        /* To keep the number of events low until DVT performance is improved */
        Task_sleep(25);
    }
}

/*
 *  ======== tsk1_func ========
 *  This function is executed on every processor except on CORE0.
 *  It opens the heap created by CORE0 and registers it.
 *  A local message queue is created and a remote message queue is opened.
 *  Messages are sent to the remote message queue and retrieved from the
 *  local MessageQ.
 */
Void tsk1_func(UArg arg0, UArg arg1)
{
    MessageQ_Msg     msg;
    MessageQ_Handle  messageQ;
    MessageQ_QueueId remoteQueueId;
    HeapBufMP_Handle heapHandle;
    Int              status;

    /* Open the heap created by the other processor. Loop until opened. */
    do {
        status = HeapBufMP_open(HEAP_NAME, &heapHandle);
    } while (status < 0);

    /* Register this heap with MessageQ */
    MessageQ_registerHeap((IHeap_Handle)heapHandle, HEAPID);

    /* Create the local message queue */
    messageQ = MessageQ_create(localQueueName, NULL);
    if (messageQ == NULL) {
        System_abort("MessageQ_create failed\n" );
    }

    /* Open the remote message queue. Spin until it is ready. */
    do {
        status = MessageQ_open(nextQueueName, &remoteQueueId);
    } while (status < 0);

    /*
     *  Wait for a message from the previous processor and
     *  send it to the next processor
     */
    while (TRUE) {
        /* Get a message */
        status = MessageQ_get(messageQ, &msg, MessageQ_FOREVER);
        if (status < 0) {
           System_abort("This should not happen since timeout is forever\n");
        }

        /* send the message to the remote processor */
        status = MessageQ_put(remoteQueueId, msg);
        if (status < 0) {
           System_abort("MessageQ_put had a failure/error\n");
        }
    }
}

/*
 *  ======== main ========
 *  Synchronizes all processors.
 *  CORE0 creates a HeapBufMP and registers it with MessageQ.
 *  Create the Task to send and receive messages.
 */
Int main(Int argc, Char* argv[])
{
    HeapBufMP_Handle              heapHandle;
    HeapBufMP_Params              heapBufParams;

    System_printf("Start test\n");
    /*
     *  Ipc_start() calls Ipc_attach() to synchronize all remote processors
     *  because 'Ipc.procSync' is set to 'Ipc.ProcSync_ALL' in *.cfg
     */
    Ipc_start();

    if (MultiProc_self() == 0) {
        /*
         *  Create the heap that will be used to allocate messages.
         */
        HeapBufMP_Params_init(&heapBufParams);
        heapBufParams.regionId       = 0;
        heapBufParams.name           = HEAP_NAME;
        heapBufParams.numBlocks      = 1;
        heapBufParams.blockSize      = sizeof(MessageQ_MsgHeader);
        heapHandle = HeapBufMP_create(&heapBufParams);
        if (heapHandle == NULL) {
            System_abort("HeapBufMP_create failed\n" );
        }

        /* Register this heap with MessageQ */
        MessageQ_registerHeap((IHeap_Handle)heapHandle, HEAPID);
    }

    /* Generate queue names based on own proc ID and total number of procs */
    System_sprintf(localQueueName, "CORE%d", MultiProc_self());
    System_sprintf(nextQueueName, "CORE%d",
        ((MultiProc_self() + 1) % MultiProc_getNumProcessors()));

    /* Create a unique 'master' Task if on proc 0 */
    if (MultiProc_self() == 0) {
        Task_create(tsk0_func, NULL, NULL);
    }
    else {
        Task_create(tsk1_func, NULL, NULL);
    }

    System_printf("Starting BIOS\n");
    BIOS_start();
    return (0);
}
