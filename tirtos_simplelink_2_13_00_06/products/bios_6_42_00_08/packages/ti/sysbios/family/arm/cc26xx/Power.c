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
 *  ======== Power.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Startup.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Intrinsics.h>

#include "PowerCC2650.h"

/* driverlib header files */
#include <inc/hw_types.h>
#include <inc/hw_prcm.h>
#include <inc/hw_nvic.h>
#include <inc/hw_aon_wuc.h>
#include <inc/hw_aon_rtc.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ccfg.h>
#include <driverlib/sys_ctrl.h>
#include <driverlib/pwr_ctrl.h>
#include <driverlib/prcm.h>
#include <driverlib/aon_wuc.h>
#include <driverlib/aon_ioc.h>
#include <driverlib/aon_rtc.h>
#include <driverlib/aon_event.h>
#include <driverlib/aux_wuc.h>
#include <driverlib/osc.h>
#include <driverlib/cpu.h>
#include <driverlib/vims.h>
#include <driverlib/sys_ctrl.h>
#include <driverlib/driverlib_release.h>

#include "package/internal/Power.xdc.h"

/* bit defines for CUSTOMER_CFG_O_MODE_CONF:SCLK_LF_OPTION */
#define CLK_LF_RCOSC_LF 0xC00000
#define CLK_LF_XOSC_LF  0x800000
#define CLK_LF_XOSC_HF  0x000000

#define SUPPORT_DELAYED_COMPLETE 0

extern volatile UInt8 ti_sysbios_family_arm_cc26xx_Power_constraintCounts[
   Power_NUMCONSTRAINTS];

extern UInt ti_sysbios_family_arm_cc26xx_Power_XOSC_HF (UInt action);
extern Bool ti_sysbios_family_arm_cc26xx_Power_isDependencyActive(
    Power_Resource resourceID);

/*
 *  ======== Power_doWFI ========
 */
Void Power_doWFI()
{
    __asm(" wfi");
}

/*
 *  ======== Power_defaultClockFunc ========
 */
Void Power_defaultClockFunc(UArg arg)
{
}

/*
 *  ======== Power_defaultNotifyTrapFunc ========
 */
Void Power_defaultNotifyTrapFunc()
{
    while (1) {};
}

/*
 *  ======== Power_getClockHandle ========
 */
Clock_Handle Power_getClockHandle()
{
    return (Power_Module_State_clockObj());
}

/*
 *  ======== Power_LF_clockFunc ========
 */
Void Power_LF_clockFunc(UArg arg)
{
    UInt32 sourceLF;

     /* query LF clock source */
    sourceLF = OSCClockSourceGet(OSC_SRC_CLK_LF);

    /* is LF source either RCOSC_LF or XOSC_LF yet? */
    if ((sourceLF == OSC_RCOSC_LF) || (sourceLF == OSC_XOSC_LF)) {

        /* yes, disable the LF clock qualifiers */
        DDI16BitfieldWrite(
            AUX_DDI0_OSC_BASE,
            DDI_0_OSC_O_CTL0,
            DDI_0_OSC_CTL0_BYPASS_XOSC_LF_CLK_QUAL_M|
                DDI_0_OSC_CTL0_BYPASS_RCOSC_LF_CLK_QUAL_M,
            DDI_0_OSC_CTL0_BYPASS_RCOSC_LF_CLK_QUAL_S,
            0x3
        );

        /* now finish by releasing the standby disallow constraint */
        Power_releaseConstraint(Power_SB_DISALLOW);
    }

    /* not yet, LF still derived from HF, restart clock to check back later */
    else {
        /* retrigger LF Clock to fire in 100 msec */
        Clock_setTimeout(
            ti_sysbios_family_arm_cc26xx_Power_Module_State_lfClockObj(),
            (100000 / Clock_tickPeriod));
        Clock_start(
            ti_sysbios_family_arm_cc26xx_Power_Module_State_lfClockObj());
    }
}

/*
 *  ======== Power_Module_startup ========
 */
