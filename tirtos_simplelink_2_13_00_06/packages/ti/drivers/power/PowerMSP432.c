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
 *  ======== PowerMSP432.c ========
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif
#include <ti/drivers/ports/DebugP.h>
#include <ti/drivers/ports/HwiP.h>
#include <ti/drivers/ports/ListP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

/* driverlib header files */
#include <flash.h>
#include <wdt_a.h>
#include <rtc_c.h>

/* Active states */
#define AM_DCDC_VCORE0        PCM_AM_DCDC_VCORE0
#define AM_DCDC_VCORE1        PCM_AM_DCDC_VCORE1
#define AM_LDO_VCORE0         PCM_AM_LDO_VCORE0
#define AM_LDO_VCORE1         PCM_AM_LDO_VCORE1
#define AM_LF_VCORE0          PCM_AM_LPR_VCORE0
#define AM_LF_VCORE1          PCM_AM_LPR_VCORE1

/* externs */
extern const PowerMSP432_Config PowerMSP432_config;
extern void PowerMSP432_schedulerDisable(void);
extern void PowerMSP432_schedulerRestore(void);
extern void PowerMSP432_updateFreqs(PowerMSP432_Freqs *freqs);

/* internal functions */
static unsigned int notify(unsigned int eventType, uintptr_t eventArg);
static bool initPerfControl(unsigned int initLevel);

/* ModuleState */
PowerMSP432_ModuleState PowerMSP432_module = {
    .notifyList = { NULL, NULL},
    .constraintMask = 0,
    .state = Power_ACTIVE,
    .currentPerfLevel = 0,
    .enablePolicy = false,
    .perfInitialized = false,
    .constraintCounts = { 0 }
};

/*
 * Notes for initial implementation of Power_setPerformanceLevel()
 *
 * 1) There are three performance levels supported:
 *
 *    Level    MCLK (MHz)    HSMCLK (MHz)    SMCLK (MHz)    ACLK (Hz)
 *    -----    ----------    ------------    -----------    ---------
 *      0         12              3              3           32768
 *      1         24              6              6           32768
 *      2         48             24             12           32768
 *
 *    Only three of the peformance level constraints are supported:
 *
 *        PowerMSP432_DISALLOW_PERFLEVEL_0
 *        PowerMSP432_DISALLOW_PERFLEVEL_1
 *        PowerMSP432_DISALLOW_PERFLEVEL_2
 *
 * 2) DCO is the only supported clock source (all clocks are derived from this;
 *    LF and HF XTALs are not used)
 *
 * 3) ACLK is fixed (and not included in the PowerMSP432_PerfLevel structure)
 *
 * 4) DCDC is assumed to be available
 *
 * 5) Hardware interrupts are disabled during the change of performance level
 *
 */

#define NUMPERFLEVELS    3

PowerMSP432_PerfLevel PowerMSP432_perfLevels[NUMPERFLEVELS] = {
    { .activeState = AM_DCDC_VCORE0,
      .VCORE = 0,
      .clockSource = CS_DCOCLK_SELECT,
      .DCORESEL = CS_DCO_FREQUENCY_12,
      .DIVM = CS_CLOCK_DIVIDER_1,
      .DIVHS = CS_CLOCK_DIVIDER_4,
      .DIVS = CS_CLOCK_DIVIDER_4,
      .flashWaitStates = 0,
      .enableFlashBuffer = false,
      .MCLK = 12000000,
      .HSMCLK = 3000000,
      .SMCLK = 3000000,
      .ACLK = 32768
     },
    { .activeState = AM_DCDC_VCORE0,
      .VCORE = 0,
      .clockSource = CS_DCOCLK_SELECT,
      .DCORESEL = CS_DCO_FREQUENCY_24,
      .DIVM = CS_CLOCK_DIVIDER_1,
      .DIVHS = CS_CLOCK_DIVIDER_4,
      .DIVS = CS_CLOCK_DIVIDER_4,
      .flashWaitStates = 1,
      .enableFlashBuffer = true,
      .MCLK = 24000000,
      .HSMCLK = 6000000,
      .SMCLK = 6000000,
      .ACLK = 32768
     },
    { .activeState = AM_DCDC_VCORE1,
      .VCORE = 1,
      .clockSource = CS_DCOCLK_SELECT,
      .DCORESEL = CS_DCO_FREQUENCY_48,
      .DIVM = CS_CLOCK_DIVIDER_1,
      .DIVHS = CS_CLOCK_DIVIDER_2,
      .DIVS = CS_CLOCK_DIVIDER_4,
      .flashWaitStates = 2,
      .enableFlashBuffer = true,
      .MCLK = 48000000,
      .HSMCLK = 24000000,
      .SMCLK = 12000000,
      .ACLK = 32768
     },
};

