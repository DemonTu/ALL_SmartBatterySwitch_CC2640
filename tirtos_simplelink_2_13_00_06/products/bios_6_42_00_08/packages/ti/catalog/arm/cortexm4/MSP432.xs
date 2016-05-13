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
 *  ======== MSP432.xs ========
 *
 */

var peripheralsTable = {};

peripheralsTable["0TIMERS"] = [
];

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(revision)
{
    this.$private.realDevice = revision;

    var tableEntry;    

    /*
     * Define which actual devices correspond to the above defined
     * descriptor tables. Also, additional configurations important
     * to SYS/BIOS are made here.
     */
    if (this.$private.realDevice.match(/^MSP432/)) {
        tableEntry = "0TIMERS";
    }
}

/*
 *  ======== MSP432.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device.
 */
function getMemoryMap(regs)
{
    return (null);
}
/*
 *  @(#) ti.catalog.arm.cortexm4; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

