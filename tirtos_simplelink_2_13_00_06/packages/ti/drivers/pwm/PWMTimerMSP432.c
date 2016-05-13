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
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

#include <rom.h>
#include <rom_map.h>
#include <timer_a.h>

void PWMTimerMSP432_close(PWM_Handle handle);
int PWMTimerMSP432_control(PWM_Handle handle, unsigned int cmd, void *arg);
unsigned int PWMTimerMSP432_getPeriodCounts(PWM_Handle handle);
unsigned int PWMTimerMSP432_getPeriodMicroSecs(PWM_Handle handle);
void PWMTimerMSP432_init(PWM_Handle handle);
PWM_Handle PWMTimerMSP432_open(PWM_Handle handle, PWM_Params *params);
void PWMTimerMSP432_setDuty(PWM_Handle handle, uint32_t duty);

/* PWM function table for PWMTimerMSP432 implementation */
const PWM_FxnTable PWMTimerMSP432_fxnTable = {
    PWMTimerMSP432_close,
    PWMTimerMSP432_control,
    PWMTimerMSP432_getPeriodCounts,
    PWMTimerMSP432_getPeriodMicroSecs,
    PWMTimerMSP432_init,
    PWMTimerMSP432_open,
    PWMTimerMSP432_setDuty
};

extern const PWM_Params PWM_defaultParams;

static PWMTimerMSP432_Status pwmTimerStatus[PWMTimerMSP432_NUM_TIMERS] = {0};

/* PWM output polarity options */
static const uint16_t outputPolarity [] = {
    TIMER_A_OUTPUTMODE_RESET_SET,       /* Active high duty */
    TIMER_A_OUTPUTMODE_SET_RESET        /* Active low duty */
};

#if DebugP_ASSERT_ENABLED
/* Max PWM clock prescalar. */
static const uint8_t maxPrescalarValue = 64;
#endif

/*
 * Timer peripheral period and duty registers are 16 bits wide.  Max value which
 * can be represented is 65535.
 */
static const uint16_t maxDutyValue = (~0);

/*
 *  ======== PWMTimerMSP432_close ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerMSP432_close(PWM_Handle handle)
{
    uintptr_t                     key;
    PWMTimerMSP432_Object        *object = handle->object;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;

    MAP_Timer_A_setCompareValue(hwAttrs->baseAddr, hwAttrs->compareRegister, 0);

    key = HwiP_disable();

    /* Mark the output as unused */
    (object->pwmTimerStatus)->activeOutputsMask &= ~(object->pwmCompareOutputBit);

    /* Stop timer & clear all status if no other PWM instances are being used */
    if ((object->pwmTimerStatus)->activeOutputsMask == 0) {
        MAP_Timer_A_stopTimer(hwAttrs->baseAddr);
        (object->pwmTimerStatus)->cyclesPerMicroSec = 0;
        (object->pwmTimerStatus)->period = 0;
        (object->pwmTimerStatus)->prescalar = 0;
    }

    object->isOpen = false;

    HwiP_restore(key);

    /* Remove power constraints */
    Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
    Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
    Power_releaseConstraint(PowerMSP432_DISALLOW_SHUTDOWN_0);
    Power_releaseConstraint(PowerMSP432_DISALLOW_SHUTDOWN_1);

    DebugP_log1("PWM: (%p) closed", (uintptr_t) handle);
}

/*
 *  ======== PWMTimerMSP432_calculatePrescalar ========
 *  Calculates timer prescalar for a given period.
 *
 *  @param  period      in timer ticks
 *  @return prescalar   required to generate period
 */
static inline uint8_t PWMTimerMSP432_calculatePrescalar(uint32_t period)
{
    /* Initialize to a prescalar of 1 */
    uint8_t prescalar = 1;

    while (period > maxDutyValue) {
        prescalar <<= 1;
        period /=2;
    }

    return (prescalar);
}