/*
 *  ======== Power_enablePolicy ========
 *  Enable the power policy to run on each pass through the idle loop
 */
void Power_enablePolicy(void)
{
    PowerMSP432_module.enablePolicy = true;
}

/*
 *  ======== Power_getConstraintMask ========
 *  Get a bitmask indicating the constraints that have been registered with
 *  Power.
 */
unsigned int Power_getConstraintMask(void)
{
    return (PowerMSP432_module.constraintMask);
}

/*
 *  ======== Power_getPerformanceLevel ========
 *  Get the current performance level.
 */
unsigned int Power_getPerformanceLevel(void)
{
    return (PowerMSP432_module.currentPerfLevel);
}

/*
 *  ======== Power_getTransitionLatency ========
 *  Get the transition latency for a sleep state.  The latency is reported
 *  in units of microseconds.
 */
uint32_t Power_getTransitionLatency(unsigned int sleepState, unsigned int type)
{
    uint32_t latency = 0;

    if (type == Power_RESUME) {
        if (sleepState == PowerMSP432_SLEEP) {
            latency = PowerMSP432_RESUMETIMESLEEP;
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_0) {
            latency = PowerMSP432_RESUMETIMEDEEPSLEEP_0;
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_1) {
            latency = PowerMSP432_RESUMETIMEDEEPSLEEP_1;
        }
    }
    else {
        if (sleepState == PowerMSP432_SLEEP) {
            latency = PowerMSP432_TOTALTIMESLEEP;
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_0) {
            latency = PowerMSP432_TOTALTIMEDEEPSLEEP_0;
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_1) {
            latency = PowerMSP432_TOTALTIMEDEEPSLEEP_1;
        }
    }

    return (latency);
}

/*
 *  ======== Power_getTransitionState ========
 *  Get the current sleep transition state.
 */
unsigned int Power_getTransitionState(void)
{
    return (PowerMSP432_module.state);
}

/*
 *  ======== Power_idleFunc ========
 *  Stub function to call the power policy from the OS idle loop.
 */
void Power_idleFunc()
{
    /* only call the policy function if this has been enabled */
    if (PowerMSP432_module.enablePolicy) {
        if (PowerMSP432_config.policyFxn != NULL) {
            (*(PowerMSP432_config.policyFxn))();
        }
    }
}

/*
 *  ======== Power_init ========
 */
void Power_init()
{
    /* if performance control is enabled, go to the initial performance level */
    if (PowerMSP432_config.enablePerf) {
        PowerMSP432_module.perfInitialized =
            initPerfControl(PowerMSP432_config.initialPerfLevel);
    }

    /* if there is a configured policy init function, call it now ... */
    if (PowerMSP432_config.policyInitFxn != NULL) {
        (*(PowerMSP432_config.policyInitFxn))();
    }

    /* explicitly enable SRAM retention (for CS) */
    SYSCTL_SRAM_BANKRET |= 0xfe;
}

/*
 *  ======== Power_registerNotify ========
 *  Register a function to be called upon a specific power event.
 */
unsigned int Power_registerNotify(Power_NotifyObj * pNotifyObj,
    unsigned int eventTypes, Power_NotifyFxn notifyFxn, uintptr_t clientArg)
{
    /* assert pointers are not NULL */
    DebugP_assert(pNotifyObj != NULL);
    DebugP_assert(notifyFxn != NULL);

    /* fill in notify object elements */
    pNotifyObj->eventTypes = eventTypes;
    pNotifyObj->notifyFxn = notifyFxn;
    pNotifyObj->clientArg = clientArg;

    /* place notify object on event notification queue */
    ListP_put(&PowerMSP432_module.notifyList, (ListP_Elem*)pNotifyObj);

    return (Power_SOK);
}

