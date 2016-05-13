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
 *  ======== PWM.xs ========
 */

var ClockFreqs;
var Timer;

/*
 *  ======== module$use ========
 *  This function will automatically include the required Timer module.
 */
function module$use()
{

    if (Program.build.target.$name.search("msp430") != -1) {
        ClockFreqs = xdc.useModule('ti.sysbios.family.msp430.ClockFreqs');
        Timer = xdc.useModule('ti.sysbios.family.msp430.Timer');
    }
    else if (Program.build.target.$name.search("msp432") != -1) {
        ClockFreqs = xdc.useModule('ti.sysbios.family.arm.msp432.ClockFreqs');
        Timer = xdc.useModule('ti.sysbios.family.arm.msp432.Timer');
    }
    else {
        var Timer = xdc.useModule('ti.sysbios.family.arm.lm4.Timer');
    }

    var PWM = xdc.module('ti.drivers.PWM');
    if (PWM.$written("libType") == true) {
        this.$logWarning("Setting the PWM.libType field is no longer " +
                         "supported. Please use the ti.tirtos.TIRTOS.libType field "  +
                         "to enable/disable logging and assert checking.", PWM);
    }
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var PWM = xdc.useModule('ti.drivers.PWM');

    var tree = viewDetailed();

    /* List if entries for the ROV module */
    var eventViews = new Array();

    for (var elem in tree) {

        /* Create a new element for ROV */
        var viewElem = Program.newViewStruct('ti.drivers.PWM', 'Basic');
        viewElem.baseAddr = "0x" + (tree[elem].hwAttrs.baseAddr).toString(16);
        viewElem.functionTable = tree[elem].fxnTablePtr;
        viewElem.pwmHandle = elem.toString(16);

        /* Add this element to the array of elements */
        eventViews[eventViews.length] = viewElem;
    }

    view.elements = eventViews;
}

/*
 *  ======== viewDetailed ========
 */
function viewDetailed()
{
    var file = xdc.findFile("ti/drivers/rovdriver.xs");
    if (file) {
        /* Include rovdriver.xs' functions */
        xdc.includeFile(file);

        var pwmDrivers = new Array();
        pwmDrivers.push({
                            name: 'PWMTimerCC3200_fxnTable',
                            object: 'PWMTimerCC3200_Object',
                            hwAttrs: 'PWMTimerCC3200_HWAttrs'
                        });
        pwmDrivers.push({
                            name: 'PWMTimerTiva_fxnTable',
                            object: 'PWMTimerTiva_Object',
                            hwAttrs: 'PWMTimerTiva_HWAttrs'
                        });
        pwmDrivers.push({
                            name: 'PWMTiva_fxnTable',
                            object: 'PWMTiva_Object',
                            hwAttrs: 'PWMTiva_HWAttrs'
                        });
        pwmDrivers.push({
                            name: 'PWMTimerMSP432_fxnTable',
                            object: 'PWMTimerMSP432_Object',
                            hwAttrs: 'PWMTimerMSP432_HWAttrs'
                        });

        var tree = getDriverStructs("PWM", pwmDrivers);
        return (tree);
    }
}
