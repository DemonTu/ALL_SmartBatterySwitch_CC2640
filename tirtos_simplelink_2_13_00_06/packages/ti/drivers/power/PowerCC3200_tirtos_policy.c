/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 *  ======== PowerCC3200_tirtos_policy.c ========
 */

#include <stdint.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>

/* driverlib header files */
#include <inc/hw_types.h>
#include <driverlib/prcm.h>
#include <driverlib/cpu.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

/* Clock object */
Clock_Struct clockObj;

/*
 *  ======== PowerCC3200_sleepPolicy ========
 */
void PowerCC3200_sleepPolicy()
{
    bool returnFromSleep = FALSE;
    uint32_t constraintMask;
    uint32_t ticks;
    uint64_t time;
    uint64_t match;
    uint64_t curr;
    uint64_t remain;
    uint32_t taskKey;
    uint32_t swiKey;

    /* disable interrupts */
    CPUcpsid();

    /* disable Swi and Task scheduling */
    swiKey = Swi_disable();
    taskKey = Task_disable();

    /* query the declared constraints */
    constraintMask = Power_getConstraintMask();

    /*
     *  Do not go into LPDS if not allowed into DEEPSLEEP.
     *  Check to see if we can go into LPDS (lowest level sleep).
     *  If not allowed, then attempt to go into DEEPSLEEP.
     *  If not allowed in DEEPSLEEP then just SLEEP.
     */

     /* check if we are allowed to go to LPDS */
    if ((constraintMask &
        ((1 << PowerCC3200_DISALLOW_LPDS) |
        (1 << PowerCC3200_DISALLOW_DEEPSLEEP))) == 0) {
        /*
         * Check how many ticks until the next scheduled wakeup.  A value of
         * zero indicates a wakeup will occur as the current Clock tick period
         * expires; a very large value indicates a very large number of Clock
         * tick periods will occur before the next scheduled wakeup.
         */
        /* Get the time remaining for the RTC timer to expire */
        ticks = Clock_getTicksUntilInterrupt();

        /* convert ticks to microseconds */
        time = ticks * Clock_tickPeriod;

        /* check if can go to LPDS */
        if (time > Power_getTransitionLatency(PowerCC3200_LPDS, Power_TOTAL)) {
            /* get the current and match values for RTC */
            match = MAP_PRCMSlowClkCtrMatchGet();
            curr = MAP_PRCMSlowClkCtrGet();
            remain = match - curr -
                (((uint64_t)PowerCC3200_TOTALTIMELPDS * 32768) / 1000000);

            /* set the LPDS wakeup time interval */
            MAP_PRCMLPDSIntervalSet(remain);

            /* enable the wake source to be timer */
            MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_TIMER);

            /* go to LPDS mode */
            Power_sleep(PowerCC3200_LPDS);

            /* set 'returnFromSleep' to TRUE*/
            returnFromSleep = TRUE;
        }
    }

    /* check if we are allowed to go to DEEPSLEEP */
    if ((constraintMask & (1 << PowerCC3200_DISALLOW_DEEPSLEEP) == 0) &&
        (!returnFromSleep)) {
        /*
         * Check how many ticks until the next scheduled wakeup.  A value of
         * zero indicates a wakeup will occur as the current Clock tick period
         * expires; a very large value indicates a very large number of Clock
         * tick periods will occur before the next scheduled wakeup.
         */
        ticks = Clock_getTicksUntilInterrupt();

        /* convert ticks to microseconds */
        time = ticks * Clock_tickPeriod;

        /* check if can go to DEEPSLEEP */
        if (time > Power_getTransitionLatency(PowerCC3200_DEEPSLEEP,
            Power_TOTAL)) {
            /* schedule the wakeup event */
            ticks -= PowerCC3200_RESUMETIMEDEEPSLEEP / Clock_tickPeriod;
            Clock_setTimeout(Clock_handle(&clockObj), ticks);
            Clock_start(Clock_handle(&clockObj));

            /* go to DEEPSLEEP mode */
            Power_sleep(PowerCC3200_DEEPSLEEP);
            Clock_stop(Clock_handle(&clockObj));

            /* set 'returnFromSleep' to TRUE so we don't go to sleep (below) */
            returnFromSleep = TRUE;
        }
    }

    /* re-enable interrupts */
    CPUcpsie();

    /* restore Swi scheduling */
    Swi_restore(swiKey);

    /* restore Task scheduling */
    Task_restore(taskKey);

    /* sleep only if we are not returning from one of the sleep modes above */
    if (!(returnFromSleep)) {
        MAP_PRCMSleepEnter();
    }
}

/*
 *  ======== PowerCC3200_dummyClockFunc ========
 */
Void PowerCC3200_dummyClockFunc(UArg arg)
{
}

/*
 *  ======== PowerCC3200_initPolicy ========
 */
void PowerCC3200_initPolicy()
{
    Clock_Params clockParams;

    /* construct Clock object */
    Clock_Params_init(&clockParams);
    clockParams.period = 0;
    clockParams.startFlag = FALSE;
    Clock_construct(&clockObj, &PowerCC3200_dummyClockFunc,
                    0, &clockParams);
}
