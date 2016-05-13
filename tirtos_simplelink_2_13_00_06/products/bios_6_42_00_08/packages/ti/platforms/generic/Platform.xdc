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

package ti.platforms.generic;

/*!
 *  ======== Platform ========
 *  A generic platform that supports any HW platform
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
    config xdc.platform.IPlatform.Board BOARD = { 
        id:             "0",
        boardName:      "generic",
        boardFamily:    null,
        boardRevision:  null
    };

instance:

    config xdc.platform.IExeContext.Cpu CPU = { 
        id:             "0",
        clockRate:      0,
        catalogName:    null,
        deviceName:     null,
        revision:       "",
    };

    /*!
     *  ======== deviceName ========
     *  The CPU simulated by this simulator platform.
     *
     *  This parameter is required.
     */
    config string deviceName;

    /*!
     *  ======== catalogName ========
     *  The name of the package that contains the module 'deviceName'.
     *
     *  This parameter is required.
     */
    config string catalogName;

    /*!
     *  ======== clockRate ========
     *  The clock rate in MHz for the simulated device.
     *
     *  This parameter is required. There are no checks if the simulated device
     *  supports the given clock rate.
     */
    config Double clockRate;

    override config string codeMemory = null;
    
    override config string dataMemory = null;

    override config string stackMemory = null;

    /*
     *  ======== l1PMode ========
     *  Define the amount of L1P RAM used for L1 Program Cache on C6000 devices.
     *
     *  Check the device documentation for valid values.
     */
    config String l1PMode = "32k";
    
    /*
     *  ======== l1DMode ========
     *  Define the amount of L1D RAM used for L1 Data Cache on C6000 devices.
     *
     *  Check the device documentation for valid values.
     */
    config String l1DMode = "32k";
    
    /*
     *  ======== l2Mode ========
     *  Define the amount of L2 RAM used for L2 Cache on C6000 devices.
     *
     *  Check the device documentation for valid values.
     */
    config String l2Mode = "0k";
};
/*
 *  @(#) ti.platforms.generic; 1, 0, 0, 1,; 4-24-2015 20:39:55; /db/ztree/library/trees/platform/platform-p07/src/
 */

