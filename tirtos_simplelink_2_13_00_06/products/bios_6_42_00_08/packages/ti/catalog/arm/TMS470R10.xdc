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
 *  ======== TMS470R10.xdc ========
 */

package ti.catalog.arm;

/*!
 *  ======== TMS470R10 ========
 *  The C470 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 *
 *  Warning: The data for this module comes from the linker command file 
 *  shipped with the TI 470 tools and does NOT come from any published data
 *  sheet.
 */
metaonly module TMS470R10 inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string   cpuCore        = "470";
    override config string   isa            = "v4T";

    override config int     minProgUnitSize = 1;
    override config int     minDataUnitSize = 1;
    override config int     dataWordSize    = 4;

    override config string   cpuCoreRevision = "1.0";

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    config xdc.platform.IPlatform.Memory memMap[string]  = [
        ["P_MEM", {
            comment: "Program Memory (ROM)",
            name: "P_MEM",
            base: 0x00000000,
            len: 0x00030000,
            space: "code"
        }],

        ["D_MEM", {
            comment: "Data Memory (RAM)",
            name: "D_MEM", 
            base: 0x00030000,
            len: 0x00050000,
            space: "data"
        }],
    ];
};
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:18; /db/ztree/library/trees/platform/platform-p07/src/
 */

