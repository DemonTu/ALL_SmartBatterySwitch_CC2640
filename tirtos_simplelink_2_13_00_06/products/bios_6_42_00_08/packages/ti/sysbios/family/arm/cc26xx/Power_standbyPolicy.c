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
 * EXEMPLARY, OR CONSEQueueNTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Power_standbyPolicy.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#include <ti/sysbios/knl/Clock.h>

/* driverlib header files */
#include <inc/hw_types.h>
#include <driverlib/prcm.h>
#include <driverlib/osc.h>
#include <driverlib/aon_wuc.h>
#include <driverlib/cpu.h>
#include <driverlib/sys_ctrl.h>

#include "package/internal/Power.xdc.h"

/* latency to allow for notifications */
#define NOTIFY_LATENCY     ( 1000 / Clock_tickPeriod )

/*
 *  ======== Power_standbyPolicy ========
 */
Void Power_standbyPolicy()
{
    Bool justIdle = TRUE;
    UInt32 constraints;
    UInt32 ticks;

    /* disable interrupts */
    CPUcpsid();

    /* query the declared constraints */
    constraints = Power_getConstraintInfo();

    /* do quick check to see if only WFI allowed; if yes, do it now */
    if ((constraints & (Power_SB_DISALLOW | Power_IDLE_PD_DISALLOW)) ==
                       (Power_SB_DISALLOW | Power_IDLE_PD_DISALLOW)) {
        PRCMSleep();
    }

    /*
     *  check if any sleep modes are allowed for automatic activation
     */

    else {

        /* check if we are allowed to go to standby */
        if ((constraints & Power_SB_DISALLOW) == 0) {

            /*
             * Check how many ticks until the next scheduled wakeup.  A value of
             * zero indicates a wakeup will occur as the current Clock tick
             * period expires; a very large value indicates a very large number
             * of Clock tick periods will occur before the next scheduled
             * wakeup.
             */
            ticks = Power_getTicksUntilWakeup();

            /* check if can go to STANDBY */
            if (ticks > Power_getTransitionLatency(Power_STANDBY,
                Power_TOTAL)) {

                /* schedule the wakeup event */
                ticks -= Power_wakeDelaySTANDBY / Clock_tickPeriod;
                Clock_setTimeout(Power_Module_State_clockObj(), ticks);
                Clock_start(Power_Module_State_clockObj());

                /* go to standby mode */
                Power_sleep(Power_STANDBY, NULL, NOTIFY_LATENCY);
                Clock_stop(Power_Module_State_clockObj());
                justIdle = FALSE;
            }
        }

        /* idle if allowed */
        if (justIdle) {

            /*
             * power off the CPU domain; VIMS will power down if SYSBUS is
             * powered down, and SYSBUS will power down if there are no
             * dependencies
             * NOTE: if radio driver is active it must force SYSBUS enable to
             * allow access to the bus and SRAM
             */
            if ((constraints & Power_IDLE_PD_DISALLOW) == 0) {

                if ((constraints & Power_NEED_FLASH_IN_IDLE) == 0) {
                    HWREG(PRCM_BASE + PRCM_O_PDCTL1VIMS) &= ~PRCM_PDCTL1VIMS_ON;
                }
                else {
                    HWREG(PRCM_BASE + PRCM_O_PDCTL1VIMS) |= PRCM_PDCTL1VIMS_ON;
                }

                PRCMCacheRetentionEnable();
                PRCMPowerDomainOff(PRCM_DOMAIN_CPU);
                SysCtrlAonSync();

                PRCMDeepSleep();

                /* make sure MCU and AON are in sync after wakeup */
                SysCtrlAonUpdate();
            }
            else {
                PRCMSleep();
            }
        }
    }

    /* re-enable interrupts */
    CPUcpsie();

    /* check operating conditions, optimally choose DCDC versus GLDO */
    SysCtrl_DCDC_VoltageConditionalControl();
}