/*
 *  ======== PWMTimerMSP432_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int PWMTimerMSP432_control(PWM_Handle handle, unsigned int cmd, void *arg)
{
    uintptr_t                     key;
    uint32_t                      period;
    uint8_t                       prescalar;
    PWMTimerMSP432_Object        *object = handle->object;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;

    switch(cmd) {
        case PWMTimerMSP432_CHANGE_PERIOD:
            DebugP_assert((uint32_t *) arg != NULL);

            key = HwiP_disable();

            /* Calculate period in PWM timer counts */
            period = (*(uint32_t *) arg) *
                (object->pwmTimerStatus)->cyclesPerMicroSec;
            prescalar = PWMTimerMSP432_calculatePrescalar(period);

            /* Ensure new period can be generated with current prescalar */
            if (prescalar != (object->pwmTimerStatus)->prescalar) {
                HwiP_restore(key);
                return (-1);
            }

            MAP_Timer_A_setCompareValue(hwAttrs->baseAddr,
                TIMER_A_CAPTURECOMPARE_REGISTER_0, period/prescalar);

            /* Update PWM status with new period */
            (object->pwmTimerStatus)->period = *((uint32_t *) arg);

            HwiP_restore(key);

            return (PWMTimerMSP432_CHANGE_PERIOD);

        default:
            /* No implementation yet */
            return (PWM_STATUS_UNDEFINEDCMD);
    }
}

/*
 *  ======== PWMTimerMSP432_getPeriodCounts ========
 *  @pre    Function assumes that handle is not NULL
 */
unsigned int PWMTimerMSP432_getPeriodCounts(PWM_Handle handle)
{
    unsigned int                  period;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;

    period = MAP_Timer_A_getCaptureCompareCount(hwAttrs->baseAddr,
        TIMER_A_CAPTURECOMPARE_REGISTER_0);

    return period;
}

/*
 *  ======== PWMTimerMSP432_getPeriodMicroSecs ========
 *  @pre    Function assumes that handle is not NULL
 */
unsigned int PWMTimerMSP432_getPeriodMicroSecs(PWM_Handle handle)
{
    uintptr_t              key;
    unsigned int           period;
    PWMTimerMSP432_Object *object = handle->object;

    key = HwiP_disable();

    period = (object->pwmTimerStatus)->period;

    HwiP_restore(key);

    return period;
}

/*
 *  ======== PWMTimerMSP432_init ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerMSP432_init(PWM_Handle handle)
{
    uint8_t                       structIndex;
    PWMTimerMSP432_Object        *object = handle->object;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;


    /* Determine which PWM status structure corresponds to the instance */
    structIndex = (hwAttrs->baseAddr & 0xFFFF) / 0x400;
    object->pwmTimerStatus = &pwmTimerStatus[structIndex];

    /* Calculate the PWM output bit */
    object->pwmCompareOutputBit = 1 << ((hwAttrs->compareRegister / 2) - 2);
}

/*
 *  ======== PWMTimerMSP432_open ========
 *  @pre    Function assumes that the handle is not NULL
 */