/*
 *  ======== Power_releaseConstraint ========
 *  Release a previously declared constraint.
 */
void Power_releaseConstraint(unsigned int constraintId)
{
    uintptr_t key;
    uint8_t count;

    /* assert constraintId is valid */
    DebugP_assert(constraintId < PowerMSP432_NUMCONSTRAINTS);

    /* disable interrupts */
    key = HwiP_disable();

    /* get the count of the constraint */
    count = PowerMSP432_module.constraintCounts[constraintId];

    /* assert that the count is not 0 */
    DebugP_assert(count != 0);

    /* decrement the count */
    count--;

    /* save the updated count */
    PowerMSP432_module.constraintCounts[constraintId] = count;

    /* if constraint count reaches 0, remove constraint from mask */
    if (count == 0) {
        PowerMSP432_module.constraintMask &= ~(1 << constraintId);
    }

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint.
 */
void Power_setConstraint(unsigned int constraintId)
{
    uintptr_t key;

    /* assert constraint id is valid */
    DebugP_assert(constraintId < PowerMSP432_NUMCONSTRAINTS);

    /* disable interrupts */
    key = HwiP_disable();

    /* set the specified constraint in the constraintMask */
    PowerMSP432_module.constraintMask |= 1 << constraintId;

    /* increment the specified constraint count */
    PowerMSP432_module.constraintCounts[constraintId]++;

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_setPerformanceLevel ========
 *  Set a new performance level for the device.
 *  This function should only be called from Task context.
 */
unsigned int Power_setPerformanceLevel(unsigned int level)
{
    unsigned int status = Power_SOK;
    bool changedStateOK = true;
    bool changedState = false;
    bool changedWaits = false;
    PowerMSP432_Freqs freqs;
    unsigned int constraints;
    unsigned int vcoreNow;
    unsigned int vcoreNew;
    unsigned int waitsNow;
    unsigned int waitsNew;
    unsigned int stateNow;
    unsigned int stateNew;
    unsigned int  hwiKey;

    /* return immediately if performance control not enabled and initialized */
    if (!PowerMSP432_module.perfInitialized) {
        return (Power_EFAIL);
    }

    /* disable interrupts */
    hwiKey = HwiP_disable();

    /* check to make sure not busy with another transition */
    if (Power_getTransitionState() != Power_ACTIVE) {
        status = Power_EBUSY;
    }

    /* validate the performance level */
    else if (level >= NUMPERFLEVELS) {
        status = Power_EFAIL;
    }

    else {

        /* check if there is a constraint prohibiting the transition */
        constraints = Power_getConstraintMask();

        /* check if constraint set to prohibit *any* perf changes */
        if (constraints & (1 << PowerMSP432_DISALLOW_PERF_CHANGES)) {
            status = Power_EBUSY;
        }

        /* now check if a constraint is set for this perf level */
        else if (((level == 0) &&
                    (constraints & (1 << PowerMSP432_DISALLOW_PERFLEVEL_0))) ||
                 ((level == 1) &&
                   (constraints & (1 << PowerMSP432_DISALLOW_PERFLEVEL_1))) ||
                 ((level == 2) &&
                    (constraints & (1 << PowerMSP432_DISALLOW_PERFLEVEL_2))) ||
                 ((level == 3) &&
                    (constraints & (1 << PowerMSP432_DISALLOW_PERFLEVEL_3))) ||
                 ((level == 4) &&
                    (constraints & (1 << PowerMSP432_DISALLOW_PERFLEVEL_4)))) {
            status = Power_ECHANGE_NOT_ALLOWED;
         }
    }

    /* if any checks failed, or already at this performance level, exit now */
    if ((status != Power_SOK) ||
        (level == PowerMSP432_module.currentPerfLevel)) {
        HwiP_restore(hwiKey);
        return (status);
    }

    /* else, proceed with the change */
    else {

        /* set Power's transition state */
        PowerMSP432_module.state = Power_CHANGING_PERF_LEVEL;

        /* do callout to disable scheduling */
        PowerMSP432_schedulerDisable();

        /* notify any clients registered for a start-change notification */
        status = notify(PowerMSP432_START_CHANGE_PERF_LEVEL, level);

        /* BEGIN PERF CHANGE ... */
        if (status == Power_SOK) {

            /* setup variables */
            vcoreNow = PowerMSP432_perfLevels[
                PowerMSP432_module.currentPerfLevel].VCORE;
            vcoreNew = PowerMSP432_perfLevels[level].VCORE;
            waitsNow = PowerMSP432_perfLevels[
                PowerMSP432_module.currentPerfLevel].flashWaitStates;
            waitsNew = PowerMSP432_perfLevels[level].flashWaitStates;
            stateNow = PowerMSP432_perfLevels[
                PowerMSP432_module.currentPerfLevel].activeState;
            stateNew = PowerMSP432_perfLevels[level].activeState;

            /* if new voltage is higher, change active state now */
            if (vcoreNew > vcoreNow) {
                changedStateOK = PCM_setPowerState(stateNew);
                changedState = true;
            }

            /* if all still OK, continue ... */
            if (changedStateOK) {

                /* if new flash wait states are higher, set them now */
                if (waitsNew > waitsNow) {
                    FlashCtl_setWaitState(FLASH_BANK0, waitsNew);
                    FlashCtl_setWaitState(FLASH_BANK1, waitsNew);
                    changedWaits = true;
                }

                /* now change clocks and dividers */
                CS_setDCOCenteredFrequency(
                    PowerMSP432_perfLevels[level].DCORESEL);
                CS_initClockSignal(CS_MCLK,
                    PowerMSP432_perfLevels[level].clockSource,
                    PowerMSP432_perfLevels[level].DIVM);
                CS_initClockSignal(CS_HSMCLK,
                    PowerMSP432_perfLevels[level].clockSource,
                    PowerMSP432_perfLevels[level].DIVHS);
                CS_initClockSignal(CS_SMCLK,
                    PowerMSP432_perfLevels[level].clockSource,
                    PowerMSP432_perfLevels[level].DIVS);

                /* if new flash waits not changed and different, set them now */
                if ((changedWaits == false) && (waitsNew != waitsNow)) {
                    FlashCtl_setWaitState(FLASH_BANK0, waitsNew);
                    FlashCtl_setWaitState(FLASH_BANK1, waitsNew);
                }

                /* setup flash buffering */
                if(PowerMSP432_perfLevels[level].enableFlashBuffer) {
                    FlashCtl_enableReadBuffering(FLASH_BANK0, FLASH_DATA_READ);
                    FlashCtl_enableReadBuffering(FLASH_BANK0,
                        FLASH_INSTRUCTION_FETCH);
                    FlashCtl_enableReadBuffering(FLASH_BANK1, FLASH_DATA_READ);
                    FlashCtl_enableReadBuffering(FLASH_BANK1,
                        FLASH_INSTRUCTION_FETCH);
                }
                else {
                    FlashCtl_disableReadBuffering(FLASH_BANK0, FLASH_DATA_READ);
                    FlashCtl_disableReadBuffering(FLASH_BANK0,
                        FLASH_INSTRUCTION_FETCH);
                    FlashCtl_disableReadBuffering(FLASH_BANK1, FLASH_DATA_READ);
                    FlashCtl_disableReadBuffering(FLASH_BANK1,
                        FLASH_INSTRUCTION_FETCH);
                }

                /* if new state not changed and is different, change it now */
                if ((changedState == false) && (stateNew != stateNow)) {
                    changedStateOK = PCM_setPowerState(stateNew);
                }

                if (changedStateOK) {

                    /* success! update Power module state with new level */
                    PowerMSP432_module.currentPerfLevel = level;

                    /* do callout to update frequencies */
                    freqs.MCLK = PowerMSP432_perfLevels[level].MCLK;
                    freqs.HSMCLK = PowerMSP432_perfLevels[level].HSMCLK;
                    freqs.SMCLK = PowerMSP432_perfLevels[level].SMCLK;
                    freqs.ACLK = PowerMSP432_perfLevels[level].ACLK;
                    PowerMSP432_updateFreqs(&freqs);

                    /* notify any done-change notification clients */
                    status = notify(PowerMSP432_DONE_CHANGE_PERF_LEVEL, level);
                }
            }
        }
    }

    /* restore state to active */
    PowerMSP432_module.state = Power_ACTIVE;

    /* re-enable interrupts */
    HwiP_restore(hwiKey);

    /* do callout to restore scheduling */
    PowerMSP432_schedulerRestore();

    return (status);
}

/*
 *  ======== Power_shutdown ========
 *  Put the device into a low power shutdown state.
 */
unsigned int Power_shutdown(unsigned int shutdownState, uint32_t shutdownTime)
{
    unsigned int status = Power_EFAIL;
    unsigned int constraints;
    uintptr_t hwiKey;

    /* first validate shutdownState */
    DebugP_assert((shutdownState == PowerMSP432_SHUTDOWN_0) ||
                  (shutdownState == PowerMSP432_SHUTDOWN_1));

    /* disable interrupts */
    hwiKey = HwiP_disable();

    /* make sure shutdown request doesn't violate a constraint */
    constraints = Power_getConstraintMask();
    if ((shutdownState == PowerMSP432_SHUTDOWN_0) &&
        (constraints & (1 << PowerMSP432_DISALLOW_SHUTDOWN_0))) {
        status = Power_ECHANGE_NOT_ALLOWED;
    }
    else if ((shutdownState == PowerMSP432_SHUTDOWN_1) &&
        (constraints & (1 << PowerMSP432_DISALLOW_SHUTDOWN_1))) {
        status = Power_ECHANGE_NOT_ALLOWED;
    }
    else {
        /* make sure not busy with some other transition */
        if (PowerMSP432_module.state == Power_ACTIVE) {

            /* set new transition state to entering shutdown */
            PowerMSP432_module.state = Power_ENTERING_SHUTDOWN;

            /* signal all clients registered for pre-shutdown notification */
            status = notify(PowerMSP432_ENTERING_SHUTDOWN, shutdownState);

            /* if no notification error... proceed to shutdown ... */
            if (status == Power_SOK) {
                if (shutdownState == PowerMSP432_SHUTDOWN_0) {
                    PCM_setPowerState(PCM_LPM35_VCORE0);
                }
                else {
                    PCM_setPowerState(PCM_LPM45);
                }
            }
        }
        else {
            status = Power_EBUSY;
        }
    }

    /* set state back to Power_ACTIVE */
    PowerMSP432_module.state = Power_ACTIVE;

    /* re-enable interrupts */
    HwiP_restore(hwiKey);

    /* if get here, failed to shutdown, return error code */
    return (status);
}

/*
 *  ======== Power_sleep ========
 *  Put the device into a sleep state.
 */
unsigned int Power_sleep(unsigned int sleepState)
{
    unsigned int status = Power_SOK;
    uintptr_t eventArg = 0;
    unsigned int preEvent;
    unsigned int postEvent;
    uint8_t powerMode;
    uint8_t targetState;

    /* first validate the sleep code */
    DebugP_assert((sleepState == PowerMSP432_SLEEP) ||
                  (sleepState == PowerMSP432_DEEPSLEEP_0) ||
                  (sleepState == PowerMSP432_DEEPSLEEP_1));

    if (PowerMSP432_module.state == Power_ACTIVE) {
        /* set transition state to entering sleep */
        PowerMSP432_module.state = Power_ENTERING_SLEEP;

        /* setup sleep vars */
        if (sleepState == PowerMSP432_SLEEP) {
            preEvent = PowerMSP432_ENTERING_SLEEP;
            postEvent = PowerMSP432_AWAKE_SLEEP;
        }
        else if ((sleepState == PowerMSP432_DEEPSLEEP_0) ||
                 (sleepState == PowerMSP432_DEEPSLEEP_1)) {
            preEvent = PowerMSP432_ENTERING_DEEPSLEEP;
            postEvent = PowerMSP432_AWAKE_DEEPSLEEP;
            if (sleepState == PowerMSP432_DEEPSLEEP_1) {
                eventArg = 1;
            }
        }

        /* get current power mode */
        powerMode = PCM_getPowerMode();

        if (sleepState == PowerMSP432_SLEEP) {
            switch (powerMode) {
                case PCM_AM_LDO_VCORE0:
                    targetState = PCM_LPM0_LDO_VCORE0;
                    break;
                case PCM_AM_LDO_VCORE1:
                    targetState = PCM_LPM0_LDO_VCORE1;
                    break;
                case PCM_AM_DCDC_VCORE0:
                    targetState = PCM_LPM0_DCDC_VCORE0;
                    break;
                case PCM_AM_DCDC_VCORE1:
                    targetState = PCM_LPM0_DCDC_VCORE1;
                    break;
                case PCM_AM_LF_VCORE0:
                    targetState = PCM_LPM0_LF_VCORE0;
                    break;
                case PCM_AM_LF_VCORE1:
                    targetState = PCM_LPM0_LF_VCORE1;
                    break;
            }
        }

        /* signal all clients registered for pre-sleep notification */
        status = notify(preEvent, eventArg);

        /* check for any error */
        if (status != Power_SOK) {
            PowerMSP432_module.state = Power_ACTIVE;
            return (status);
        }

        /* invoke specific sequences to activate sleep states... */

        if (sleepState == PowerMSP432_SLEEP) {
            PCM_setPowerState(targetState);
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_0) {
            PCM_setPowerState(PCM_LPM3);
        }
        else if (sleepState == PowerMSP432_DEEPSLEEP_1) {
            RTC_C_holdClock();
            WDT_A_holdTimer();
            PCM_setPowerState(PCM_LPM3);
        }

        /* set transition state to EXITING_SLEEP */
        PowerMSP432_module.state = Power_EXITING_SLEEP;

        /* signal clients registered for post-sleep notification */
        status = notify(postEvent, eventArg);

        /* now clear the transition state before re-enabling scheduler */
        PowerMSP432_module.state = Power_ACTIVE;
    }
    else {
        status = Power_EBUSY;
    }

    return (status);
}

/*
 *  ======== Power_unregisterNotify ========
 *  Unregister for a power notification.
 */
void Power_unregisterNotify(Power_NotifyObj * pNotifyObj)
{
    uintptr_t key;

    /* disable interrupts */
    key = HwiP_disable();

    /* remove notify object from its event queue */
    ListP_remove(&PowerMSP432_module.notifyList, (ListP_Elem *)pNotifyObj);

    /* re-enable interrupts */
    HwiP_restore(key);
}

/********************* MSP432 named functions **********************/

/*
 *  ======== PowerMSP432_getFreqs ========
 *  Get the frequencies for a performance level.
 */
void PowerMSP432_getFreqs(unsigned int level, PowerMSP432_Freqs * freqs)
{
    /* assert perf level is valid */
    DebugP_assert(level < NUMPERFLEVELS);

    freqs->MCLK = PowerMSP432_perfLevels[level].MCLK;
    freqs->HSMCLK = PowerMSP432_perfLevels[level].HSMCLK;
    freqs->SMCLK = PowerMSP432_perfLevels[level].SMCLK;
    freqs->ACLK = PowerMSP432_perfLevels[level].ACLK;
}

/*
 *  ======== PowerMSP432_getNumPerfLevels ========
 *  Get the number of configured performance levels.
 */
unsigned int PowerMSP432_getNumPerfLevels(void)
{
    return (NUMPERFLEVELS);
}

/*********************** internal functions **************************/

/*
 *  ======== initPerfControl ========
 *  Initialize performance control (to be called during Power_init()).
 */
static bool initPerfControl(unsigned int initLevel)
{
    PowerMSP432_Freqs freqs;
    bool status = false;
    bool changedStateOK;

    /* first validate the initial performance level */
    if (initLevel < NUMPERFLEVELS) {

        /*
         * since not sure of initial conditions, to be safe, first set a
         * DCO frequency compatible with VCORE0, and requiring no wait
         * states (e.g. 12 MHz), and select this as the source for MCLK,
         * HSMCLK and SMCLK
         */
        CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
        CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
        CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);
        CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

        /* now go the the power state for the initial performance level */
        changedStateOK = PCM_setPowerState(
            PowerMSP432_perfLevels[initLevel].activeState);

        if (changedStateOK) {

            /* setup flash wait states */
            FlashCtl_setWaitState(FLASH_BANK0,
                PowerMSP432_perfLevels[initLevel].flashWaitStates);
            FlashCtl_setWaitState(FLASH_BANK1,
                PowerMSP432_perfLevels[initLevel].flashWaitStates);

            /* setup flash buffering */
            if(PowerMSP432_perfLevels[initLevel].enableFlashBuffer) {
                FlashCtl_enableReadBuffering(FLASH_BANK0, FLASH_DATA_READ);
                FlashCtl_enableReadBuffering(FLASH_BANK0,
                    FLASH_INSTRUCTION_FETCH);
                FlashCtl_enableReadBuffering(FLASH_BANK1, FLASH_DATA_READ);
                FlashCtl_enableReadBuffering(FLASH_BANK1,
                    FLASH_INSTRUCTION_FETCH);
            }
            else {
                FlashCtl_disableReadBuffering(FLASH_BANK0, FLASH_DATA_READ);
                FlashCtl_disableReadBuffering(FLASH_BANK0,
                    FLASH_INSTRUCTION_FETCH);
                FlashCtl_disableReadBuffering(FLASH_BANK1, FLASH_DATA_READ);
                FlashCtl_disableReadBuffering(FLASH_BANK1,
                    FLASH_INSTRUCTION_FETCH);
            }

            /* now setup clocks */
            CS_setDCOCenteredFrequency(
                PowerMSP432_perfLevels[initLevel].DCORESEL);
            CS_initClockSignal(CS_MCLK,
                PowerMSP432_perfLevels[initLevel].clockSource,
                PowerMSP432_perfLevels[initLevel].DIVM);
            CS_initClockSignal(CS_HSMCLK,
                PowerMSP432_perfLevels[initLevel].clockSource,
                PowerMSP432_perfLevels[initLevel].DIVHS);
            CS_initClockSignal(CS_SMCLK,
                PowerMSP432_perfLevels[initLevel].clockSource,
                PowerMSP432_perfLevels[initLevel].DIVS);

            /* do callout to update frequencies */
            freqs.MCLK = PowerMSP432_perfLevels[initLevel].MCLK;
            freqs.HSMCLK = PowerMSP432_perfLevels[initLevel].HSMCLK;
            freqs.SMCLK = PowerMSP432_perfLevels[initLevel].SMCLK;
            freqs.ACLK = PowerMSP432_perfLevels[initLevel].ACLK;
            PowerMSP432_updateFreqs(&freqs);

            /* set perf level in Power module state */
            PowerMSP432_module.currentPerfLevel = initLevel;

            status = true;
        }
    }

    return (status);
}

/*
 *  ======== notify ========
 *  Notify clients registered for eventType notifications.
 *  When this function is called interrupts are disabled.
 */
static unsigned int notify(unsigned int eventType, uintptr_t eventArg)
{
    unsigned int status = Power_SOK;
    Power_NotifyFxn notifyFxn;
    ListP_List *notifyList;
    unsigned int notifyStatus;
    ListP_Elem *elem;
    uintptr_t clientArg;

    notifyList = &PowerMSP432_module.notifyList;

    /* point to first notify object */
    elem = ListP_head(notifyList);

    /* while queue is not empty, service each element */
    while (elem != NULL) {

        /* check if this client wants notification of this event type... */
        if (((Power_NotifyObj *)elem)->eventTypes & eventType) {

            /* yes, pull params from notify object */
            notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
            clientArg = ((Power_NotifyObj *)elem)->clientArg;

            /* call the client's notification function */
            notifyStatus = (unsigned int) (*(Power_NotifyFxn)notifyFxn)(
                eventType, eventArg, clientArg);

            /* if notification failed: stop all further notifications */
            if (notifyStatus != Power_NOTIFYDONE) {
                return (Power_EFAIL);
            }
        }

        /* get next element in the notification queue */
        elem = ListP_next(elem);
    }

    return (status);
}
