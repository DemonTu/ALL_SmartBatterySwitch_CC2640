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
 *  ======== TMS570PSF762.xs ========
 *
 */

/*
 *  ======== TMS570PSF762.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device
 *  when the registers are set as specified by the regs hash.
 */
function getMemoryMap(regs)
{
    if ("flash_lo" in regs) {
        var flash_lo = regs.flash_lo;
    }
    else {
        var flash_lo = true;
    }
    if (flash_lo == true) {
        delete this.memMap["LO_IRAM"];
        delete this.memMap["HI_FLASH"];
    }
    else {
        delete this.memMap["HI_IRAM"];
        delete this.memMap["LO_FLASH"];
    }
    return (this.memMap);
}

/*
 *  ======== module$validate ========
 */
function module$validate() 
{
    this.$logWarning("This catalog module is deprecated.  The " +
        "catalog module ti.catalog.arm.cortexr4.TMS570PSF762 should be used " +
        "instead.", this);
}
/*
 *  @(#) ti.catalog.arm; 1, 0, 1,; 4-24-2015 20:37:18; /db/ztree/library/trees/platform/platform-p07/src/
 */