PWM_Handle PWMTimerMSP432_open(PWM_Handle handle, PWM_Params *params)
{
    uintptr_t                     key;
    bool                          timerIsRunning;
    uint8_t                       cyclesPerMicroSec;
    uint32_t                      clockFreq;
    uint32_t                      tempPeriod;
    Timer_A_PWMConfig             pwmConfig;
    PowerMSP432_Freqs             powerFreqs;
    PWMTimerMSP432_Object        *object = handle->object;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;

    if(params == NULL) {
        params = (PWM_Params *) &PWM_defaultParams;
    }

    key = HwiP_disable();

    /*
     * Before opening the PWM instance, we must verify that the Timer is not
     * already open or being used by another source (possibly the Kernel).
     * Additionally, the Timer peripheral could have already been initialized
     * by another PWM instance, so we must verify if any other PWM driver
     * (on the same Timer) is initialized.
     */
    timerIsRunning =
        (TIMER_A_CMSIS(hwAttrs->baseAddr)->rCTL.b.bMC != TIMER_A_STOP_MODE);
    if (object->isOpen ||
        (timerIsRunning && (object->pwmTimerStatus)->activeOutputsMask == 0)) {
        /* Timer already opened or used by source other than PWM driver */
        HwiP_restore(key);
        DebugP_log1("PWM:(%p) timer used by another source.",
            (uintptr_t) handle);
        return (NULL);
    }

    /*
     * Timer capture/compare register 0 is used as the period.  It cannot be
     * used to generate PWM output.
     */
    DebugP_assert(hwAttrs->compareRegister != TIMER_A_CAPTURECOMPARE_REGISTER_0);

    /*
     * Add power management support - PWM driver does not allow performance
     * level changes, low power modes or shutdown while open.
     */
    Power_setConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);
    Power_setConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
    Power_setConstraint(PowerMSP432_DISALLOW_SHUTDOWN_0);
    Power_setConstraint(PowerMSP432_DISALLOW_SHUTDOWN_1);

    PowerMSP432_getFreqs(Power_getPerformanceLevel(), &powerFreqs);
    clockFreq = powerFreqs.SMCLK;
    cyclesPerMicroSec = clockFreq / 1000000;

    /* Assert if period is too large for peripheral */
    tempPeriod = params->period * cyclesPerMicroSec;
    DebugP_assert(tempPeriod <= maxPrescalarValue * maxDutyValue);

    /*
     * Verify if timer has been initialized by another PWM instance.  If so,
     * make sure PWM periods are the same, do not open driver if otherwise.
     */
    if ((object->pwmTimerStatus)->period &&
        (object->pwmTimerStatus)->period != params->period) {
        HwiP_restore(key);
        DebugP_log1("PWM:(%p) differing PWM periods, cannot open driver.",
            (uintptr_t) handle);
        PWMTimerMSP432_close(handle);
        return (NULL);
    }
    else {
        /* PWM timer has not been initialized */
        (object->pwmTimerStatus)->cyclesPerMicroSec = cyclesPerMicroSec;
        (object->pwmTimerStatus)->prescalar =
            PWMTimerMSP432_calculatePrescalar(tempPeriod);
        (object->pwmTimerStatus)->period = params->period;
    }

    /* Mark driver as being used */
    object->isOpen = true;
    (object->pwmTimerStatus)->activeOutputsMask |= object->pwmCompareOutputBit;

    HwiP_restore(key);

    /* Store PWM instance parameters */
    object->dutyMode = params->dutyMode;

    /* Configure PWM output & start timer */
    pwmConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    pwmConfig.clockSourceDivider = (object->pwmTimerStatus)->prescalar;
    pwmConfig.timerPeriod = tempPeriod / (object->pwmTimerStatus)->prescalar;
    pwmConfig.compareRegister = hwAttrs->compareRegister;
    pwmConfig.compareOutputMode = outputPolarity[params->polarity];
    pwmConfig.dutyCycle = 0;
    MAP_Timer_A_generatePWM(hwAttrs->baseAddr, &pwmConfig);
    MAP_Timer_A_startCounter(hwAttrs->baseAddr, TIMER_A_UP_MODE);

    DebugP_log2("PWM:(%p) opened; period set to: %d", (uintptr_t) handle,
        params->period);

    return (handle);
}

/*
 *  ======== PWMTimerMSP432_setDuty ========
 *  @pre    Function assumes that handle is not NULL
 */
void PWMTimerMSP432_setDuty(PWM_Handle handle, uint32_t duty)
{
    uintptr_t                     key;
    uint16_t                      period;
    uint32_t                      newDuty;
    PWMTimerMSP432_Object        *object = handle->object;
    PWMTimerMSP432_HWAttrs const *hwAttrs = handle->hwAttrs;

    key = HwiP_disable();

    period = MAP_Timer_A_getCaptureCompareCount(hwAttrs->baseAddr,
        TIMER_A_CAPTURECOMPARE_REGISTER_0);

    switch(object->dutyMode) {
        case PWM_DUTY_COUNTS:
            /* Duty specified as PWM timer counts */
            DebugP_assert(duty <= period);

            newDuty = duty;
            break;

        case PWM_DUTY_TIME:
            /* Duty is specified in microseconds */
            DebugP_assert(duty <= (object->pwmTimerStatus)->period);

            newDuty = (duty * (object->pwmTimerStatus)->cyclesPerMicroSec) /
                (object->pwmTimerStatus)->prescalar;
            break;

        case PWM_DUTY_SCALAR:
            /* Duty specified as a number [0 - 65535] scaled to the period */
            DebugP_assert(duty <= maxDutyValue);

            if (duty >= maxDutyValue) {
                newDuty = period;
            }
            else {
                newDuty = (period * 100) / maxDutyValue;
                newDuty = (newDuty * duty) / 100;
            }
            break;

        default:
            DebugP_log1("PWM: (%p) unsupported PWM duty mode; duty unchanged",
                (uintptr_t) handle);
            return;
    }

    /*
     * This condition ensures that the output will remain active if the newDuty
     * is greater or equal to the period.
     */
    if (newDuty >= period) {
        newDuty = (period + 1);
    }

    MAP_Timer_A_setCompareValue(hwAttrs->baseAddr, hwAttrs->compareRegister,
        newDuty);

    HwiP_restore(key);

    DebugP_log2("PWM: (%p) duty set to: %d", (uintptr_t) handle, duty);
}
