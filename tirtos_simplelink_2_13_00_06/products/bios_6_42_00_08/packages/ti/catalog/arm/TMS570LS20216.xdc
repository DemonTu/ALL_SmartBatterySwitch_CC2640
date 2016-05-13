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
 *  ======== TMS570LS20216.xdc ========
 */
package ti.catalog.arm;

/*!
 *  ======== TMS570LS20216 ========
 *  The C470 device data sheet module.
 *
 *  This module implements the xdc.platform.ICpuDataSheet interface and is 
 *  used by platforms to obtain "data sheet" information about this device.
 *
 *  Warning: The data for this module comes from the linker command file 
 *  shipped with the TI 470 tools and does NOT come from any published data
 *  sheet.
 */
metaonly module TMS570LS20216 inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string cpuCore           = "R4";
    override config string isa               = "v7R";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;

    /*!
     *  ======== memMap ========
     *  The default memory map for this device
     */
    config xdc.platform.IPlatform.Memory memMap[string]  = [
        ["LO_FLASH", {
            name:       "FLASH",
            base:       0x00000000,
            len:        0x00200000,
            space:      "code",
            access:     "RX"
        }],
        ["HI_IRAM", {
            name:       "IRAM",
            base:       0x08000000,
            len:        0x00028000,
            space:      "code/data",
            access:     "RWX"
        }],
        ["HI_FLASH", {
            name:       "FLASH",
            base:       0x20000000,
            len:        0x00080000,
            space:      "code",
            access:     "RX"
        }],
        ["LO_IRAM", {
            name:       "IRAM",
            base:       0x00000000,
            len:        0x00028000,
            space:      "code/data",
            access:     "RWX"
        }],
    ];
};

/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:18; /db/ztree/library/trees/platform/platform-p07/src/
 */