Int ti_sysbios_family_arm_cc26xx_Power_Module_startup(Int status)
{
    DRIVERLIB_ASSERT_CURR_RELEASE();

    /* enable cache (in case boot ROM doesn't because of HIB) */
    HWREG(VIMS_BASE + VIMS_O_CTL) = ((HWREG(VIMS_BASE + VIMS_O_CTL) &
        ~VIMS_CTL_MODE_M ) | VIMS_CTL_MODE_CACHE );

    /* force AUX on and enable OSC clock */
    OSCInterfaceEnable();

    /* source the HF clock from RC_OSC */
    OSCHF_SwitchToRcOscTurnOffXosc();

    /* when in sleep, power off JTAG */
    AONWUCJtagPowerOff();

    /* allow AUX to power down */
    AUXWUCPowerCtrl(AUX_WUC_POWER_DOWN);

    /* clear PDCTL1.VIMS_MODE to power VIMS only when CPU domain is powered */
    HWREG(PRCM_BASE + PRCM_O_PDCTL1VIMS) &= ~PRCM_PDCTL1VIMS_ON;

    /* sync with AON */
    SysCtrlAonSync();

    /* set standby disallow constraint pending LF clock quailifier disabling */
    Power_setConstraint(Power_SB_DISALLOW);

    return (Startup_DONE);
}

/*
 *  ======== Power_getConstraintInfo ========
 *  Get a bitmask indicating the constraints that have been registered with
 *  Power.
 */
UInt32 Power_getConstraintInfo(Void)
{
    return (Power_module->constraintsMask);
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 */
UInt32 Power_getDependencyCount(Power_Resource resourceID)
{
    return (ti_sysbios_family_arm_cc26xx_Power_refCount[resourceID]);
}

/*
 *  ======== Power_getTicksUntilWakeup ========
 *  Get the number of system ticks until the next scheduled wakeup event
 */
UInt32 Power_getTicksUntilWakeup(Void)
{
    return (Clock_getTicksUntilInterrupt());
}

/*
 *  ======== Power_getTransitionLatency ========
 *  Get the transition latency for a sleep state.  The latency is reported
 *  in units of Clock ticks.
 */
UInt32 Power_getTransitionLatency(Power_SleepState sleepState,
    Power_LatencyType type)
{
    UInt32 latency = 0;

    if (type == Power_RESUME) {
        if (sleepState == Power_STANDBY) {
            latency = Power_resumeTicksSTANDBY;
        }
    }
    else {
        if (sleepState == Power_STANDBY) {
            latency = Power_totalTicksSTANDBY;
        }
    }

    return (latency);
}

/*
 *  ======== Power_getTransitionState ========
 *  Get the current sleep transition state.
 */
Power_TransitionState Power_getTransitionState(Void)
{
    return (Power_module->state);
}

/*
 *  ======== Power_getXoscStartupTime ========
 *  Get the estimated crystal oscillator startup time
 */
UInt32 Power_getXoscStartupTime(UInt32 timeUntilWakeupInMs)
{
    return (OSCHF_GetStartupTime(timeUntilWakeupInMs));
}

/*
 *  ======== Power_isStableXOSC_HF ========
 *  Check if XOSC_HF has stabilized.
 */
Bool Power_isStableXOSC_HF(Void)
{
    Bool ready = TRUE;
    UInt key;

    key = Hwi_disable();

    /* only query if HF source is ready if there is a pending change */
    if (Power_module->xoscPending == TRUE) {
        if (OSCHfSourceReady()) {
            ready = TRUE;
        }
        else {
            ready = FALSE;
        }
    }

    Hwi_restore(key);

    return (ready);
}

/*
 *  ======== Power_releaseConstraint ========
 *  Release a previously declared constraint.
 */
Void Power_releaseConstraint(Power_Constraint constraint)
{
    UInt key;
    UInt8 count;

    key = Hwi_disable();

    count = ti_sysbios_family_arm_cc26xx_Power_constraintCounts[
        Intrinsics_maxbit(constraint)];

    Assert_isTrue(count != 0, Power_A_tooManyCallsReleaseConstraint);

    ti_sysbios_family_arm_cc26xx_Power_constraintCounts[
        Intrinsics_maxbit(constraint)] = count - 1;

    if (count == 1) {
        Power_module->constraintsMask &= ~constraint;
    }

    Hwi_restore(key);
}


/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency.
 */
Void Power_releaseDependency(Power_Resource resourceID)
{
    UInt8 parent;
    UInt8 count;
    UInt32 id;
    UInt key;

    /* disable interrupts */
    key = Hwi_disable();

    /* read and decrement the reference count */
    count = ti_sysbios_family_arm_cc26xx_Power_refCount[resourceID];

    Assert_isTrue(count != 0, Power_A_tooManyCallsReleaseDependency);

    ti_sysbios_family_arm_cc26xx_Power_refCount[resourceID] = count - 1;

    /* if this was the last dependency being released.., */
    if (count == 1) {

        /* deactivate this resource ... */
        id = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].driverlibID;

        /* is resource a peripheral?... */
        if (ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags
            & PERIPH) {
            PRCMPeripheralRunDisable(id);
            PRCMPeripheralSleepDisable(id);
            PRCMPeripheralDeepSleepDisable(id);
            PRCMLoadSet();
            while(!PRCMLoadGet()){};
        }

        /* else, does resource require a special handler?... */
        else if (ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags
            & SPECIAL) {

            /* call the special handler */
            ti_sysbios_family_arm_cc26xx_Power_resourceHandlers[id](DISABLE);
        }

        /* else resource is a power domain */
        else {
            PRCMPowerDomainOff(id);
            while (PRCMPowerDomainStatus(id) != PRCM_DOMAIN_POWER_OFF) {};
        }

        /* propagate release up the dependency tree ... */

        /* check for a first parent */
        parent = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags
          & PARENTMASK;

        /* if 1st parent, make recursive call to release that dependency */
        if (parent != NOPARENT) {
            Power_releaseDependency(parent);
        }

        /* check for a second parent */
        parent = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags2
          & PARENTMASK;

        /* if 2nd parent, make recursive call to release that dependency */
        if (parent != NOPARENT) {
            Power_releaseDependency(parent);
        }
    }

    /* re-enable interrupts */
    Hwi_restore(key);
}


