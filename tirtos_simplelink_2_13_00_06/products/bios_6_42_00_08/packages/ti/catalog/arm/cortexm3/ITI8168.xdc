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
 *  ======== ITI8168.xdc ========
 *
 */

/*!
 *  ======== ITI8168 ========
 *  An interface implemented by all TI8168 devices
 *
 *  This interface is defined to factor common data about all TI8168 type devices
 *  into a single place; they all have the same internal memory.
 */
metaonly interface ITI8168 inherits ti.catalog.ICpuDataSheet
{
instance:
    config ti.catalog.peripherals.hdvicp2.HDVICP2.Instance hdvicp0;
    config ti.catalog.peripherals.hdvicp2.HDVICP2.Instance hdvicp1;
    config ti.catalog.peripherals.hdvicp2.HDVICP2.Instance hdvicp2;

    override config string cpuCore           = "CM3";
    override config string isa               = "v7M";
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
         * AMMU mapped L2 BOOT virtual address
         * Physical address is 0x5502_0000, virt is 0x00000000
         * Reset vectors and other boot code is placed here.
         *
         * Note that actual L2 RAM is 256K starting at 0x5502_0000.
         * The first 16K is reserved for reset vectors (i.e. L2_BOOT).
         * The remaining 240K is placed in the L2_RAM definition.
         */
        ["L2_BOOT", {
            name: "L2_BOOT",
            base: 0x00000000,
            len:  0x4000
        }],

        /*
         * AMMU mapped L2 RAM virtual address
         * Physical address is 0x5502_4000, virt is 0x2000_4000
         *
         * Note that actual L2 RAM is 256K starting at 0x5502_0000.
         * The first 16K is reserved for reset vectors (i.e. L2_BOOT).
         * The remaining 240K is placed in the L2_RAM definition.
         */
        ["L2_RAM", {
            name: "L2_RAM",
            base: 0x20004000,
            len:  0x3C000
        }],

        /*
         * OCMC (On-chip RAM) Bank 0
         * Physical address is 0x40300000
         * Size is 256K
         */
        ["OCMC_0", {
            name: "OCMC_0",
            base: 0x00300000,
            len:  0x40000
        }],

        /*
         * OCMC (On-chip RAM) Bank 1
         * Physical address is 0x40400000
         * Size is 256K
         */
        ["OCMC_1", {
            name: "OCMC_1",
            base: 0x00400000,
            len:  0x40000
        }],
    ];
};
/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

