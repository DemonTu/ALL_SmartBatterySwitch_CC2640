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
 *  ======== Power.xs ========
 */

var Clock;
var Idle;
var Power;
var Timestamp;
var Swi;

/*
 *  ======== module$use ========
 *  This function will automatically add the Power policy to the idle loop and
 *  configure system tick source if necessary.
 */
function module$use()
{
    Power = xdc.module("ti.drivers.Power");

    /* Warn if trying to add Power to anything other than CC3200 or MSP432 */
    if (!Program.cpu.deviceName.match(/CC32/) &&
        !Program.cpu.deviceName.match(/MSP432/)) {
        Power.$logWarning("Power driver is not supported on the " +
            Program.cpu.deviceName + " device.",  Power);

        return;
    }

    /* Swi used in Power policy to disable scheduling */
    Swi = xdc.useModule('ti.sysbios.knl.Swi');

    /* If CC3200 check that delegates are compatible with sleep */
    if (Program.cpu.deviceName.match(/CC32/)) {

        var doIdle = true;
        Clock = xdc.useModule('ti.sysbios.knl.Clock');
        Timestamp = xdc.useModule('xdc.runtime.Timestamp');

        /* check if user has specified Clock proxy: yes if $$scope = 1 */
        if (Clock.TimerProxy.delegate$.$$scope == 1) {

            /* if user did not select the RTC, warn and disable sleep */
            if (!Clock.TimerProxy.delegate$.$name.match(
                /ti.sysbios.family.arm.cc32xx.Timer/)) {
                Power.$logWarning("The specified Clock.TimerProxy (" + Clock.TimerProxy.delegate$.$name + ") is not compatible with CC3200 sleep modes. The Power policy that can activate sleep modes has been automatically disabled.", this);
                doIdle = false;
            }
        }

        /* else, user did not set, BIOS may have already chosen default */
        else {
            Clock.TimerProxy =
                xdc.useModule('ti.sysbios.family.arm.cc32xx.Timer');
        }

        /* if Clock.TimerProxy is OK, now check Timestamp.SupportProxy */
        if (doIdle == true) {

            /* check if user specified a Timestamp proxy */
            if (Timestamp.SupportProxy.delegate$.$$scope == 1) {

                /* check if RTC */
                if (!Timestamp.SupportProxy.delegate$.$name.match(
                    /ti.sysbios.family.arm.cc32xx.TimestampProvider/)) {
                    Power.$logWarning("The specified Timestamp.SupportProxy (" + Timestamp.SupportProxy.delegate$.$name + ") is not compatible with CC3200 sleep modes. The Power policy that can activate sleep modes has been automatically disabled.", this);
                    doIdle = false;
                }
            }

            /* else, user did not set, but BIOS may have chosen default */
            else {
                Timestamp.SupportProxy =
                    xdc.useModule(
                        'ti.sysbios.family.arm.cc32xx.TimestampProvider');
            }
        }

        /* if delegates are good... plug the Power policy */
        if (doIdle == true) {
            /* Add Idle module and Power idle fxn */
            Idle = xdc.useModule('ti.sysbios.knl.Idle');
            Idle.addFunc('&Power_idleFunc');
        }
    }

    /* Else, if not CC3200, plug Power's idle function into Idle loop */
    else {
        /* Add Idle module and Power idle fxn */
        Idle = xdc.useModule('ti.sysbios.knl.Idle');
        Idle.addFunc('&Power_idleFunc');
    }
}
