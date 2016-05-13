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
 *  ======== TMS320CDM740.xs ========
 *
 */

/*
 *  ======== TMS320CDM740.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device
 *  when the registers are set as specified by the regs hash.
 */
function getMemoryMap(regs)
{
    return (this.memMap);
}

/*
 *  ======== module$validate ========
 */
function module$validate() 
{
    this.$logWarning("This catalog module is deprecated.  The " +
        "catalog module ti.catalog.arm.cortexm3.TMS320DM8168 should be used " +
        "instead.", this);
}
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:18; /db/ztree/library/trees/platform/platform-p07/src/
 */

