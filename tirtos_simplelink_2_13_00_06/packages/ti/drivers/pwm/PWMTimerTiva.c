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

#include <stdbool.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Types.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>
#include <ti/sysbios/family/arm/lm4/Timer.h>
#include <ti/drivers/pwm/PWMTimerTiva.h>

#include <inc/hw_types.h>
#include <inc/hw_timer.h>
#include <driverlib/timer.h>

void         PWMTimerTiva_close(PWM_Handle handle);
int          PWMTimerTiva_control(PWM_Handle handle, unsigned int cmd,
                                  void *arg);
unsigned int PWMTimerTiva_getPeriodCounts(PWM_Handle handle);
unsigned int PWMTimerTiva_getPeriodMicroSecs(PWM_Handle handle);
void         PWMTimerTiva_init(PWM_Handle handle);
PWM_Handle   PWMTimerTiva_open(PWM_Handle handle, PWM_Params *params);
void         PWMTimerTiva_setDuty(PWM_Handle handle, uint32_t duty);

/* PWM function table for PWMTimerTiva implementation */
const PWM_FxnTable PWMTimerTiva_fxnTable = {
    PWMTimerTiva_close,
    PWMTimerTiva_control,
    PWMTimerTiva_getPeriodCounts,
    PWMTimerTiva_getPeriodMicroSecs,
    PWMTimerTiva_init,
    PWMTimerTiva_open,
    PWMTimerTiva_setDuty
};

extern const PWM_Params PWM_defaultParams;

/*
 * GPT peripheral match register prescalars are 8 bits wide.  Max value which
 * can be represented is 255.
 */
const uint8_t PWMTimerTiva_MAX_PRESCALAR_VALUE = (~0);

/*
 * GPT peripheral load & match registers are 16 bits wide.  Max value which
 * can be represented is 65535.
 */
const uint16_t PWMTimerTiva_MAX_MATCH_VALUE = (~0);

/*
 *  ======== PWMTimerTiva_close ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerTiva_close(PWM_Handle handle)
{
    unsigned int                key;
    uint16_t                    kernelTimerID;
    uint16_t                    timerPairEnabledBit;
    PWMTimerTiva_Object        *object = handle->object;
    PWMTimerTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Set duty to 0 before closing the PWM instance */
    PWMTimerTiva_setDuty(handle, 0);
    TimerDisable(hwAttrs->baseAddr, hwAttrs->timer);

    key = Hwi_disable();
    /*
     * Before closing the PWM instance, we must verify whether the other PWM
     * timer half is operaing.  The Timer will be marked unused only if both
     * PWM half timers are disabled.
     */
    kernelTimerID = 1 << ((hwAttrs->baseAddr >> 12) & 0xF);
    timerPairEnabledBit = (TIMER_BOTH & (~hwAttrs->timer)) &
                          (TIMER_CTL_TAEN | TIMER_CTL_TBEN);

    if (!(HWREG(hwAttrs->baseAddr + TIMER_O_CTL) & timerPairEnabledBit)) {
        /* Both timer is completely disabled */
        Timer_setAvailMask(Timer_getAvailMask() | kernelTimerID);
    }

    object->period = 0;
    Hwi_restore(key);

    Log_print1(Diags_USER1, "PWM: (%p) is closed", (UArg) handle);
}

/*
 *  ======== PWMTimerTiva_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int PWMTimerTiva_control(PWM_Handle handle, unsigned int cmd, void *arg)
{
    unsigned int                key;
    uint32_t                    newPeriod;
    uint32_t                    maxPeriod;
    PWMTimerTiva_Object        *object = handle->object;
    PWMTimerTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    switch (cmd) {
        case PWMTimerTiva_CHANGE_PERIOD:
            newPeriod = *((uint32_t *) arg);

            /* Ensure the period is less than the max period supported */
            maxPeriod = (((PWMTimerTiva_MAX_PRESCALAR_VALUE + 1) *
                       PWMTimerTiva_MAX_MATCH_VALUE) / object->cyclesPerMicroSec) - 1;
            if (!newPeriod || newPeriod > maxPeriod) {
                return (-1);
            }

            object->period  = newPeriod;
            key = Hwi_disable();
            TimerPrescaleSet(hwAttrs->baseAddr, hwAttrs->timer,
                             (object->period * object->cyclesPerMicroSec) /
                             PWMTimerTiva_MAX_MATCH_VALUE);
            TimerLoadSet(hwAttrs->baseAddr, hwAttrs->timer,
                         (object->period * object->cyclesPerMicroSec) %
                         PWMTimerTiva_MAX_MATCH_VALUE);

            Hwi_restore(key);
            return PWMTimerTiva_CHANGE_PERIOD;
    }

    /* No implementation yet */
    return (PWM_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== PWMTimerTiva_getPeriodCounts ========
 *  @pre    Function assumes that the handle is not NULL
 */
unsigned int PWMTimerTiva_getPeriodCounts(PWM_Handle handle)
{
    PWMTimerTiva_Object *object = handle->object;

    return (object->period * object->cyclesPerMicroSec);
}

/*
 *  ======== PWMTimerTiva_getPeriodMicroSecs ========
 *  @pre    Function assumes that the handle is not NULL
 */
unsigned int PWMTimerTiva_getPeriodMicroSecs(PWM_Handle handle)
{
    return (((PWMTimerTiva_Object *) handle->object)->period);
}

/*
 *  ======== PWMTimerTiva_init ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTimerTiva_init(PWM_Handle handle)
{
    /* Mark the object as available */
    ((PWMTimerTiva_Object *) handle->object)->period = 0;
}

