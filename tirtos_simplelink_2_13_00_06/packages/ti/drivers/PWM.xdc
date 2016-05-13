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
 *  ======== PWM.xdc ========
 */
package ti.drivers;
import xdc.rov.ViewInfo;

/*!
 *  ======== PWM ========
 */
@NoRuntime
@HeaderName("")
module PWM {

    /*!
     *  ======== LibType ========
     *  PWM library selection options
     *
     *  This enumeration defines all the PWM library types
     *  provided by the product.  You can select the library type by setting
     *  the {@link #libType PWM.libType} configuration parameter.
     *
     *  @field(LibType_Instrumented) The library supplied is prebuilt with
     *  logging and assertions enabled.
     *  Diags_USER1 mask logs basic information
     *  Diags_USER2 mask logs more detailed information
     *
     *  @field(LibType_NonInstrumented) The library supplied is prebuilt
     *  with logging and assertions disabled.
     */
    enum LibType {
        LibType_Instrumented,           /*! instrumented */
        LibType_NonInstrumented         /*! non-instrumented */
    };

    /*!
     *  ======== libType ========
     *  PWM Library type
     *
     *  Deprecated: Please use ti.tirtos.TIRTOS.libType instead.
     *
     *  The PWM runtime is provided in the form of a library that is
     *  linked with your application.  Several forms of this library are
     *  provided with the PWM product. This configuration parameter
     *  allows you to select the form of PWM to use.
     *
     *  The default value of libType is
     *  {@link #LibType_Instrumented PWM_LibType_Instrumented}.  For a
     *  complete list of options and what they offer see {@link #LibType}.
     */
    metaonly config LibType libType = LibType_Instrumented;

    /*!
     *  @_nodoc
     *  ======== PWM_Config ========
     *  Structure must match the PWM_Config structure defined in PWM.h
     */
    struct PWM_Config {
        Void *fxnTablePtr;
        Void *object;
        Void *hwAttrs;
    };

    /*!
     *  @_nodoc
     *  ======== PWM_DutyMode ========
     *  Structure must match the PWM_DutyMode enumeration defined in PWM.h
     */
    enum PWM_DutyMode {
        PWM_DUTY_COUNTS,
        PWM_DUTY_SCALAR,
        PWM_DUTY_TIME
    };

    /*!
     *  @_nodoc
     *  ======== PWM_Polarity ========
     *  Structure must match the PWM_Polarity enumeration defined in PWM.h
     */
    enum PWM_Polarity {
        PWM_POL_ACTIVE_HIGH,
        PWM_POL_ACTIVE_LOW
    };

    /*!
     *  @_nodoc
     *  ======== PWM_Params ========
     *  Structure must match the PWM_Params structure defined in PWM.h
     */
    struct PWM_Params {
        Bits32       period;
        PWM_DutyMode dutyMode;
        PWM_Polarity polarity;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerCC3200_hwAttrs ========
     *  Structure must match the PWMTimerCC3200_HWAttrs structure defined in
     *  pwm/PWMTimerCC3200.h
     */
    struct PWMTimerCC3200_HWAttrs {
        Bits32 baseAddr;
        Bits16 timer;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerCC3200_Object ========
     *  Structure must match the PWMTimerCC3200_Object structure defined in
     *  pwm/PWMTimerCC3200.h
     */
    struct PWMTimerCC3200_Object {
        Bits32 period;
        Bits32 duty;
        Bits8  dutyMode;
        Bits8  cyclesPerMicroSec;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTiva_Status ========
     *  Structure must match the PWMTiva_Status structure defined in
     *  pwm/PWMTiva.h
     */
     struct PWMTiva_Status {
        Bits16 genPeriods[4];
        Bits8  cyclesPerMicroSec;
        Bits8  prescalar;
        Bits8  activeOutputs;
     };

    /*!
     *  @_nodoc
     *  ======== PWMTiva_hwAttrs ========
     *  Structure must match the PWMTiva_HWAttrs structure defined in
     *  pwm/PWMTiva.h
     */
    struct PWMTiva_HWAttrs {
        Bits32 baseAddr;
        Bits32 pwmOutput;
        Bits32 pwmGenOpts;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTiva_Object ========
     *  Structure must match the PWMTiva_Object structure defined in
     *  pwm/PWMTiva.h
     */
    struct PWMTiva_Object {
        Ptr    pwmStatus;
        Bits16 pwmDuty;
        Bits8  pwmOutputBit;
        Bits8  dutyMode;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerTiva_HWAttrs ========
     *  Structure must match the PWMTimerTiva_HWAttrs structure defined in
     *  pwm/PWMTimerTiva.h
     */
    struct PWMTimerTiva_HWAttrs {
        Bits32 baseAddr;
        Bits16 timer;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerTiva_Object ========
     *  Structure must match the PWMTimerTiva_Object structure defined in
     *  pwm/PWMTimerTiva.h
     */
    struct PWMTimerTiva_Object {
        Bits32 period;
        Bits32 duty;
        Bits8  dutyMode;
        Bits8  cyclesPerMicroSec;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerMSP432_Status ========
     *  Structure must match the PWMTimerMSP432_Status structure defined in
     *  pwm/PWMTimerMSP432.h
     */
     struct PWMTimerMSP432_Status {
        Bits16 period;
        Bits8  prescalar;
        Bits8  cyclesPerMicroSec;
        Bits8  activeOutputs;
     };

    /*!
     *  @_nodoc
     *  ======== PWMTimerMSP432_HWAttrs ========
     *  Structure must match the PWMTimerMSP432_HWAttrs structure defined in
     *  pwm/PWMTimerMSP432.h
     */
    struct PWMTimerMSP432_HWAttrs {
        Bits32 baseAddr;
        Bits8  compareRegister;
    };

    /*!
     *  @_nodoc
     *  ======== PWMTimerMSP432_Object ========
     *  Structure must match the PWMTimerMSP432_Object structure defined in
     *  pwm/PWMTimerMSP432.h
     */
    struct PWMTimerMSP432_Object {
        Ptr    pwmStatus;
        Bits16 pwmDuty;
        Bits8  pwmCompareRegisterBit;
        Bits8  dutyMode;
    };

    /*!
     *  @_nodoc
     *  ======== BasicView ========
     */
    metaonly struct BasicView {
        String pwmHandle;
        String baseAddr;
        String functionTable;
    };

    /*!
     *  @_nodoc
     *  ======== rovViewInfo ========
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Basic',
                    {
                        type: ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewInitBasic',
                        structName: 'BasicView'
                    }
                ]
            ]
        });
}
