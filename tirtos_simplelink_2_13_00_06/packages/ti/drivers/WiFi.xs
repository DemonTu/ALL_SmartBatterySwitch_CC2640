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
 *  ======== WiFi.xs ========
 */

/*
 *  ======== module$use ========
 *  This function will automatically include the required SPI module.
 */
function module$use()
{
    var WiFi = xdc.module('ti.drivers.WiFi');
    var SPI = xdc.useModule('ti.drivers.SPI');
}

/*
 *  ======== module$static$init ========
 */
function module$static$init(mod, params)
{
    var Task = xdc.module('ti.sysbios.knl.Task');
    var WiFi = xdc.module('ti.drivers.WiFi');

    if (params.spawnTaskPriority == 0) {
        WiFi.$logWarning("Priority 0 reserved for Idle task", WiFi,
                         "spawnTaskPriority");
    }

    if (params.spawnTaskPriority >= Task.numPriorities) {
        WiFi.$logError("Priority " + params.spawnTaskPriority +
                       " must be less than Task.numPriorities (" +
                       Task.numPriorities + ").",  WiFi,
                       "spawnTaskPriority");
    }
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var WiFi = xdc.useModule('ti.drivers.WiFi');
    var modCfg = Program.getModuleConfig(WiFi.$name);

    /* Get the global varialbe WiFi_config */
    var wifiConfigSymbol = Program.getSymbolValue("WiFi_config");

    /* Convert into the Data strucuture */
    var wifiHandle = Program.fetchStruct(WiFi.WiFi_Config$fetchDesc,
                                         wifiConfigSymbol);

    var viewElem = Program.newViewStruct('ti.drivers.WiFi', 'Basic');
    view.elements.$add(viewElem);

    /* Fill in the functionTable */
    viewElem.functionTable = "0x"+ Number(wifiHandle.fxnTablePtr).toString(16);

    /* Fill in the vector Id */
    viewElem.wifiHandle = "0x" + Number(wifiConfigSymbol).toString(16);

    /* CC3100 */
    var WiFiCC3100_fxnTableAddr = Number(Program.getSymbolValue(
                                         "WiFiCC3100_fxnTable"));
    var hwAttrs = Program.fetchStruct(WiFi.WiFiCC3100_HWAttrs$fetchDesc,
                                      wifiHandle.hwAttrs);
    var object = Program.fetchStruct(WiFi.WiFiCC3100_Object$fetchDesc,
                                     wifiHandle.object);

    /* Fill in the SPI handle */
    viewElem.spiHandle = "0x" + Number(object.spiHandle).toString(16);

    switch (object.spiState) {
        case WiFi.WiFiCC3100_SPI_UNINITIALIZED:
            viewElem.spiState = "STATE_UNINITIALIZED";
            break;
        case WiFi.WiFiCC3100_SPI_IDLE:
            viewElem.spiState = "STATE_IDLE";
            break;
        case WiFi.WiFiCC3100_SPI_READ_EOT:
            viewElem.spiState = "STATE_READ_EOT";
            break;
        case WiFi.WiFiCC3100_SPI_READ_MSG:
            viewElem.spiState = "STATE_READ_MSG";
            break;
        case WiFi.WiFiCC3100_SPI_WRITE_EOT:
            viewElem.spiState = "STATE_WRITE_EOT";
            break;
        case WiFi.WiFiCC3100_SPI_WRITE_MSG:
            viewElem.spiState = "STATE_WRITE_MSG";
            break;
        default:
            viewElem.spiState = "Error!";
            break;
    }
}
