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
 * 
 */
/*
 *  ======== package.xdc ========
 *
 */

requires xdc.bld;
requires ti.targets;

/*!
 *  ======== ti.targets.arm.elf ========
 *  Package of Arm targets for TI code generation tools for ELF
 *
 */
package ti.targets.arm.elf [1,0,0] {
    interface IArm, IR4, IR4_big_endian;
    interface IA15, IA8, IM3, IM3_big_endian, IM4, IM0;
    module Arm7, Arm7_big_endian, Arm9, Arm9t;
    module M0;
    module M3, M3_big_endian;
    module M4, M4F;
    module A8F, A8Ft;
    module A8Fv, A8Fnv;
    module A15Fnv;
    module R4, R4_big_endian;
    module R4t, R4t_big_endian;
    module R4F, R4F_big_endian;
    module R4Ft, R4Ft_big_endian;
}

/*
 *  @(#) ti.targets.arm.elf; 1, 0, 0,5; 2-27-2015 12:11:35; /db/ztree/library/trees/xdctargets/xdctargets-i02/src/ xlibrary

 */

