/*
 * Copyright (c) 2013-2014, Texas Instruments Incorporated
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
 *
 */

/*
 *  ======== loggerStreamer2.c ========
 *  Simple example showing usage of LoggerStreamer2.  Two tasks are running,
 *  each logging to its own LoggerStreamer2 log.
 */

#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/uia/runtime/UIAPacket.h>
#include <ti/uia/runtime/LogUC.h>

#include <ti/uia/sysbios/LoggerStreamer2.h>

#include "TestEvents.h"

#ifdef xdc_target__isaCompatible_64P
#include <c6x.h>
#else
#define DNUM 0
#endif

#ifdef xdc_target__bigEndian
#define ENDIANNESS 1
#else
#define ENDIANNESS 0
#endif

#define NUMBUFS 2

/* include declarations of Logger instances */
#include <xdc/cfg/global.h>

#define BUFSIZE (sizeof(Int32) * 32)

static Void initBuffer(Char *buf, Int size);

volatile Int count = 0;

Char buffer0[BUFSIZE];
Char buffer1[BUFSIZE];

/*
 *  ======== main ========
 */
Int main(Int argc, String argv[])
{
    System_printf("main: Starting LoggerStreamer2 example\n");

    BIOS_start();

    return (0);
}

/*
 *  ======== prime ========
 */
Ptr prime(LoggerStreamer2_Object *log)
{
    UInt32 id = (UInt32)LoggerStreamer2_getContext(log);

    if (id > 1) {
        System_printf("Context = %d\n", id);
        return (NULL);
    }

    /*
     *  Calling LoggerStreamer2_initBuffer() here before object is
     *  completely initialized.
     */
    if (id == 0) {
        initBuffer(buffer0, BUFSIZE);
        LoggerStreamer2_initBuffer(logger0, buffer0, DNUM);
        return ((Ptr)(buffer0));
    }
    if (id == 1) {
        initBuffer(buffer1, BUFSIZE);
        LoggerStreamer2_initBuffer(logger1, buffer1, DNUM);
        return ((Ptr)(buffer1));
    }

    return (NULL);
}

/*
 *  ======== exchange ========
 * Called when LoggerStream is full
 */
Ptr exchange(LoggerStreamer2_Object *log, Ptr full)
{
    UInt32 id = (UInt32)LoggerStreamer2_getContext(log);
    Int i, len;
    UInt32 *wordPtr = (UInt32 *)full;

    if (id >= NUMBUFS) {
        return (NULL);
    }

    /* Print the UIA packet */
    if (ENDIANNESS) {
        System_printf("UIA Packet Header: 0x%.8lx 0x%.8lx 0x%.8lx 0x%.8lx\r\n",
                (ULong)wordPtr[0], (ULong)wordPtr[1],
                (ULong)wordPtr[2], (ULong)wordPtr[3]);
    }
    else {
        System_printf("UIA Packet Header: 0x%.8lx 0x%.8lx 0x%.8lx 0x%.8lx\r\n",
                (ULong)UIAPacket_swizzle(wordPtr[0]),
                (ULong)UIAPacket_swizzle(wordPtr[1]),
                (ULong)UIAPacket_swizzle(wordPtr[2]),
                (ULong)UIAPacket_swizzle(wordPtr[3]));
    }

    /* Print the UIA packet body */
    len = UIAPacket_getLength((UIAPacket_Hdr *)wordPtr);
    wordPtr += 4;
    for (i = 4; i < (len / 4); i++) {
        System_printf("\t0x%.8lx\n", *wordPtr++);
    }
    System_flush();

    /* If there is still space in the buffer print the invalid packet */
    if (len < BUFSIZE) {
        if (ENDIANNESS) {
            System_printf("UIA Invalid Packet Header: 0x%.8lx\n", *wordPtr);
        }
        else {
            System_printf("UIA Invalid Packet Header: 0x%.8lx\n",
                    UIAPacket_swizzle(*wordPtr));
        }
        len = UIAPacket_getLength((UIAPacket_Hdr *)wordPtr++);
        for (i = 1; i < (len / 4); i++) {
            System_printf("\t0x%.8lx\n", *wordPtr++);
        }
    }

    System_flush();

    /*
     *  Clear all but the UIAPacket header.  This is not necessary, but
     *  may be useful for debugging.
     */
    initBuffer((Char *)full + sizeof(UIAPacket_Hdr),
            BUFSIZE - sizeof(UIAPacket_Hdr));

    return (full);
}

/*
 *  ======== tsk0Fxn ========
 *  Statically created task
 */
Void tsk0Fxn(UArg arg0, UArg arg1)
{
    Int num = 0;

    /* To demonstrate the exchange function */
    while (TRUE) {
        Log_iwriteUC1(logger0, LoggerStreamer2_L_test, num++);
        Task_sleep(1);
    }
}

/*
 *  ======== tsk1Fxn ========
 *  Statically created task
 */
Void tsk1Fxn(UArg arg0, UArg arg1)
{
    Int num = 0;

    /* To demonstrate the exchange function */
    while (TRUE) {
        Log_iprintUC1(logger1, "Num = %d", num++);
        Task_sleep(1);
    }
}

/*
 *  ======== initBuffer ========
 */
static Void initBuffer(Char *buf, Int size)
{
    Int i;

    for (i = 0; i < size; i++) {
        if (sizeof(Char) == sizeof(UInt16)) {
            buf[i] = 0xffff;
        }
        else {
            buf[i] = 0xff;
        }
    }
}
