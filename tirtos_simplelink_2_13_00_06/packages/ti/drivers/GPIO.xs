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
 *  ======== GPIO.xs ========
 */

/*
 *  ======== module$use ========
 *  This function will automatically include the required GPIO module.
 */
function module$use()
{
    var GPIO = xdc.module('ti.drivers.GPIO');
    if (GPIO.$written("libType") == true) {
        this.$logWarning("Setting the GPIO.libType field is no longer " +
                         "supported. Please use the ti.tirtos.TIRTOS.libType field "  +
                         "to enable/disable logging and assert checking.", GPIO);
    }
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var GPIO = xdc.useModule('ti.drivers.GPIO');
    var ScalarStructs = xdc.useModule('xdc.rov.support.ScalarStructs');

    var eventViews = new Array();

    /* Fetch device specific GPIO config struct */

    var GPIOConfigSymbol;
    GPIOConfigSymbol = Program.getSymbolValue("GPIOCC3200_config");
    if (GPIOConfigSymbol == -1) {
        GPIOConfigSymbol = Program.getSymbolValue("GPIOMSP432_config");
        if (GPIOConfigSymbol == -1) {
            GPIOConfigSymbol = Program.getSymbolValue("GPIOTiva_config");
            if (GPIOConfigSymbol == -1) {
                GPIOConfigSymbol = Program.getSymbolValue("GPIOMSP430_config");
                if (GPIOConfigSymbol == -1) {
                    var viewElem = Program.newViewStruct('ti.drivers.GPIO', 'Basic');
                    eventViews[eventViews.length] = viewElem;
                    view.elements = eventViews;
                    Program.displayError(viewElem, "Index",
                        "Unable to find GPIOXXX_config structure");
                    return;
                }
            }
        }
    }

    var GPIO_config = Program.fetchStruct(GPIO.GPIO_Config$fetchDesc, GPIOConfigSymbol);
    var numPins = GPIO_config.numberOfPinConfigs;
    var GPIO_pinConfigs = Program.fetchArray({type: 'xdc.rov.support.ScalarStructs.S_UInt', isScalar: true}, GPIO_config.pinConfigs, numPins);

    /* Loop through all GPIOs--count to 64 in case not NULL-terminated */
    for (var i = 0; i < numPins; i++) {
        /* Create a new element for ROV */
        var viewElem = Program.newViewStruct('ti.drivers.GPIO', 'Basic');

        viewElem.Index = i.toString(10);
        var pinConfig = GPIO_pinConfigs[i];
        var undefinedPin = false;
        if (pinConfig == 0x7fff0000) {
            viewElem.Direction = "Undefined";
            undefinedPin = true;
        }
        else if (pinConfig & 0x00010000) {
            viewElem.Direction = "INPUT";
        }
        else {
            viewElem.Direction = "OUTPUT";
            viewElem.Value = pinConfig & 0x00080000 ? "1" : "0";
        }

        if (undefinedPin == false) {
            viewElem.Port = Number((pinConfig & 0xff00) >> 8).toString(10);
            var pin;
            switch (pinConfig & 0xff) {
                case 1:
                    pin = 0;
                    break;
                case 2:
                    pin = 1;
                    break;
                case 4:
                    pin = 2;
                    break;
                case 8:
                    pin = 3;
                    break;
                case 16:
                    pin = 4;
                    break;
                case 32:
                    pin = 5;
                    break;
                case 64:
                    pin = 6;
                    break;
                case 128:
                    pin = 7;
                    break;
            }
            viewElem.Pin = Number(pin).toString(10);
        }

        /* Add the this element into the array of elements */
        eventViews[eventViews.length] = viewElem;
    }

    view.elements = eventViews;
}
