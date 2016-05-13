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
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Power.xs ========
 *
 */

var Power = null;
var Queue = null;
var Clock = null;
var Intrinsics = null;
var Idle = null;
var Hwi = null;

/*
 * ======== getCFiles ========
 * return the array of C language files associated
 * with targetName (ie Program.build.target.$name)
 */
function getCFiles(targetName)
{
    return (["Power.c", "Power_standbyPolicy.c", "Power_calibrateRCOSC.c", "TimerGPT.c" ]);
}

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }

    /* provide getCFiles() for Build.getCFiles() */
    this.$private.getCFiles = getCFiles;

    Power = this;

    /* set fxntab default */
    Power.common$.fxntab = false;
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Queue = xdc.useModule("ti.sysbios.knl.Queue");
    Clock = xdc.useModule("ti.sysbios.knl.Clock");
    Intrinsics = xdc.useModule("ti.sysbios.knl.Intrinsics");
    Hwi = xdc.useModule("ti.sysbios.family.arm.m3.Hwi");

    if (Power.idle == true) {
        Idle = xdc.useModule('ti.sysbios.knl.Idle');

        Idle.addFunc(Power.policyFunc);
    }

    if (Power.calibrateRCOSC == true) {
        Hwi.create(44, Power.auxISR);
    }

    Power.resumeTicksSTANDBY = Power.resumeSTANDBY / Clock.tickPeriod;
    Power.totalTicksSTANDBY = Power.totalSTANDBY / Clock.tickPeriod;
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    /* init module state */
    mod.constraintsMask = 0;
    mod.state = Power.ACTIVE;
    mod.xoscPending = false;

    /* construct notification queues */
    Queue.construct(mod.notifyQ);

    /* construct the Clock object for scheduling of wakeups */
    /* initiated and started by the power policy */
    var clockParams = new Clock.Params();
    clockParams.period = 0;
    clockParams.startFlag = false;
    clockParams.arg = 0;
    Clock.construct(mod.clockObj, Power.clockFunc, 0, clockParams);

    /* construct the Clock object for XOSC_HF switching */
    /* initiated and started by Power module when activating XOSC_HF */
    clockParams.period = 0;
    clockParams.startFlag = false;
    clockParams.arg = 0;
    Clock.construct(mod.xoscClockObj, Power.xoscClockFunc, 0, clockParams);

    /* construct the Clock object for disabling LF clock quailifiers */
    /* one shot, auto start, first expires at 100 msec */
    clockParams.period = 0;
    clockParams.startFlag = true;
    clockParams.arg = 0;
    Clock.construct(mod.lfClockObj, Power.lfClockFunc,
        (100000 / Clock.tickPeriod), clockParams);
}

/*
 *  ======== viewInitModule ========
 *  Initialize the Power 'Module' view.
 */
function viewInitModule(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var modRaw = Program.scanRawView("ti.sysbios.family.arm.cc26xx.Power");
    view.ConstraintsMask = "0x" + modRaw.modState.constraintsMask.toString(16);
}
