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
 *  ======== Tiva.xdc ========
 *  Generic Tiva Cpu definition
 *
 *  This device represents all Tiva devices independent.
 */

metaonly module Tiva inherits ti.catalog.ICpuDataSheet
{
instance:
    override config string   cpuCore        = "CM3";
    override config string   isa            = "v7M";
    override config string cpuCoreRevision   = "1.0";
    override config int    minProgUnitSize   = 1;
    override config int    minDataUnitSize   = 1;
    override config int    dataWordSize      = 4;
}
/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:25; /db/ztree/library/trees/platform/platform-p07/src/
 */

