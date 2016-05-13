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
 *  ======== loggerCircBuf.c ========
 *
 *  This testcase exercises CCS4's CPU Load and Task Load RTA GUIs.
 *  It also exercises the BIOS Execution GUI.
 *
 *  cpuLoadInit() gets the CPU frequency using an API and
 *  fills the loadValue arrays with load values corresponding to
 *  0, 25, 50, 75 and 95 percent cpuload.
 *
 *  timerFunc() (a Hwi thread) runs every 100ms and launches
 *  3 types of threads (Hwi, Swi, and Task).
 *  Each respective thread then performs a corresponding load
 *  function before relinquishing the CPU.
 *
 *  After dwelling for 5 seconds at each load setting, timerFunc()
 *  calls step() which advances to the next set of Hwi, Swi, and
 *  Task load values. The cycle repeats forever.
 *
 *  The loads of the 3 thread types can be seen separately using
 *  CCS's Task Load view, and cumulatively with the CPU Load view.
 *
 *  The BIOS Execution GUI depicts all the threads and transition
 *  events.
 */

#include <xdc/std.h>

#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/ILogger.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Diags.h>

#include <ti/uia/runtime/IUIATransfer.h>

#include <xdc/cfg/global.h>

Char buffer[1024];

/*
 *  ======== main ========
 */
Void main()
{
    Types_Timestamp64 startTimestamp;
    Types_Timestamp64 endTimestamp;
    Bool moreRecords = 0;
    UInt bytes;
    Int noWrap;
    Int noWrapAvg;
    Int i;

    /* Filler */
    for (i = 0; i < 70; i++) {
        Log_print1(Diags_USER1, "This is a write1 test &d\n", i);
    }

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Ten Log_print1 test mask not enable          */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    Timestamp_get64(&startTimestamp);
    for (i = 0; i < 10; i++) {
        Log_print1(Diags_USER2, "This is a write1 test &d\n", i);
    }
    Timestamp_get64(&endTimestamp);

    System_printf("Log_print1 x 10 times with mask not enable wrap is %d, average = %d\n",
                   endTimestamp.lo - startTimestamp.lo,
                   (endTimestamp.lo - startTimestamp.lo) / 10);

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Ten Log_print1 test                          */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    Timestamp_get64(&startTimestamp);
    for (i = 0; i < 10; i++) {
        Log_print1(Diags_USER1, "This is a write1 test &d\n", i);
    }
    Timestamp_get64(&endTimestamp);

    System_printf("Log_print1 x 10 times is %d, average = %d\n",
                   endTimestamp.lo - startTimestamp.lo,
                   (endTimestamp.lo - startTimestamp.lo) / 10);
    noWrap = endTimestamp.lo - startTimestamp.lo;
    noWrapAvg = (endTimestamp.lo - startTimestamp.lo) / 10;

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Read 128 bytes from logger                   */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    Timestamp_get64(&startTimestamp);
    for (i = 0; i < 10; i++) {
        moreRecords = IUIATransfer_getContents((IUIATransfer_Handle)logger, buffer, 128, &bytes);
    }
    Timestamp_get64(&endTimestamp);

    System_printf("getContents x 10 times of 128 bytes %d, average = %d\n",
                   endTimestamp.lo - startTimestamp.lo,
                   (endTimestamp.lo - startTimestamp.lo) / 10);

    if (moreRecords == FALSE) {
        System_printf("Potentially did not get a full buffer\n");
    }

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Ten Log_print1 with wrap                     */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    Timestamp_get64(&startTimestamp);
    for (i = 0; i < 10; i++) {
        Log_print1(Diags_USER1, "This is a write1 test &d\n", i);
    }
    Timestamp_get64(&endTimestamp);

    System_printf("Log_print1 x 10 times with wrap is %d, approx. time for wrap one = %d\n",
                   endTimestamp.lo - startTimestamp.lo,
                   (endTimestamp.lo - startTimestamp.lo) - noWrap + noWrapAvg);

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Read 1024 bytes from logger                  */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    ILogger_disable((ILogger_Handle)logger);
    IUIATransfer_reset((IUIATransfer_Handle)logger);
    ILogger_enable((ILogger_Handle)logger);

    /* Filler */
    for (i = 0; i < 70; i++) {
        Log_print1(Diags_USER1, "This is a write1 test &d\n", i);
    }

    Timestamp_get64(&startTimestamp);
    moreRecords = IUIATransfer_getContents((IUIATransfer_Handle)logger, buffer, 1024, &bytes);
    Timestamp_get64(&endTimestamp);

    System_printf("getContents x 1 times of 1024 bytes %d\n",
                   endTimestamp.lo - startTimestamp.lo);
    if (moreRecords == FALSE) {
        System_printf("Potentially did not get a full buffer\n");
    }
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Read 1024 bytes from logger with wrap        */
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Filler */
    moreRecords = IUIATransfer_getContents((IUIATransfer_Handle)logger, buffer, 128, &bytes);
    for (i = 0; i < 90; i++) {
        Log_print1(Diags_USER1, "This is a write1 test &d\n", i);
    }
    Timestamp_get64(&startTimestamp);
    moreRecords = IUIATransfer_getContents((IUIATransfer_Handle)logger, buffer, 1024, &bytes);
    Timestamp_get64(&endTimestamp);

    System_printf("getContents x 1 times with wrap of 1024 bytes %d\n",
                   endTimestamp.lo - startTimestamp.lo);

    if (moreRecords == FALSE) {
        System_printf("Potentially did not get a full buffer\n");
    }
    return;
}
