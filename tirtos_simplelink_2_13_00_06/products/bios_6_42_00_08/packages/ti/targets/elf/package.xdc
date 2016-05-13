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

/*!
 *  ======== ti.targets.elf ========
 *  Package of targets for TI code generation tools for ELF
 *
 */
package ti.targets.elf [1,0,0] {
    interface ITarget;
    module C64P, C674, C64T, C67P;
    module C64P_big_endian, C674_big_endian, C64T_big_endian;
    module C66, C66_big_endian;
}
/*
 *  @(#) ti.targets.elf; 1, 0, 0,5; 2-27-2015 12:11:37; /db/ztree/library/trees/xdctargets/xdctargets-i02/src/ xlibrary

 */

