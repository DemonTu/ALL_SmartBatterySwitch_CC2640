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
 *  ======== DRA7XX.xdc ========
 *
 */

/*!
 *  ======== DRA7XX ========
 *  The DRA7XX device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is
 *  used by platforms to obtain "data sheet" information about this device.
 *
 */
metaonly module DRA7XX inherits ti.catalog.ICpuDataSheet
{
instance:
    config ti.catalog.peripherals.hdvicp2.HDVICP2.Instance hdvicp0;

    override config string cpuCore           = "CM4";
    override config string isa               = "v7M4";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    /*!
     *  ======== memMap ========
     *  The memory map returned be getMemoryMap().
     */
    config xdc.platform.IPlatform.Memory memMap[string] = [

        /*
         * AMMU mapped L2 ROM virtual address
         * Physical address is 0x55000000
         */
        ["L2_ROM", {
            name: "L2_ROM",
            base: 0x00000000,
            len:  0x00004000
        }],

        /*
         * AMMU mapped L2 RAM virtual address
         * Physical address is 0x55020000
         */
        ["L2_RAM", {
            name: "L2_RAM",
            base: 0x20000000,
            len:  0x00010000
        }],

        /*
         * On-chip RAM memory 
         */
        ["OCMC_RAM1", {
            comment:    "OCMC (On-chip RAM) Bank 1 (512KB)",
            name: "OCMC_RAM1",
            base: 0x40300000,
            len:  0x00080000
        }],

        /*
         * On-chip RAM memory 
         */
        ["OCMC_RAM2", {
            comment:    "OCMC (On-chip RAM) Bank 2 (1MB)",
            name: "OCMC_RAM2",
            base: 0x40400000,
            len:  0x00100000
        }],

        /*
         * On-chip RAM memory 
         */
        ["OCMC_RAM3", {
            comment:    "OCMC (On-chip RAM) Bank 3 (1MB)",
            name: "OCMC_RAM3",
            base: 0x40500000,
            len:  0x00100000
        }]
    ];
};
/*
 *  @(#) ti.catalog.arm.cortexm4; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

