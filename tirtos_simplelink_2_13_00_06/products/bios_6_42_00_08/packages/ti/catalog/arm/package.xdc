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

requires xdc.platform;
requires ti.catalog.arm.peripherals.timers;

/*!
 *  ======== ti.catalog.arm ========
 *  Package of devices for the arm family of Arm processors.
 *
 *  Each module in this package implements the xdc.platform.ICpuDataSheet
 *  interface. This interface is used by platforms (modules that implement
 *  xdc.platform.IPlatform) to obtain the memory map supported by each CPU.
 */
package ti.catalog.arm [1,0,1] {
    interface ILM3Sx7xx;
    module LM3S9792, LM3S9790, LM3S5791, LM3S2793;
    interface ILM3SxBxx;
    module LM3S9B96, LM3S9B95, LM3S9B92, LM3S5B91, LM3S9B90, LM3S2B93;
    interface ILM3Sx9xx;
    module LM3S8962;
    module TMS470R10;
    module TMS320C5912;
    module TMS320C1030;
    module TMS320CDM6446, TMS320CDM6467;
    module TMS320DM355, TMS320DM357, TMS320DM365;
    module TMS320CDM510;
    interface IOMAP2x3x;
    module OMAP2430, OMAP2431, OMAP2530, OMAP2531;
    interface IOMAP3xxx;
    module OMAP3403, OMAP3405, OMAP3503, OMAP3505, OMAP3515, OMAP3517;
    module OMAP3425, OMAP3525, TMS320C3430, OMAP3530;
    interface ITMS320DA8xx;
    module TMS320DA828, TMS320DA830;
    module OMAPL137;
    module CortexM3;
    module TMS570PSF762, TMS570LS20216;
    module TMS320DA840, TMS320DA850;
    module AM1705, AM1707;
    module AM1806, AM1808;
    module OMAPL108, OMAPL118, OMAPL138;
    module TMS320CDM730, TMS320CDM740;
    module TNETV107X;
}
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:19; /db/ztree/library/trees/platform/platform-p07/src/
 */

