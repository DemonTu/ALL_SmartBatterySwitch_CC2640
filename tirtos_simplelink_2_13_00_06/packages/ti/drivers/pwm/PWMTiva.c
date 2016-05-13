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
#include <ti/drivers/pwm/PWMTiva.h>

#include <inc/hw_memmap.h>
#include <inc/hw_pwm.h>
#include <inc/hw_sysctl.h>
#include <inc/hw_types.h>
#include <driverlib/pwm.h>
#include <driverlib/sysctl.h>

void               PWMTiva_close(PWM_Handle handle);
int                PWMTiva_control(PWM_Handle handle, unsigned int cmd,
                                   void *arg);
unsigned int       PWMTiva_getPeriodCounts(PWM_Handle handle);
unsigned int       PWMTiva_getPeriodMicroSecs(PWM_Handle handle);
void               PWMTiva_init(PWM_Handle handle);
PWM_Handle         PWMTiva_open(PWM_Handle handle, PWM_Params *params);
void               PWMTiva_setDuty(PWM_Handle handle, uint32_t duty);
static inline void PWMTiva_calculatePrescalar(uint32_t  period,
                                              uint32_t *presRegVal,
                                              uint8_t  *prescalar);
static inline void PWMTiva_setPrescalar(uint32_t  pwmBase, uint32_t presRegVal);

/* PWM function table for PWMTiva implementation */
const PWM_FxnTable PWMTiva_fxnTable = {
    PWMTiva_close,
    PWMTiva_control,
    PWMTiva_getPeriodCounts,
    PWMTiva_getPeriodMicroSecs,
    PWMTiva_init,
    PWMTiva_open,
    PWMTiva_setDuty
};

extern const PWM_Params PWM_defaultParams;

static PWMTiva_Status PWMTiva_PWM_STATUS[PWMTiva_NUM_PWM_PERIPHERALS] = {0};

/* Max PWM clock prescalar. */
const uint8_t PWMTiva_MAX_PRESCALAR = 64;

/*
 * PWM peripheral period and duty registers are 16 bits wide.  Max value which
 * can be represented is 65535.
 */
const uint16_t PWMTiva_MAX_MATCH_VALUE = (~0);

#define PWM_GEN_MASK (0xFF0)

/*
 *  ======== PWMTiva_close ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTiva_close(PWM_Handle handle)
{
    unsigned int           key;
    uint8_t                pwmPairOutput;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    PWMTiva_setDuty(handle, 0);
    PWMOutputState(hwAttrs->baseAddr, hwAttrs->pwmOutput, false);

    key = Hwi_disable();

    pwmPairOutput = (hwAttrs->pwmOutput & 0x01) ? object->pwmOutputBit >> 1 :
                                                  object->pwmOutputBit << 1;

    /* Disable PWM generator only if the pair output is not being used. */
    if (!((object->pwmStatus)->activeOutputs & pwmPairOutput)) {
        PWMGenDisable(hwAttrs->baseAddr, hwAttrs->pwmOutput & PWM_GEN_MASK);
        *((object->pwmStatus)->genPeriods +
          ((hwAttrs->pwmOutput / PWM_OUT_0) - 1)) = 0;
    }

    (object->pwmStatus)->activeOutputs &= ~(object->pwmOutputBit);

    if (!(object->pwmStatus)->activeOutputs) {
        /* PWM completely off, clear all settings */
        (object->pwmStatus)->cyclesPerMicroSec = 0;
        (object->pwmStatus)->prescalar = 0;
    }

    Hwi_restore(key);

    Log_print1(Diags_USER1, "PWM: (%p) closed", (UArg) handle);
}

