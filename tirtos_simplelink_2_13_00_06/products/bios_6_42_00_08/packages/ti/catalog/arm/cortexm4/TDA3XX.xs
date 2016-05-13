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
 *  ======== TDA3XX.xs ========
 *
 */

/*
 *  ======== TDA3XX.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device
 *  when the registers are set as specified by the regs hash.
 */
function getMemoryMap(regs)
{
    return (this.memMap);
}

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init()
{
}

/*
 *  @(#) ti.catalog.arm.cortexm4; 1, 0, 0,; 4-24-2015 20:37:25; /db/ztree/library/trees/platform/platform-p07/src/
 */

