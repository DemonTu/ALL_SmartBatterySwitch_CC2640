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
 *  ======== Platform.xdc ========
 */

package ti.platforms.simCM3;

/*!
 *  ======== Platform ========
 *  Simulation-based Platform support for TI Cortex M3 platforms
 *
 *  This module implements xdc.platform.IPlatform and defines configuration
 *  parameters that correspond to this platform's Cpu's, Board's, etc.
 *
 *  The configuration parameters are initialized in this package's
 *  configuration script (package.cfg) and "bound" to the TCOM object
 *  model.  Once they are part of the model, these parameters are
 *  queried by a program's configuration script.
 *
 *  This particular platform has a single Cpu, and therefore, only
 *  declares a single CPU configuration object.  Multi-CPU platforms
 *  would declare multiple Cpu configuration parameters (one per
 *  platform CPU).
 */
metaonly module Platform inherits xdc.platform.IPlatform
{
    /*!
     *  ======== BOARD ========
     *  This platform's board attributes
     */
    readonly config xdc.platform.IPlatform.Board BOARD = {
        id:             "0",
        boardName:      "simCM3",
        boardFamily:    "simCM3",
        boardRevision:  null
    };

    /*!
     *  ======== CPU ========
     *  The CPU simulated by this platform.
     *
     *  The device simulated is determined by the platform instance name
     *  or if this does not name a ti.catalog.arm module, then we
     *  we simulate a TMS470.
     */
    readonly config xdc.platform.IExeContext.Cpu CPU = {        
        id:             "0",
        clockRate:      200.0,
        catalogName:    "ti.catalog.arm.cortexm3",
        deviceName:     "CortexM3",
        revision:       "1.0",
    };

instance:

    override readonly config xdc.platform.IPlatform.Memory 
        externalMemoryMap[string] = [

            ["Code", {
                comment: "Code Memory",
                name:   "Code",
                base:   0x00000000,
                len:    0x20000000,
                space:  "code",
                access: "RWX"
            }],

            ["SRAM", {
                comment: "Shared RAM",
                name:   "SRAM", 
                base:   0x20000000,
                len:    0x20000000,
                space:  "data",
                access: "RW"
            }],
        ];

    override config string codeMemory = "Code";
    override config string dataMemory = "SRAM";
    override config string stackMemory = "SRAM";
};
/*
 *  @(#) ti.platforms.simCM3; 1, 0, 1, 0,; 4-24-2015 20:40:36; /db/ztree/library/trees/platform/platform-p07/src/
 */