/*
 *  ======== PWMTimerTiva_open ========
 *  @pre    Function assumes that the handle is not NULL
 */
PWM_Handle PWMTimerTiva_open(PWM_Handle handle, PWM_Params *params)
{
    unsigned int                key;
    uint16_t                    kernelTimerID;
    uint16_t                    timerPairEnabledBit;
    uint32_t                    timerPeriod;
    uint32_t                    timerConfigVal;
    Types_FreqHz                freq;
    PWMTimerTiva_Object        *object = handle->object;
    PWMTimerTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    key = Hwi_disable();

    /* Ensure peripheral is not already open */
    if (object->period) {
        Hwi_restore(key);
        return (NULL);
    }

    /*
     * Before opening the PWM instance, we must verify that the Timer is not
     * being used by the Kernel.  Additionally, the Timer peripheral could have
     * already been initialized by the other PWM timer half.  So we must also
     * verify if the other half is initialized.
     */
    kernelTimerID = 1 << ((hwAttrs->baseAddr >> 12) & 0xF);
    timerPairEnabledBit = (TIMER_BOTH & (~hwAttrs->timer)) &
                          (TIMER_CTL_TAEN | TIMER_CTL_TBEN);

    if (kernelTimerID & Timer_getAvailMask()) {
        /* Timer unused, claim timer from Kernel */
        kernelTimerID = (~kernelTimerID) & 0x0F;
        Timer_setAvailMask(Timer_getAvailMask() & kernelTimerID);
    }
    else if (!(HWREG(hwAttrs->baseAddr + TIMER_O_CTL) & timerPairEnabledBit)) {
        /* Timer used and was not initialized by the other half timer */
        Hwi_restore(key);
        return (NULL);
    }

    object->period = (~0);
    Hwi_restore(key);

    if(params == NULL) {
        params = (PWM_Params *) &PWM_defaultParams;
    }

    BIOS_getCpuFreq(&freq);
    object->cyclesPerMicroSec = freq.lo / 1000000;

    /* Calculate the maximum period supported by the peripheral */
    timerPeriod = (((PWMTimerTiva_MAX_PRESCALAR_VALUE + 1) *
                    PWMTimerTiva_MAX_MATCH_VALUE) / object->cyclesPerMicroSec) - 1;

    /* Ensure the period is less than the max period supported */
    Assert_isTrue(params->period && params->period <= timerPeriod, NULL);

    /*
     * Set PWM duty to initial value (not 0) - required when inverting
     * output polarity to generate a duty equal to 0 or period.  See comments in
     * PWMTimerCC3200_setDuty for more information.
     */
    object->duty = 1;
    object->dutyMode = params->dutyMode;
    object->period = params->period;

    /*
     * The TimerConfigure API halts both timers when it is used to configure
     * a single half timer.  The code below performs the register
     * operations necessary to configure each half timer individually.
     */
    TimerDisable(hwAttrs->baseAddr, hwAttrs->timer);

    /* Split the timer and configure it as a PWM */
    timerConfigVal = ((hwAttrs->timer & (TIMER_CFG_A_PWM | TIMER_CFG_B_PWM)) |
                      TIMER_CFG_SPLIT_PAIR);
    HWREG(hwAttrs->baseAddr + TIMER_O_CFG) |= (timerConfigVal >> 24);
    if(hwAttrs->timer & TIMER_A){
        HWREG(hwAttrs->baseAddr + TIMER_O_TAMR) = timerConfigVal & 255;
    }
    else  {
        HWREG(hwAttrs->baseAddr + TIMER_O_TBMR) = (timerConfigVal >> 8) & 255;
    }
    TimerControlLevel(hwAttrs->baseAddr, hwAttrs->timer, params->polarity);

    timerPeriod = object->period * object->cyclesPerMicroSec;
    TimerPrescaleSet(hwAttrs->baseAddr, hwAttrs->timer,
                     timerPeriod / PWMTimerTiva_MAX_MATCH_VALUE);
    TimerLoadSet(hwAttrs->baseAddr, hwAttrs->timer,
                 timerPeriod % PWMTimerTiva_MAX_MATCH_VALUE);

    PWMTimerTiva_setDuty(handle, 0);
    TimerEnable(hwAttrs->baseAddr, hwAttrs->timer);

    Log_print2(Diags_USER1, "PWM:(%p) opened; period set to: %d", (UArg) handle,
               object->period);

    return (handle);
}

