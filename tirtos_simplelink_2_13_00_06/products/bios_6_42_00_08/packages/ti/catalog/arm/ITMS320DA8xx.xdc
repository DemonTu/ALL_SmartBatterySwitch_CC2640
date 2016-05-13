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
 *  ======== ITMS320DA8xx.xdc ========
 *
 */

metaonly interface ITMS320DA8xx inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string cpuCore           = "v5T";
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
        ["IRAM", {
            comment:    "8KB ARM local RAM",
            name:       "IRAM",
            base:       0xFFFF0000,
            len:        0x00002000,
            space:      "code/data",
            access:     "RWX"
        }],
        ["L3_CBA_RAM", {
            comment:    "128KB ARM/DSP local shared RAM",
            name:       "L3_CBA_RAM",
            base:       0x80000000,
            len:        0x00020000,
            space:      "code/data",
            access:     "RWX"
        }],
    ];
}
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:17; /db/ztree/library/trees/platform/platform-p07/src/
 */

