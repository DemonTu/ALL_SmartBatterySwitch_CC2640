/*
 *  Copyright (c) 2015 by Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */

/*
 *  ======== ILM3Sx7xx.xdc ========
 *  
 */

/*!
 *  ======== ILM3Sx7xx.xdc ========
 *  The Stellaris device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 *
 */

metaonly interface ILM3Sx7xx inherits ti.catalog.ICpuDataSheet
{

instance:
    override config string cpuCore           = "CM3";
    override config string isa               = "v7M";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    config ti.catalog.arm.peripherals.timers.Timer.Instance timer0;
    config ti.catalog.arm.peripherals.timers.Timer.Instance timer1;
    config ti.catalog.arm.peripherals.timers.Timer.Instance timer2;
    config ti.catalog.arm.peripherals.timers.Timer.Instance timer3;

    /*!
     *  ======== memMap ========
     *  The memory map returned by getMemoryMap().
     */
    config xdc.platform.IPlatform.Memory memMap[string] = [
        ["FRAM", {
            comment: "128KB FRAM",
            name: "FRAM",
            base: 0x00000000, 
            len:  0x00020000
        }],

        ["IRAM", {
            comment: "64KB IRAM",
            name: "IRAM",
            base: 0x20000000, 
            len:  0x00010000
        }],
    ];

};

/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

