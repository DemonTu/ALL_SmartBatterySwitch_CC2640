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
package ti.platforms.simplelink;

/*!
 *  ======== Platform ========
 *  A generic platform that supports any CC13xx, CC26xx, or CC32xx device
 *
 *  The device to be used by this platform is passed as the platform instance
 *  name. On the `xdc.tools.configuro` command line, it is done in the
 *  following way:
 *  @p(code)
 *  xs xdc.tools.configuro ... -p "ti.platforms.simplelink:CC3200"
 *  @p
 *
 *  In package.bld, the platform instance is selected as in:
 *  @p(code)
 *  Pkg.addExecutable("test", target, "ti.platforms.simplelink:CC3200");
 *  @p
 */
@Template ("./Platform.xdt")
metaonly module Platform inherits xdc.platform.IPlatform
{
    config xdc.platform.IPlatform.Board BOARD = {
        id:             "0",
        boardName:      "simplelink",
        boardFamily:    null,
        boardRevision:  null
    };

    /*!
     *  ======== nameFormat ========
     *  Encoding of instance creation parameters in the instance's name
     *
     *  For this platform, the parameters `deviceName`, `includeLinkCmdFile`
     *  and `clockRate` can be encoded in the instance name supplied on
     *  `xdc.tools.configuro` command line, for example:
     *  @p(code)
     *      xs xdc.tools.configuro ... -p ti.platforms.simplelink:CC3200:1:20
     *  @p
     *  Optional parameters can be omitted:
     *  @p(code)
     *      xs xdc.tools.configuro ... -p ti.platforms.simplelink:CC3200
     *  @p
     */
     readonly config string nameFormat
         = "$(deviceName):$(includeLinkCmdFile):$(clockRate)";

    /*!
     *  ======== useGnuRomLinkCmd ========
     *  Use ROM compatible linker script when building using GNU tools
     *
     *  If this config param is set to true and "includeLinkCmdFile" is true,
     *  the ROM compatible linker script will be used. The ROM compatible
     *  linker script is required when building a ROM based application.
     */
     config Bool useGnuRomLinkCmd = false;

instance:

    /*
     *  This platform supports either Cortex M3 or M4 cores.
     *  The corresponding "actual" catalogName is overwritten
     *  in Platform.instance$meta$init() based on the device name
     */

    config xdc.platform.IExeContext.Cpu CPU = {
        id:             "0",
        clockRate:      80.0,
        catalogName:    "ti.catalog.arm.cortexm4",
        deviceName:     "CC32xx",
        revision:       "",
    };

    /*!
     *  ======== deviceName ========
     *  The name of an `ICpuDataSheet` module for the device
     *
     *  This parameter is required, but it does not have to be set explicitly;
     *  it can be encoded in the instance's name.
     */
    config string deviceName;

    /*!
     *  ======== clockRate ========
     *  The clock rate for this device.
     */
    config Double clockRate;

    override config string codeMemory = null;

    override config string dataMemory = null;

    override config string stackMemory = null;

    /*!
     *  ======== includeLinkCmdFile ========
     *  The flag that specifies if the platform should include a linker command
     *  file.
     *
     *  By default, a user is responsible for adding a linker command file to
     *  the project, or to the linker command line. However, if this flag is
     *  set, this platform will include a default linker command file for the
     *  selected device.
     */
    config Bool includeLinkCmdFile = false;
};
/*
 *  @(#) ti.platforms.simplelink; 1, 0, 0,; 4-24-2015 20:40:51; /db/ztree/library/trees/platform/platform-p07/src/
 */