/*
 *  ======== PWMTiva_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int PWMTiva_control(PWM_Handle handle, unsigned int cmd, void *arg)
{
    unsigned int           key;
    uint32_t               period;
    uint32_t               presRegVal;
    uint8_t                prescalar;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    switch(cmd) {
        case PWMTiva_CHANGE_GEN_PERIOD:
            Assert_isTrue((uint32_t *) arg != NULL, NULL);

            /* Calculate period in PWM timer counts */
            period = (*(uint32_t *) arg) * (object->pwmStatus)->cyclesPerMicroSec;

            /* Ensure new period can be generated with current prescalar */
            PWMTiva_calculatePrescalar(period, &presRegVal, &prescalar);
            if (prescalar != (object->pwmStatus)->prescalar) {
                return (-1);
            }
            period /= prescalar;

            key = Hwi_disable();
            PWMGenPeriodSet(hwAttrs->baseAddr, hwAttrs->pwmOutput & PWM_GEN_MASK,
                            period);

            /* Update PWM status with new period */
            *((object->pwmStatus)->genPeriods +
              ((hwAttrs->pwmOutput / PWM_OUT_0) - 1)) = *((uint32_t *) arg);

            Hwi_restore(key);

            return (PWMTiva_CHANGE_GEN_PERIOD);
    }

    /* No implementation yet */
    return (PWM_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== PWMTiva_getPeriodCounts ========
 *  @pre    Function assumes that handle is not NULL
 */
unsigned int PWMTiva_getPeriodCounts(PWM_Handle handle)
{
    unsigned int           key;
    unsigned int           period;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    key = Hwi_disable();
    period = PWMGenPeriodGet(hwAttrs->baseAddr, hwAttrs->pwmOutput & PWM_GEN_MASK);

    Hwi_restore(key);

    return period;
}

/*
 *  ======== PWMTiva_getPeriodMicroSecs ========
 *  @pre    Function assumes that handle is not NULL
 */
unsigned int PWMTiva_getPeriodMicroSecs(PWM_Handle handle)
{
    unsigned int           key;
    uint32_t               period;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    key = Hwi_disable();
    period = *((object->pwmStatus)->genPeriods +
               ((hwAttrs->pwmOutput / PWM_OUT_0) - 1));

    Hwi_restore(key);

    return period;
}

/*
 *  ======== PWMTiva_calculatePrescalar ========
 *  Calculates PWM prescalar for a given period.
 *
 *  @param   period       PWM period in timer ticks
 *  @param  *presRegVal   pointer to store the prescalar register value
 *  @param  *prescalar    pointer to store the prescalar decimal value
 *
 */
static inline void PWMTiva_calculatePrescalar(uint32_t  period,
                                              uint32_t *presRegVal,
                                              uint8_t  *prescalar)
{
    uint8_t  prescalarCount = 0;
    uint32_t prescalarIncrement = 0;

    /* Initialize to a prescalar of 1 */
    *presRegVal = PWM_SYSCLK_DIV_1;
    *prescalar = 1;

    if (period > PWMTiva_MAX_MATCH_VALUE) {
        while (period > PWMTiva_MAX_MATCH_VALUE) {
            ++prescalarCount;
            period /=2;
        }

        /* Calculate PWM prescalar - device dependent */
        if ((HWREG(SYSCTL_DID0) & SYSCTL_DID0_CLASS_M) ==
            SYSCTL_DID0_CLASS_TM4C129) {
            /* TM4C129 device */
            *presRegVal = PWM_SYSCLK_DIV_2;
            prescalarIncrement = (PWM_SYSCLK_DIV_4 - PWM_SYSCLK_DIV_2);
        }
        else {
            /* TM4C123 device */
            *presRegVal = SYSCTL_PWMDIV_2;
            prescalarIncrement = (SYSCTL_PWMDIV_4 - SYSCTL_PWMDIV_2);
        }

        *presRegVal += (prescalarIncrement * (prescalarCount - 1));
        *prescalar = 1 << prescalarCount;
    }
}

/*
 *  ======== PWMTiva_setPrescalar ========
 *  Set PWM prescalar for a given peripheral.
 *
 *  @param  pwmBase    PWM period in timer ticks
 *  @param  presRegVal prescalar register value
 */
static inline void PWMTiva_setPrescalar(uint32_t  pwmBase, uint32_t presRegVal)
{
    /* Set PWM prescalar - device dependent */
    if ((HWREG(SYSCTL_DID0) & SYSCTL_DID0_CLASS_M) ==
        SYSCTL_DID0_CLASS_TM4C129) {
        /* TM4C129 device */
        PWMClockSet(pwmBase, presRegVal);
    }
    else {
        /* TM4C123 device */
        SysCtlPWMClockSet(presRegVal);
    }
}

/*
 *  ======== PWMTiva_init ========
 *  @pre    Function assumes that the handle is not NULL
 */
void PWMTiva_init(PWM_Handle handle)
{
    uint8_t                statusIndex;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    /*
     * Determine which PWM status structure corresponds to the instance and
     * store reference in the instance object.
     */
    if (PWMTiva_NUM_PWM_PERIPHERALS > 1) {
        statusIndex = (hwAttrs->baseAddr - PWM0_BASE)/ 0x1000;
    }
    object->pwmStatus = &PWMTiva_PWM_STATUS[statusIndex];

    /* Calculate the PWM output bit and store it in the instance object. */
    object->pwmOutputBit = 1 << (hwAttrs->pwmOutput & 0x00F);
}

/*
 *  ======== PWMTiva_open ========
 *  @pre    Function assumes that the handle is not NULL
 */
PWM_Handle PWMTiva_open(PWM_Handle handle, PWM_Params *params)
{
    unsigned int           key;
    uint8_t                cyclesPerMicroSec;
    uint8_t                prescalar;
    uint16_t               pwmGenerator;
    uint16_t              *pwmGenPeriod;
    uint32_t               pwmGenOptions;
    uint32_t               prescalarRegVal;
    uint32_t               tempPeriod;
    Types_FreqHz           freq;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    BIOS_getCpuFreq(&freq);
    cyclesPerMicroSec = freq.lo / 1000000;

    if(params == NULL) {
        params = (PWM_Params *) &PWM_defaultParams;
    }

    /* Assert if period is too large for peripheral */
    tempPeriod = params->period * cyclesPerMicroSec;
    Assert_isTrue(tempPeriod <= (PWMTiva_MAX_MATCH_VALUE * PWMTiva_MAX_PRESCALAR),
                  NULL);

    PWMTiva_calculatePrescalar(tempPeriod, &prescalarRegVal, &prescalar);

    /* Get the PWM generator and it's period */
    pwmGenerator = hwAttrs->pwmOutput & PWM_GEN_MASK;
    pwmGenPeriod = (object->pwmStatus)->genPeriods +
                   ((hwAttrs->pwmOutput / PWM_OUT_0) - 1);

    key = Hwi_disable();

    /* Verify if PWM peripheral has been initialized by another PWM instance */
    if ((object->pwmStatus)->cyclesPerMicroSec) {
        /*
         * The PWM peripheral has already been initialized.
         *
         * Ensure the instance has not been opened and the peripheral prescalar
         * is >= the prescalar required for new instance.
         */
        if (((object->pwmStatus)->activeOutputs & object->pwmOutputBit) ||
            ((object->pwmStatus)->prescalar < prescalar)) {
            Hwi_restore(key);
            return (NULL);
        }

        /* Check if the other output on the generator is active */
        if (*pwmGenPeriod) {
            /* Ensure period are the same */
            if (params->period != *pwmGenPeriod) {
                Hwi_restore(key);
                return (NULL);
            }

            /* Get PWM generator options & ensure options are the same */
            pwmGenOptions = (HWREG(hwAttrs->baseAddr + pwmGenerator) &
                             ~PWM_X_CTL_ENABLE);
            if (hwAttrs->pwmGenOpts != pwmGenOptions) {
                Hwi_restore(key);
                return (NULL);
            }
        }
    }
    else {
        /* PWM has not been initialized by another instance */

        (object->pwmStatus)->prescalar = prescalar;
        (object->pwmStatus)->cyclesPerMicroSec = cyclesPerMicroSec;
        PWMTiva_setPrescalar(hwAttrs->baseAddr, prescalarRegVal);
    }

    /* PWM can be opened safely, mark as being used */
    (object->pwmStatus)->activeOutputs |= object->pwmOutputBit;

    if (!(*pwmGenPeriod)) {
        /* Initialize PWM signal generator */
        *pwmGenPeriod = params->period;
        PWMGenConfigure(hwAttrs->baseAddr, pwmGenerator, hwAttrs->pwmGenOpts);
        PWMGenPeriodSet(hwAttrs->baseAddr, pwmGenerator, (tempPeriod / prescalar));
    }
    Hwi_restore(key);

    /*
     * Set PWM duty to initial value (not 0 or period) - required when
     * inverting output polarity to generate a duty equal to 0 or
     * period.  See comments in PWMTiva_setDuty for more information.
     */
    object->pwmDuty = 1;
    object->dutyMode = params->dutyMode;

    /* Set PWM generator outputs to 0 initially */
    PWMOutputInvert(hwAttrs->baseAddr, object->pwmOutputBit, params->polarity);
    PWMTiva_setDuty(handle, 0);
    PWMOutputState(hwAttrs->baseAddr, object->pwmOutputBit, true);
    PWMGenEnable(hwAttrs->baseAddr, pwmGenerator);

    Log_print2(Diags_USER1,
               "PWM:(%p) opened; period set to: %d",
               (UArg) handle,
               params->period);

    return (handle);
}

/*
 *  ======== PWMTiva_setDuty ========
 *  @pre    Function assumes that handle is not NULL
 */
void PWMTiva_setDuty(PWM_Handle handle, uint32_t duty)
{
    unsigned int           key;
    uint8_t                maxDutySet;
    uint16_t               period;
    uint16_t               pwmGenerator;
    uint16_t               pwmGenPeriod;
    uint32_t               newDuty;
    PWMTiva_Object        *object = handle->object;
    PWMTiva_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Get the PWM generator, generator period and instance output bit */
    pwmGenerator = hwAttrs->pwmOutput & PWM_GEN_MASK;
    pwmGenPeriod = *(object->pwmStatus)->genPeriods +
                    ((hwAttrs->pwmOutput / PWM_OUT_0) - 1);

    /* Get the generator period */
    period = PWMGenPeriodGet(hwAttrs->baseAddr, pwmGenerator);
    switch(object->dutyMode) {
        case PWM_DUTY_COUNTS:
            /* Duty specified as PWM timer counts */
            Assert_isTrue(duty <= period, NULL);

            maxDutySet = (duty >= period);
            newDuty = (duty && duty < period) ? duty : period;
            break;

        case PWM_DUTY_TIME:
            /* Duty is specified in microseconds */
            Assert_isTrue(duty <= pwmGenPeriod, NULL);

            maxDutySet = (duty >= pwmGenPeriod);
            newDuty = (duty * (object->pwmStatus)->cyclesPerMicroSec) /
                      (object->pwmStatus)->prescalar;
            if (!(newDuty) || newDuty > period) {
                newDuty = period;
            }
            break;

        case PWM_DUTY_SCALAR:
            /* Duty specified as a number [0 - 65535] scaled to the period */
            Assert_isTrue(duty <= PWMTiva_MAX_MATCH_VALUE, NULL);

            maxDutySet = (duty >= PWMTiva_MAX_MATCH_VALUE);
            newDuty = period;
            if (duty && duty < PWMTiva_MAX_MATCH_VALUE) {
                newDuty = (newDuty * 100) / PWMTiva_MAX_MATCH_VALUE;
                newDuty = (newDuty * duty) / 100;
                if (!newDuty) {
                    newDuty++;
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
     * The PWM peripheral cannot generate a duty of 0 when in count down mode
     * or a duty equal to period when in count up-down mode.  To generate a 0
     * duty when in count down mode, the PWM duty is set
     * to the period value (output remains active) and output polarity is
     * inverted.  Additionally, if the output is changed from 0 (to a non-zero
     * value) the PWM output polarity must be inverted again.
     *
     * Likewise, to generate a duty equal to the period when in count up-down
     * mode, the PWM duty is set to the period value and the output polarity is
     * inverted.
     *
     * The code below determines if the PWM is in count down or count up-down
     * mode and inverts the PWM output polarity if necessary.
     * For more details refer to the device specific datasheet and the following
     * E2E post:
     *  http://e2e.ti.com/support/microcontrollers/stellaris_arm/f/471/t/137249.aspx
     *  http://e2e.ti.com/support/microcontrollers/tiva_arm/f/908/t/354826.aspx
     */
    if (HWREG(hwAttrs->baseAddr + pwmGenerator) & PWM_GEN_MODE_UP_DOWN) {
        /*
         * PWM in count up/down mode - invert output if setting duty to or
         * changing from MAX
         */
        if ((maxDutySet && object->pwmDuty != period) ||
            ((!maxDutySet) && object->pwmDuty == period)) {
            HWREG(hwAttrs->baseAddr + PWM_O_INVERT) ^= object->pwmOutputBit;
        }
    }
    else {
        /*
         * PWM in count down mode - invert output if setting duty to or
         * changing from 0
         */
        if (((!duty) && object->pwmDuty) || (duty && (!object->pwmDuty))) {
            HWREG(hwAttrs->baseAddr + PWM_O_INVERT) ^= object->pwmOutputBit;
        }
    }
    object->pwmDuty = (duty) ? newDuty : 0;

    PWMPulseWidthSet(hwAttrs->baseAddr, hwAttrs->pwmOutput, newDuty);
    Hwi_restore(key);

    Log_print2(Diags_USER2, "PWM: (%p) duty set to: %d", (UArg) handle, duty);
}
