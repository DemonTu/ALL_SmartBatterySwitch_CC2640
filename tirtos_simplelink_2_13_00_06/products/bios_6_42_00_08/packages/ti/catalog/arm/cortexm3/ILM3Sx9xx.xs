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
 *  ======== ILM3Sx9xx.xs ========
 *
 */

/*
 *  ======== ILM3Sx9xx.getMemoryMap ========
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
    var Timer = xdc.useModule("ti.catalog.arm.peripherals.timers.Timer");
    this.timer0 = Timer.create(
        {name: "Timer 0", baseAddr: 0x40030000, intNum: 35}
    );
    this.timer1 = Timer.create(
        {name: "Timer 1", baseAddr: 0x40031000, intNum: 37}
    );
    this.timer2 = Timer.create(
        {name: "Timer 2", baseAddr: 0x40032000, intNum: 39}
    );
    this.timer3 = Timer.create(
        {name: "Timer 3", baseAddr: 0x40033000, intNum: 51}
    );
}
/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

