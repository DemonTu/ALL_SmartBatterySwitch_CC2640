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
 *  ======== F28M35x.xdc ========
 *  
 */

/*!
 *  ======== F28M35x.xdc ========
 *  The F28M35x device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 *
 */

metaonly module F28M35x inherits ti.catalog.ICpuDataSheet
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
        ["BOOTROM", {
            comment: "On-Chip Boot ROM",
            name: "BOOTROM",
            base: 0x0,
            len:  0x10000,
            space: "code",
            access: "RX"
        }],

        ["FLASH_BOOT", {
            comment: "Branch for boot from Flash",
            name: "FLASH_BOOT",
            base: 0x200030,
            len:  0x4,
            space: "code/data",
            access: "RWX"
        }],

        ["FLASH", {
            comment: "512KB Flash memory",
            name: "FLASH",
            base: 0x200034,
            len:  0x7FFC8,
            space: "code/data",
            access: "RWX"
        }],

        ["OTP", {
            comment: "4KB OTP memory",
            name: "OTP",
            base: 0x680000,
            len:  0x1000,
            space: "code",
            access: "RWX"
        }],

        ["C03SRAM", {
            comment: "32KB On-Chip RAM Memory",
            name: "C03SRAM",
            base: 0x20000000, 
            len:  0x8000,
            space: "code/data",
            access: "RWX"
        }],

        ["S07SHRAM", {
            comment: "64KB On-Chip Shared RAM Memory",
            name: "S07SHRAM",
            base: 0x20008000, 
            len:  0x10000,
            space: "code/data",
            access: "RWX"
        }],

        ["CTOMMSGRAM", {
            comment: "C28 to M3 MSG RAM Memory",
            name: "CTOMMSGRAM",
            base: 0x2007F000,
            len:  0x800,
            space: "data",
            access: "R"
        }],

        ["MTOCMSGRAM", {
            comment: "M3 to C28 MSG RAM Memory",
            name: "MTOCMSGRAM",
            base: 0x2007F800,
            len:  0x800,
            space: "data",
            access: "RW"
        }],
    ];
};

/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:23; /db/ztree/library/trees/platform/platform-p07/src/
 */