/*
 *  ======== PWMTimerTiva_setDuty ========
 *  @pre    Function assumes that handle is not NULL
 */
void PWMTimerTiva_setDuty(PWM_Handle handle, uint32_t duty)
{
    unsigned int                key;
    uint32_t                    timerPeriod;
    uint32_t                    dutyInCycles;
    PWMTimerTiva_Object        *object = handle->object;
    PWMTimerTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    timerPeriod = object->period * object->cyclesPerMicroSec;
    switch (object->dutyMode) {
        case PWM_DUTY_COUNTS:
            /* Duty specified as timer counts */
            Assert_isTrue(duty <= timerPeriod, NULL);

            dutyInCycles = timerPeriod;
            if (duty < dutyInCycles) {
                dutyInCycles -= duty;
            }
            break;

        case PWM_DUTY_TIME:
            /* Duty is specified in microseconds */
            Assert_isTrue(duty <= object->period, NULL);

            dutyInCycles = object->period;
            if (duty < object->period) {
                dutyInCycles -= duty;
            }
            dutyInCycles *= object->cyclesPerMicroSec;
            break;

        case PWM_DUTY_SCALAR:
            /* Duty specified as a number [0 - 65535] scaled to the period */
            Assert_isTrue(duty <= PWMTimerTiva_MAX_MATCH_VALUE, NULL);

            /* If duty is 0; Max value is used but the polarity inverted */
            dutyInCycles = timerPeriod;
            if (duty && duty < PWMTimerTiva_MAX_MATCH_VALUE) {
                dutyInCycles = (dutyInCycles * 100) / PWMTimerTiva_MAX_MATCH_VALUE;
                dutyInCycles = (dutyInCycles *
                                (PWMTimerTiva_MAX_MATCH_VALUE - duty)) / 100;
                if (!dutyInCycles) {
                    ++dutyInCycles;
                }
            }
            break;
        default:
            Log_print1(Diags_USER1,
                       "PWM: (%p) unsupported PWM duty mode; duty unchanged",
                       (UArg) handle);
            return;
    }

    key = Hwi_disable();
    /*
     * The timer peripheral cannot generate a duty of 0 when the timer is
     * counting down.  In cases where a 0 duty is desired, the PWM duty is set
     * to the period value (output remains active) and output polarity is
     * inverted.  Additionally, if the output is changed from 0 (to a non-zero
     * value) the PWM output polarity must be inverted again.
     *
     * The code below uses the previous duty (object->duty) and the new duty to
     * determine if the polarity should be inverted.
     * For more details refer to the device specific datasheet and the following
     * E2E post:
     *  http://e2e.ti.com/support/microcontrollers/tiva_arm/f/908/t/354826.aspx
     */
    if (((!duty) && object->duty) || (duty && (!object->duty))) {
        HWREG(hwAttrs->baseAddr + TIMER_O_CTL) ^=
                       (hwAttrs->timer & (TIMER_CTL_TAPWML | TIMER_CTL_TBPWML));
    }

    object->duty = duty;
    TimerPrescaleMatchSet(hwAttrs->baseAddr, hwAttrs->timer,
                          dutyInCycles / PWMTimerTiva_MAX_MATCH_VALUE);
    TimerMatchSet(hwAttrs->baseAddr, hwAttrs->timer,
                  dutyInCycles % PWMTimerTiva_MAX_MATCH_VALUE);

    Hwi_restore(key);
    Log_print2(Diags_USER2, "PWM: (%p) duty set to: %d", (UArg) handle, duty);
}
