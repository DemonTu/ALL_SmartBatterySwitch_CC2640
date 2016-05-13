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

#include <xdc/runtime/Types.h>
#include <ti/sysbios/knl/Swi.h>

#include "package/internal/Seconds.xdc.h"


/* Definitions from CC3200 SDK */
#define HWREG(x)                (*((volatile unsigned long *)(x)))

#define HIB3P3_BASE                        0x4402F800
#define HIB3P3_O_MEM_HIB_RTC_TIMER_ENABLE  0x00000004
#define HIB3P3_O_MEM_HIB_RTC_TIMER_RESET   0x00000008
#define HIB3P3_O_MEM_HIB_RTC_TIMER_READ    0x0000000C
#define HIB3P3_O_MEM_HIB_RTC_TIMER_LSW     0x00000010
#define HIB3P3_O_MEM_HIB_RTC_TIMER_MSW     0x00000014
#define HIB3P3_O_MEM_INT_OSC_CONF          0x0000002C

#define ARCM_BASE                          0x44025000
#define APPS_RCM_O_APPS_RCM_INTERRUPT_STATUS 0x00000120


/*
 *  ======== Seconds_get ========
 */
UInt32 Seconds_get(Void)
{
    UInt32 curSeconds;
    UInt   key;

    /*
     *  Disable scheduling.  We use Swi_disable() instead of
     *  Hwi_disable() because of the large times for accessing
     *  these registers.
     */
    key = Swi_disable();

    curSeconds = Seconds_getSeconds() - Seconds_module->refSeconds +
        Seconds_module->setSeconds;

    /* Re-enable scheduling */
    Swi_restore(key);

    return (curSeconds);
}

/*
 *  ======== Seconds_set ========
 */
Void Seconds_set(UInt32 seconds)
{
    UInt32        status;
    UInt          key;

    /*
     *  Disable scheduling.  We use Swi_disable() instead of
     *  Hwi_disable() because of the large times for accessing
     *  these registers.
     */
    key = Swi_disable();

    /* Start the RTC counter the first time Seconds_set() is called. */
    if (Seconds_module->refSeconds == 0xffffffff) {
        /*
         *  Disable the timer. This is needed for reloading without having
         *  to power-cycle the board.
         */
        HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_ENABLE) = 0x0;

        /* Reset the timer */
        HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_RESET) = 1;

        HWREG(HIB3P3_BASE+HIB3P3_O_MEM_INT_OSC_CONF) = 0x00000101;

        /* Read the interrupt status to clear any pending interrupt */
        status = HWREG(ARCM_BASE + APPS_RCM_O_APPS_RCM_INTERRUPT_STATUS);
        (void)status;

        /* Enable the timer */
        HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_ENABLE) = 0x1;
    }

    Seconds_module->refSeconds = Seconds_getSeconds();
    Seconds_module->setSeconds = seconds;

    /* Re-enable scheduling */
    Swi_restore(key);
}

/*
 *  ======== Seconds_getSeconds ========
 *  Called with scheduling disabled.
 */
UInt32 Seconds_getSeconds()
{
    UInt32 curSeconds;
    UInt64 count;

    /* Latch the RTC vlaue */
    HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_READ) = 0x1;

    /* Read latched values as 2 32-bit vlaues */
    count = HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_MSW);
    count = count << 32;
    count |= HWREG(HIB3P3_BASE + HIB3P3_O_MEM_HIB_RTC_TIMER_LSW);

    /*
     *  The CC3200 timer has a frequency of 32768 (2 ** 15), so
     *  to get seconds, drop the lower 15 bits.
     */
    curSeconds = (UInt32)(count >> 15);

    return (curSeconds);
}
