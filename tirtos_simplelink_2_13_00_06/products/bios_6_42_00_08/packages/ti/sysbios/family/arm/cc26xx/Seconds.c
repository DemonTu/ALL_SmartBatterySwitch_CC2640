/*
 * Copyright (c) 2014, Texas Instruments Incorporated
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
 *  ======== Seconds.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/hal/Hwi.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_aon_rtc.h>
#include <inc/hw_ints.h>
#include <driverlib/aon_rtc.h>
#include <driverlib/aon_event.h>
#include <driverlib/interrupt.h>

#include "package/internal/Seconds.xdc.h"

volatile UInt32 curShadow;

/*
 *  ======== Seconds_get ========
 */
UInt32 Seconds_get(Void)
{
    volatile UInt32 curSeconds;
    UInt            key;

    key = Hwi_disable();

    curSeconds = HWREG(AON_RTC_BASE + AON_RTC_O_SEC);

    curSeconds = (curSeconds - Seconds_module->refSeconds) +
        Seconds_module->setSeconds;

    Hwi_restore(key);

    return (curSeconds);
}

/*
 *  ======== Seconds_set ========
 */
Void Seconds_set(UInt32 seconds)
{
    UInt            key;

    key = Hwi_disable();

    Seconds_module->refSeconds = HWREG(AON_RTC_BASE + AON_RTC_O_SEC);

    Seconds_module->setSeconds = seconds;

    Hwi_restore(key);
}
