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
 *  ======== TMS320CDM6446.xdc ========
 *
 */

metaonly module TMS320CDM6446 inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string cpuCore           = "Arm9";
    override config string isa               = "v5T";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    /*!
     *  ======== memMap ========
     *  The memory map returned be getMemoryMap().
     */
    config xdc.platform.IPlatform.Memory memMap[string]  = [
        ["IRAM0", {
            comment:    "Internal ARM Instruction RAM0",
            name:       "IRAM0",
            base:       0x00000000,
            len:        0x00002000,
            space:      "code",
            access:     "RWX"
        }],

        ["IRAM1", {
            comment:    "Internal ARM Instruction RAM1",
            name:       "IRAM1",
            base:       0x00002000,
            len:        0x00002000,
            space:      "code",
            access:     "RWX"
        }],

        ["DRAM0", {
            comment:    "Internal ARM Data RAM0",
            name:       "DRAM0",
            base:       0x00008000,
            len:        0x00002000,
            space:      "data",
            access:     "RW"
        }],

        ["DRAM1", {
            comment:    "Internal ARM Data RAM1",
            name:       "DRAM1",
            base:       0x0000A000,
            len:        0x00002000,
            space:      "data",
            access:     "RW"
        }],
    ];
}
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:18; /db/ztree/library/trees/platform/platform-p07/src/
 */

