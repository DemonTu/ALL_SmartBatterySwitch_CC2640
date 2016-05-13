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
 *  ======== SPI.xs ========
 */

var ClockFreqs;

/*
 *  ======== module$use ========
 *  This function will automatically include ClockFreqs modules if we're
 *  dealing with a MSP430 device
 */
function module$use()
{
    if (Program.build.target.$name.search("msp430") != -1) {
        ClockFreqs = xdc.useModule('ti.sysbios.family.msp430.ClockFreqs');
    }

    var SPI = xdc.module('ti.drivers.SPI');
    if (SPI.$written("libType") == true) {
        this.$logWarning("Setting the SPI.libType field is no longer " +
                         "supported. Please use the ti.tirtos.TIRTOS.libType field "  +
                         "to enable/disable logging and assert checking.", SPI);
    }
}

/*
 *  ======== viewInitBasic ========
 */
function viewInitBasic(view)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var SPI = xdc.useModule('ti.drivers.SPI');

    var tree = viewDetailed();

    /* List if entries for the ROV module */
    var eventViews = new Array();

    for (var elem in tree) {

        /* Create a new element for ROV */
        var viewElem = Program.newViewStruct('ti.drivers.SPI', 'Basic');
        viewElem.baseAddr      = "0x" + (tree[elem].hwAttrs.baseAddr).toString(16);
        viewElem.functionTable = tree[elem].fxnTablePtr;
        viewElem.spiHandle     = elem.toString(16);

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

        var spiDrivers = new Array();
        spiDrivers.push({
                        name: 'SPITivaDMA_fxnTable',
                        object: 'SPITivaDMA_Object',
                        hwAttrs: 'SPITivaDMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPICC3200DMA_fxnTable',
                        object: 'SPICC3200DMA_Object',
                        hwAttrs: 'SPICC3200DMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPIUSCIADMA_fxnTable',
                        object: 'SPIUSCIADMA_Object',
                        hwAttrs: 'SPIUSCIADMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPIUSCIBDMA_fxnTable',
                        object: 'SPIUSCIBDMA_Object',
                        hwAttrs: 'SPIUSCIBDMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPIEUSCIADMA_fxnTable',
                        object: 'SPIEUSCIADMA_Object',
                        hwAttrs: 'SPIEUSCIADMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPIEUSCIBDMA_fxnTable',
                        object: 'SPIEUSCIBDMA_Object',
                        hwAttrs: 'SPIEUSCIBDMA_HWAttrs'
                        });
        spiDrivers.push({
                        name: 'SPIMSP432DMA_fxnTable',
                        object: 'SPIMSP432DMA_Object',
                        hwAttrs: 'SPIMSP432DMA_HWAttrs'
                        });

        var tree = getDriverStructs("SPI", spiDrivers);
        return (tree);
    }
}
