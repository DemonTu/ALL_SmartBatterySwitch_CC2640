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
 *  ======== TNETV107X.xdc ========
 *
 */

metaonly module TNETV107X inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string cpuCore           = "Arm11";
    override config string isa               = "v6";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    config xdc.platform.IPlatform.Memory memMap[string] = [
        ["SRAM", {
            comment:    "On-Chip SRAM",
            name:       "SRAM",
            base:       0x20000000,
            len:        0x00010000,
            space:      "code/data",
            access:     "RWX"
        }],
    ];
}

/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:19; /db/ztree/library/trees/platform/platform-p07/src/
 */

