/* 
 *  Copyright (c) 2008 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */

/*!
 *  ======== IM4.xdc ========
 *  Cortex M4 little endian thumb2 target
 */
metaonly interface IM4 inherits ti.targets.arm.elf.IArm {
    override readonly config string isa         = "v7M4";

    override config string platform   = "ti.platforms.stellaris:LM4FSXLH5BB:1";

    override readonly config xdc.bld.ITarget.Model model = {
        endian: "little",
        codeModel: "thumb2",
        shortEnums: true
    };

    override readonly config xdc.bld.ITarget2.Command lnk = {
        cmd:  "armlnk",
        opts: "--silicon_version=7M4 --strict_compatibility=on"
    };
}
/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,5; 2-27-2015 12:11:34; /db/ztree/library/trees/xdctargets/xdctargets-i02/src/ xlibrary

 */

