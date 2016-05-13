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

/*!
 *  ======== Stellaris.xdc ========
 *  Generic Stellaris Cpu definition
 *
 *  This device represents all Stellaris devices independent.
 */

metaonly module Stellaris inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string   cpuCore        = "CM4";
    override config string   isa            = "v7M4";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;
}
/*
 *  @(#) ti.catalog.arm.cortexm4; 1, 0, 0,; 4-24-2015 20:37:25; /db/ztree/library/trees/platform/platform-p07/src/
 */

