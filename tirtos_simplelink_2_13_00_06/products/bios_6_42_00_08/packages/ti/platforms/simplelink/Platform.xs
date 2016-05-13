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
 *  ======== Platform.xs ========
 *  Generic platform support for Simplelink devices.
 */

var Boot = null;
var Utils = xdc.useModule('xdc.platform.Utils');

/*
 *  ======== Platform.getCpuDataSheet ========
 *  This Platform's implementation xdc.platform.IPlatform.getCpuDataSheet.
 *
 *  Return the xdc.platform.ICpuDataSheet.Instance object that corresponds
 *  to the device that runs executables loaded on the specified cpuId.
 */
function getCpuDataSheet(cpuId)
{
    var cpuAttrs = this.CPU.$copy();

    cpuAttrs.deviceName = this.deviceName;

    return (Utils.getCpuDataSheet(cpuAttrs));
}

/*
 *  ======== Platform.getExeContext ========
 *  This Platform's implementation xdc.platform.IPlatform.getExeContext.
 *
 *  Return the xdc.platform.IExeContext.Instance object that will run the
 *  specified program prog.
 */
function getExeContext(prog)
{
    /* create a default ExeContext execution context */
    var ExeContext = xdc.useModule('xdc.platform.ExeContext');

    var cpu = ExeContext.create(this.CPU, this.$module.BOARD);
    var revision = this.CPU.revision;

    /* This is where we have peripherals available, so we can get their base
     * addresses and other info.
     */

    cpu.memoryMap = Utils.assembleMemoryMap(cpu, this);
    cpu.deviceName = revision;
    cpu.revision = "";

    if (prog.build.target.$name.match(/gnu/)) {
        if (this.codeMemory == undefined) {
            this.codeMemory = "REGION_TEXT";
        }
        if (this.dataMemory == undefined) {
            this.dataMemory = "REGION_DATA";
        }
        if (this.stackMemory == undefined) {
            this.stackMemory = "REGION_STACK";
        }
    }
    /* check for the overlap in the memory map */
    var overlap = Utils.checkOverlap(cpu.memoryMap);

    if (overlap != null) {
        this.$module.$logError("Memory objects " + overlap + " overlap", this,
            overlap);
    }

    this.$seal();
    return (cpu);
}

/*
 *  ======== Platform.getExecCmd ========
 *  This Platform's implementation xdc.platform.IPlatform.getExecCmd.
 */
function getExecCmd(prog, packagePath)
{
    var os = environment["xdc.hostOS"];
    var doNotKnow = "@$(ECHO) ti.platforms.simplelink platform package cannot "
        + "execute " + prog.name + " on " + os + "\n";
    return (doNotKnow);
}

/*
 *  ======== Platform.getLinkTemplate ========
 *  This Platform's implementation xdc.platform.IPlatform.getLinkTemplate
 */
function getLinkTemplate(prog)
{
    if (prog.build.target.$name.match(/gnu/)) {
        return ("ti/platforms/simplelink/linkcmd_gnu.xdt");
    }
    else if (prog.build.target.$name.match(/iar/)) {
        return ("ti/platforms/simplelink/linkcmd_iar.xdt");
    }
    else {
        return ("ti/platforms/simplelink/linkcmd.xdt");
    }
}

/*
 *  ======== Platform.instance$meta$init ========
 *  This function is called to initialize a newly created instance of a
 *  platform.  Platform instances are created just prior to running
 *  program configuration scripts.
 *
 *  Platform instances may also be created in the build domain.
 *
 *  @param(name)        the name used to identify this instance (without
 *                      the package name prefix)
 *
 *  For this platform, 'name' must identify a device. Besides the catalog
 *  module, this parameter may encode values for other configuration
 *  parameters, as defined by `nameFormat`. The values are separated by ':'.
 */
function instance$meta$init(name)
{
    var maxParamsLength = this.$module.nameFormat.split(":").length;
    var nameParams = name.split(":");

    if (nameParams.length > maxParamsLength) {
        this.$module.$logWarning("The platform ti.platforms.simplelink accepts "
            + "only " + maxParamsLength + " instance parameters in its name. "
            + "The supplied string '" + name + "' contains additional values, "
            + "which will be ignored.", this, this.$module.nameFormat);
    }

    if (nameParams[0] != "" && this.deviceName != null
        && this.deviceName != nameParams[0]) {
        this.$module.$logError(this.$package.$name
            + " was passed two different device names: '" + nameParams[0]
            + "' and '" + this.deviceName + "'.", this, this.deviceName);
    }
    else if (nameParams[0] != "") {
        this.deviceName = nameParams[0];
    }

    if (this.deviceName == null) {
        this.$module.$logFatal("A device name must be specified for the "
            + "platform '" + this.$package.$name + "'. A device name can be "
            + "specified in the platform name, as "
            + "'ti.platforms.simplelink:CC3200', where CC3200 is the name "
            + "of the selected device.", this, this.deviceName);
    }

    if (nameParams[1] != "false" && Boolean(nameParams[1]).valueOf()) {
        this.includeLinkCmdFile = true;
    }

    if (nameParams[2] != undefined && !isNaN(parseFloat(nameParams[2]))) {
        this.clockRate = parseFloat(nameParams[2]);
    }

    if (this.externalMemoryMap.length != 0) {
        this.$module.$logWarning(this.$package.$name + " does not support "
            + "external memory map, and the parameter 'externalMemoryMap' is "
            + "ignored.", this, this.externalMemoryMap);
    }

    if (Program.build.target.$name.match(/gnu/)) {
        this.codeMemory = "REGION_TEXT";
        this.dataMemory = "REGION_DATA";
        this.stackMemory = "REGION_STACK";
    }

    /* if in the configuration model ... */
    if (xdc.om.$name == "cfg") {
        /* We use 'revision' to pass 'the real device name' to the generic
         * CC13xx, CC26xx, or CC32xx device.
         */
        this.CPU.revision = this.deviceName;

        /* resolve catalog device name and characteristics  */
        if (this.deviceName.match(/CC26.*/)) {
            if (nameParams[2] != undefined && !isNaN(parseFloat(nameParams[2]))) {
                this.CPU.clockRate = this.clockRate = parseFloat(nameParams[2]);
            }
            else {
                this.CPU.clockRate = this.clockRate = 48;
            }
            this.CPU.catalogName = "ti.catalog.arm.cortexm3";
            this.CPU.deviceName = "CC26xx";
            xdc.loadPackage(this.CPU.catalogName);
        }
        else if (this.deviceName.match(/CC13.*/)) {
            if (nameParams[2] != undefined && !isNaN(parseFloat(nameParams[2]))) {
                this.CPU.clockRate = this.clockRate = parseFloat(nameParams[2]);
            }
            else {
                this.CPU.clockRate = this.clockRate = 48;
            }
            this.CPU.catalogName = "ti.catalog.arm.cortexm3";
            this.CPU.deviceName = "CC13xx";
            xdc.loadPackage(this.CPU.catalogName);
        }
    }
}
/*
 *  @(#) ti.platforms.simplelink; 1, 0, 0,; 4-24-2015 20:40:51; /db/ztree/library/trees/platform/platform-p07/src/
 */