/*
 *  ======== Power_registerNotify ========
 *  Register a function to be called on a specific power event.
 *
 */
Power_Status Power_registerNotify(Power_NotifyObj * pNotifyObj,
    UInt32 eventTypes, Fxn notifyFxn, UArg clientArg, UArg arg)
{
    /* check for NULL pointers  */
    if ((pNotifyObj == NULL) || (notifyFxn == NULL)) {
        return (Power_EINVALIDPOINTER);
    }

    /* fill in notify object elements */
    pNotifyObj->eventTypes = eventTypes;
    pNotifyObj->notifyFxn = notifyFxn;
    pNotifyObj->clientArg = clientArg;

    /* place notify object on event notification queue */
    Queue_put(Power_Module_State_notifyQ(), (Queue_Elem*) pNotifyObj);

    return (Power_SOK);
}


/*
 *  ======== Power_setDependency ========
 *  Declare a dependency upon a resource.
 */
Bool Power_setDependency(Power_Resource resourceID)
{
    UInt8 parent;
    UInt8 count;
    UInt32 id;
    UInt key;

    /* disable interrupts */
    key = Hwi_disable();

    /* read and increment reference count */
    count = ti_sysbios_family_arm_cc26xx_Power_refCount[resourceID]++;

    /* if resource was NOT activated previously ... */
    if (count == 0) {

        /* propagate set up the dependency tree ... */

        /* check for a first parent */
        parent = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags
          & PARENTMASK;

        /* if first parent, make recursive call to set that dependency */
        if (parent != NOPARENT) {
            Power_setDependency(parent);
        }

        /* check for a second parent */
        parent = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags2
          & PARENTMASK;

        /* if second parent, make recursive call to set that dependency */
        if (parent != NOPARENT) {
            Power_setDependency(parent);
        }

        /* now activate this resource ... */
        id = ti_sysbios_family_arm_cc26xx_Power_db[resourceID].driverlibID;

        /* is resource a peripheral?... */
        if (ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags & PERIPH) {
            PRCMPeripheralRunEnable(id);
            PRCMPeripheralSleepEnable(id);
            PRCMPeripheralDeepSleepEnable(id);
            PRCMLoadSet();
            while(!PRCMLoadGet()){};
        }

        /* else, does resource require a special handler?... */
        else if (ti_sysbios_family_arm_cc26xx_Power_db[resourceID].flags
            & SPECIAL) {

            /* call the special handler */
            ti_sysbios_family_arm_cc26xx_Power_resourceHandlers[id](ENABLE);
        }

        /* else resource is a power domain */
        else {
            PRCMPowerDomainOn(id);
            while (PRCMPowerDomainStatus(id) != PRCM_DOMAIN_POWER_ON) {};
        }
    }

    /* re-enable interrupts */
    Hwi_restore(key);

    /* return the resource's activation status */
    return (ti_sysbios_family_arm_cc26xx_Power_isDependencyActive(resourceID));
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint.
 */
Void Power_setConstraint(Power_Constraint constraint)
{
    UInt key;

    key = Hwi_disable();
    Power_module->constraintsMask |= constraint;
    ti_sysbios_family_arm_cc26xx_Power_constraintCounts[
        Intrinsics_maxbit(constraint)]++;
    Hwi_restore(key);
}

/*
 *  ======== Power_shutdown ========
 */
Power_Status Power_shutdown(UArg arg)
{
    Power_Status status = Power_EFAIL;
    Bool exitNow = FALSE;
    UInt32 constraints;
    UInt hwiKey;

    /* make sure shutdown request doesn't violate a constraint */
    constraints = Power_getConstraintInfo();
    if ((constraints & (Power_SD_DISALLOW)) != 0) {
        status = Power_ECHANGE_NOT_ALLOWED;
    }

    if (status == Power_EFAIL) {

        /* make sure Power is not still busy with a previous transition */
        hwiKey = Hwi_disable();

        if (Power_module->state == Power_ACTIVE) {
            /* set new transition state to entering shutdown */
            Power_module->state = Power_SHUTDOWN;
        }
        else {
            exitNow = TRUE;
        }

        Hwi_restore(hwiKey);

        if (exitNow == TRUE) {
            status = Power_EBUSY;
        }

        else {

            /* disable interrupts as start the shutdown sequence */
            Hwi_disable();

            /* signal all clients registered for pre-shutdown notification */
            status = Power_notify(Power_ENTERING_SHUTDOWN);

            /* check for any error */
            if (status != Power_SOK) {
                Power_module->state = Power_ACTIVE;
                CPUcpsie();
                return (status);
            }

            /* proceed with shutdown sequence ... */

            /* switch to RCOSC_HF and RCOSC_LF */
            OSCInterfaceEnable();
            if(OSCClockSourceGet(OSC_SRC_CLK_HF) != OSC_RCOSC_HF) {
                OSCClockSourceSet(OSC_SRC_CLK_HF | OSC_SRC_CLK_MF,
                    OSC_RCOSC_HF);
                while(!OSCHfSourceReady());
                OSCHfSourceSwitch();
            }
            OSCClockSourceSet(OSC_SRC_CLK_LF,OSC_RCOSC_LF);
            while(OSCClockSourceGet(OSC_SRC_CLK_LF) != OSC_RCOSC_LF);
            OSCInterfaceDisable();

            /* make sure DMA and CRYTO clocks are off in deep-sleep */
            PRCMPeripheralDeepSleepDisable(PRCM_PERIPH_CRYPTO);
            PRCMPeripheralDeepSleepDisable(PRCM_PERIPH_UDMA);
            PRCMLoadSet();
            while(!PRCMLoadGet()){};

            /* power OFF AUX and disconnect from bus */
            AUXWUCPowerCtrl(AUX_WUC_POWER_OFF);

            /* remove AUX force ON */
            HWREG(AON_WUC_BASE + AON_WUC_O_AUXCTL) &=
                ~AON_WUC_AUXCTL_AUX_FORCE_ON;

            /*
             * reset AON event source IDs to avoid pending events powering
             * on MCU/AUX
             */
            HWREG(AON_EVENT_BASE + AON_EVENT_O_MCUWUSEL) = 0x3F3F3F3F;
            HWREG(AON_EVENT_BASE + AON_EVENT_O_AUXWUSEL) = 0x003F3F3F;

            /* sync AON */
            HWREG(AON_RTC_BASE + AON_RTC_O_SYNC);

            /*
             * enable shutdown - this latches the IOs, so configuration of
             * IOCFGx registers must be done prior to this
             */
            AONWUCShutDownEnable();

            /* sync AON */
            HWREG(AON_RTC_BASE + AON_RTC_O_SYNC);

            /* wait until AUX powered off */
            while (AONWUCPowerStatusGet() & AONWUC_AUX_POWER_ON);

            /* request to power off MCU when go to deep sleep */
            PRCMMcuPowerOff();

            /* turn off power domains inside MCU VD (BUS, FL_BUS, RFC, CPU) */
            PRCMPowerDomainOff(PRCM_DOMAIN_RFCORE | PRCM_DOMAIN_SERIAL |
                PRCM_DOMAIN_PERIPH | PRCM_DOMAIN_CPU | PRCM_DOMAIN_VIMS);

            /* deep sleep to activate shutdown */
            PRCMDeepSleep();
        }
    }

    Power_module->state = Power_ACTIVE;

    /* if get here failed to shutdown, return failure code */
    return (Power_EFAIL);
}


/*
 *  ======== Power_sleep ========
 */
Power_Status Power_sleep(Power_SleepState sleepState, UArg arg0, UArg arg1)
{
    Power_Status status = Power_SOK;
    UInt xosc_hf_active = FALSE;
    Power_Event postEventLate;
    UInt32 poweredDomains = 0;
    Bool exitNow = FALSE;
    Power_Event preEvent;
    Power_Event postEvent;
    UInt32 constraints;
    Bool retainCache = FALSE;
    UInt32 modeVIMS;
    UInt taskKey;
    UInt swiKey;
    UInt hwiKey;

    /* first validate the sleep code */
    if ( sleepState != Power_STANDBY) {
        status = Power_EFAIL;
    }

    if (status == Power_SOK) {

        /* make sure Power is not still busy with a previous transition */
        hwiKey = Hwi_disable();

        if (Power_module->state == Power_ACTIVE) {

            /* set transition state to entering sleep */
            Power_module->state = Power_ENTERING_SLEEP;
        }
        else {
            exitNow = TRUE;
        }

        Hwi_restore(hwiKey);

        if (exitNow == TRUE) {
            status = Power_EBUSY;
        }

        else {

            /* setup sleep vars */
            if (sleepState == Power_STANDBY) {
                preEvent = Power_ENTERING_STANDBY;
                postEvent = Power_AWAKE_STANDBY;
                postEventLate = Power_AWAKE_STANDBY_LATE;
            }

            /* disable Task scheduling; allow Swis and Hwis for notifications */
            taskKey = Task_disable();

            /* signal all clients registered for pre-sleep notification */
            status = Power_notify(preEvent);

            /* check for any error */
            if (status != Power_SOK) {
                Power_module->state = Power_ACTIVE;
                Task_restore(taskKey);          /* re-enable scheduler */
                return (status);
            }

            /* now disable Swi scheduling */
            swiKey = Swi_disable();

            /* freeze the IOs on the boundary between MCU and AON */
            AONIOCFreezeEnable();

            /* if XOSC_HF is active, force it off */
            if(OSCClockSourceGet(OSC_SRC_CLK_HF) == OSC_XOSC_HF) {
                xosc_hf_active = TRUE;
                ti_sysbios_family_arm_cc26xx_Power_XOSC_HF(DISABLE);
            }

            /* allow AUX to power down */
            AONWUCAuxWakeupEvent(AONWUC_AUX_ALLOW_SLEEP);

            /* make sure writes take effect */
            SysCtrlAonSync();

            /* invoke specific sequences to activate sleep states... */

            if (sleepState == Power_STANDBY) {

                /* query and save domain states before powering them off */
                if (Power_getDependencyCount(DOMAIN_RFCORE)) {
                    poweredDomains |= PRCM_DOMAIN_RFCORE;
                }
                if (Power_getDependencyCount(DOMAIN_SERIAL)){
                    poweredDomains |= PRCM_DOMAIN_SERIAL;
                }
                if (Power_getDependencyCount(DOMAIN_PERIPH)) {
                    poweredDomains |= PRCM_DOMAIN_PERIPH;
                }

                /* gate running deep sleep clocks for Crypto and DMA */
                if (Power_getDependencyCount(PERIPH_CRYPTO)) {
                    PRCMPeripheralDeepSleepDisable(
                        ti_sysbios_family_arm_cc26xx_Power_db[
                            PERIPH_CRYPTO].driverlibID);
                }
                if (Power_getDependencyCount(PERIPH_UDMA)) {
                    PRCMPeripheralDeepSleepDisable(
                        ti_sysbios_family_arm_cc26xx_Power_db[
                            PERIPH_UDMA].driverlibID);
                }
                /* make sure clock settings take effect */
                PRCMLoadSet();

                /* request power off of domains in the MCU voltage domain */
                PRCMPowerDomainOff(poweredDomains | PRCM_DOMAIN_CPU);

                /* request uLDO during standby */
                PRCMMcuUldoConfigure(true);

                /* query constraints to determine if cache should be retained */
                constraints = Power_getConstraintInfo();
                if ((constraints & Power_SB_VIMS_CACHE_RETAIN) != 0) {
                    retainCache = TRUE;
                }

                /* if don't want retention in standby, disable it now ... */
                if (retainCache == FALSE) {
                    modeVIMS = VIMSModeGet(VIMS_BASE);
                    /* wait if invalidate in progress... */
                    while (modeVIMS == VIMS_MODE_CHANGING) {
                        modeVIMS = VIMSModeGet(VIMS_BASE);
                    }
                    PRCMCacheRetentionDisable();
                    VIMSModeSet(VIMS_BASE, VIMS_MODE_OFF);
                }

                /* setup recharge parameters */
                SysCtrlSetRechargeBeforePowerDown(XoscInHighPowerMode);

                /* make sure all writes have taken effect */
                SysCtrlAonSync();

                /* invoke deep sleep to go to STANDBY */
                PRCMDeepSleep();

                /* if didn't retain cache in standby, re-enable retention now */
                if (retainCache == FALSE) {
                    VIMSModeSet(VIMS_BASE, modeVIMS);
                    PRCMCacheRetentionEnable();
                }

                /* force power on of AUX to keep it on when system is not
                 * sleeping; this also counts as a write to the AON interface
                 * ensuring that a following sync of the AON interface will
                 * force an update of all registers
                 */
                AONWUCAuxWakeupEvent(AONWUC_AUX_WAKEUP);
                while(!(AONWUCPowerStatusGet() & AONWUC_AUX_POWER_ON)) {};

                /* if XOSC_HF was forced off above, initiate switch back */
                if (xosc_hf_active == TRUE) {
                    ti_sysbios_family_arm_cc26xx_Power_XOSC_HF(ENABLE);
                }

                /* restore power domain states in effect before standby */
                PRCMPowerDomainOn(poweredDomains);
                while (PRCMPowerDomainStatus(poweredDomains) !=
                    PRCM_DOMAIN_POWER_ON){};

                /* restore deep sleep clocks of Crypto and DMA */
                if (Power_getDependencyCount(PERIPH_CRYPTO)) {
                    PRCMPeripheralDeepSleepEnable(
                        ti_sysbios_family_arm_cc26xx_Power_db[
                            PERIPH_CRYPTO].driverlibID);
                }
                if (Power_getDependencyCount(PERIPH_UDMA)) {
                    PRCMPeripheralDeepSleepEnable(
                        ti_sysbios_family_arm_cc26xx_Power_db[
                            PERIPH_UDMA].driverlibID);
                }
                /* make sure clock settings take effect */
                PRCMLoadSet();
            }

            /* release request for uLDO */
            PRCMMcuUldoConfigure(false);

            /* set transition state to EXITING_SLEEP */
            Power_module->state = Power_EXITING_SLEEP;

            /*
             * signal clients registered for early post-sleep notification;
             * this should be used to initialize any timing critical or IO
             * dependent hardware
             */
            status = Power_notify(postEvent);

            /* disable IO freeze and ensure RTC shadow value is updated */
            AONIOCFreezeDisable();
            SysCtrlAonSync();

            /* re-enable interrupts */
            CPUcpsie();

            /* signal all clients registered for late post-sleep notification */
            status = Power_notify(postEventLate);

            /* now clear the transition state before re-enabling scheduler */
            Power_module->state = Power_ACTIVE;

            /* re-enable Swi scheduling */
            Swi_restore(swiKey);

            /* adjust recharge parameters */
            SysCtrlAdjustRechargeAfterPowerDown();

            /* re-enable Task scheduling */
            Task_restore(taskKey);

            /* check for any notification error */
            if (status != Power_SOK) {
                return (status);
            }
        }
    }

    return (status);
}

/*
 *  ======== Power_switchXOSC_HF ========
 *  Switch to enable XOSC_HF.
 *
 */
Void Power_switchXOSC_HF(Void)
{
    UInt key;

    key = Hwi_disable();

    /* only switch source if there is a pending change */
    if (Power_module->xoscPending == TRUE) {
        OSCHF_AttemptToSwitchToXosc();
        Power_module->xoscPending = FALSE;
    }

    Hwi_restore(key);
}

/*
 *  ======== Power_unregisterNotify ========
 *  Unregister for a power notification.
 *
 */
Void Power_unregisterNotify(Power_NotifyObj * pNotifyObj)
{
    UInt key;

    /* remove notify object from its event queue */
    key = Hwi_disable();
    Queue_remove((Queue_Elem *)pNotifyObj);
    Hwi_restore(key);
}


/* * * * * * * * * * * * internal support functions * * * * * * * * * * * */


/*
 *  ======== Power_notify ========
 *
 *  Note: When this function is called Task scheduling is disabled.
 */
Power_Status Power_notify(Power_Event eventType)
{
    Power_Status returnStatus = Power_SOK;
    Power_NotifyResponse notifyStatus;
    Queue_Handle notifyQ;

    notifyQ = Power_Module_State_notifyQ();

    /* if notification queue is not empty, service the queue */
    if (!Queue_empty(notifyQ)) {

       /* service the notification queue... */
       notifyStatus = Power_serviceNotifyQ(eventType);

       /* if notifications did not complete successfully ... */
        if (notifyStatus != Power_NOTIFYDONE) {

            /* call the configured notify trap function */
            (*(Power_FuncPtr)Power_notifyTrapFunc)();

            returnStatus = Power_EFAIL;
        }
    }

    return (returnStatus);
}

/*
 *  ======== Power_serviceNotifyQ ========
 *
 */
Power_NotifyResponse Power_serviceNotifyQ(Power_Event eventType)
{
    Power_NotifyResponse returnStatus = Power_NOTIFYDONE;
    Power_NotifyResponse clientStatus;
    Queue_Handle notifyQ;
    Queue_Elem * elem;
    Fxn notifyFxn;
    UArg clientArg;

    notifyQ = Power_Module_State_notifyQ();

    /* point to first client notify object */
    elem = Queue_head(notifyQ);

    /* walk the queue and notify each registered client of the event */
    do {
        if (((Power_NotifyObj *)elem)->eventTypes & eventType) {

            /* pull params from notify object */
            notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
            clientArg = ((Power_NotifyObj *)elem)->clientArg;

            /* call the client's notification function */
            clientStatus = (Power_NotifyResponse) (*(Fxn)notifyFxn)(
                eventType, clientArg);

            /* if client declared error stop all further notifications */
            if (clientStatus == Power_NOTIFYERROR) {
                return (Power_NOTIFYERROR);
            }
        }

        /* get next element in the notification queue */
        elem = Queue_next(elem);

    } while (elem != (Queue_Elem *) notifyQ);

    return (returnStatus);
}
