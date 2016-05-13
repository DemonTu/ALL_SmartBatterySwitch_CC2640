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
requires ti.catalog.peripherals.hdvicp2;

/*!
 *  ======== ti.catalog.arm.cortexm3 ========
 *  Package of devices for the arm Cortex M3 family of Arm processors.
 *
 *  Each module in this package implements the xdc.platform.ICpuDataSheet
 *  interface. This interface is used by platforms (modules that implement
 *  xdc.platform.IPlatform) to obtain the memory map supported by each CPU.
 */
package ti.catalog.arm.cortexm3 [1,0,0] {
    module OMAP4430;
    interface ILM3Sx7xx;
    module LM3S9792, LM3S9790, LM3S5791, LM3S2793;
    interface ILM3SxBxx;
    module LM3S9B96, LM3S9B95, LM3S9B92, LM3S5B91, LM3S9B90, LM3S2B93;
    interface ILM3Sx9xx;
    module LM3S8962;

    interface ITI8168;
    module TMS320DM8168;
    module TMS320TI816X;
    module TMS320C6A8168;

    interface ITI8148;
    module TMS320DM8148;
    module TMS320TI814X;

    module CC13xx, CC26xx, F28M35x, Stellaris, Tiva;

    module CortexM3;

    interface ITI813X;
    module TMS320TI813X;

    interface ITI811X;
    module TMS320TI811X;

    module TMS320C6A8149;
}
/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:25; /db/ztree/library/trees/platform/platform-p07/src/
 */

