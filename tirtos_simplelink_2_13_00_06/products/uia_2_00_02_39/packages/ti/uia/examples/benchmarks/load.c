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
 *  ======== load.c ========
 */

#include <xdc/std.h>
#include <string.h>

/*  -----------------------------------XDC.RUNTIME module Headers    */
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Diags.h>

/*  ----------------------------------- BIOS6 module Headers         */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

/*  ----------------------------------- UIA module Headers         */
#include <ti/uia/events/UIABenchmark.h>

/*  ----------------------------------- To get globals from .cfg Header */
#include <xdc/cfg/global.h>

/*
 *  ======== taskLoad ========
 */
Void taskLoad(Void)
{
    Bool flag;
    Types_Timestamp64 startTime;
    Types_Timestamp64 currentTime;
    Types_FreqHz freq;
    UInt32 count;
    Int loops;

    /* Have this task use ~50% of the CPU */
    Timestamp_getFreq(&freq);
    count = freq.lo / 1000 / 1000 * (Clock_tickPeriod/ 2);

    while (TRUE) {
        Semaphore_pend(loadSem, BIOS_WAIT_FOREVER);

        Log_write1(UIABenchmark_start, (xdc_IArg)"running");
        Timestamp_get64(&startTime);

        flag = TRUE;
        loops = 0;

        while (flag == TRUE) {
            Timestamp_get64(&currentTime);

            loops++;

            // TODO deal with wrap
            if (startTime.lo + count <= currentTime.lo) {
                flag = FALSE;
                Log_write1(UIABenchmark_stop, (xdc_IArg)"running");
                Log_write1(UIABenchmark_stop, (xdc_IArg)"whole");
            }
        }
    }
}

/*
 *  ======== releaseTaskLoad ========
 */
Void releaseTaskLoad(UArg arg0)
{
    Log_write1(UIABenchmark_start, (xdc_IArg)"whole");
    Semaphore_post(loadSem);
}

/*
 *  ======== main ========
 */
Int main(Int argc, Char* argv[])
{
    BIOS_start();
    return (0);
}
