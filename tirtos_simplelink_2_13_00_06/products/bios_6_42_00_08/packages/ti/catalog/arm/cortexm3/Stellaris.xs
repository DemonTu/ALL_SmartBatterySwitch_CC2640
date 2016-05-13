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
 *  ======== Stellaris.xs ========
 *
 */

var peripheralsTable = {};

peripheralsTable["2TIMERS"] = [
    {
        factory: "ti.catalog.arm.peripherals.timers.Timer",
        params: {name: "Timer0", baseAddr: 0x40030000, intNum: 35}
    },
    {
        factory: "ti.catalog.arm.peripherals.timers.Timer",
        params: {name: "Timer1", baseAddr: 0x40031000, intNum: 37}
    }
];

peripheralsTable["3TIMERS"] = [];
for (var i = 0; i < peripheralsTable["2TIMERS"].length; i++) {
    peripheralsTable["3TIMERS"][i] = {};
    for (var prop in peripheralsTable["2TIMERS"][i]) {
        peripheralsTable["3TIMERS"][i][prop]
            = peripheralsTable["2TIMERS"][i][prop];
    }
}
peripheralsTable["3TIMERS"][i] = {
    factory: "ti.catalog.arm.peripherals.timers.Timer",
    params: {name: "Timer2", baseAddr: 0x40032000, intNum: 39}
};

peripheralsTable["4TIMERS"] = [];
for (var i = 0; i < peripheralsTable["3TIMERS"].length; i++) {
    peripheralsTable["4TIMERS"][i] = {};
    for (var prop in peripheralsTable["3TIMERS"][i]) {
        peripheralsTable["4TIMERS"][i][prop]
            = peripheralsTable["3TIMERS"][i][prop];
    }
}
peripheralsTable["4TIMERS"][i] = {
    factory: "ti.catalog.arm.peripherals.timers.Timer",
    params: {name: "Timer3", baseAddr: 0x40033000, intNum: 51}
};

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(revision)
{
    this.$private.realDevice = revision;

    //var Boot = xdc.useModule('ti.catalog.arm.cortexm3.lm3init.Boot');

    var tableEntry;    
    /*
     * Define which actual devices correspond to the above defined
     * descriptor tables. Also, additional configurations important
     * to SYS/BIOS are made here.
     */
    if (this.$private.realDevice.match(/^LM3S\d\d76/)
        || this.$private.realDevice.match(/^LM3S\d110/)
        || this.$private.realDevice.match(/^LM3S\d\d52/)
        || this.$private.realDevice.match(/^LM3S\d4\d\d/)
        || this.$private.realDevice.match(/^LM3S1\D1\d/)
        || this.$private.realDevice.match(/^LM3S3\w26/)
        || this.$private.realDevice.match(/^LM3S(1|2)\d39/)) {
        tableEntry = "3TIMERS";
    }
    else if (this.$private.realDevice.match(/^LM3S5\D3(1|6)/)
        && !this.$private.realDevice.match(/^LM3S5\Dr\w/)) {
        tableEntry = "3TIMERS";
    }
    else if (this.$private.realDevice.match(/^LM3S\d\d\d$/)
        && !this.$private.realDevice.match(/^LM3S(1|3)0(1|2)$/)) {
        tableEntry = "3TIMERS";
    }
    else if (this.$private.realDevice.match(/^LM3S(1|3)0(1|2)$/)
        || this.$private.realDevice == "LM3S2016") {
        tableEntry = "2TIMERS";
    }
    else if (this.$private.realDevice == "LM3S1620"
        || this.$private.realDevice == "LM3S1751"
        || this.$private.realDevice == "LM3S1850"
        || this.$private.realDevice == "LM3S1937"
        || this.$private.realDevice == "LM3S2730"
        || this.$private.realDevice == "LM3S3634"
        || this.$private.realDevice == "LM3S5632"
        || this.$private.realDevice == "LM3S5662"
        || this.$private.realDevice == "LM3S5732"
        || this.$private.realDevice == "LM3S5737"
        || this.$private.realDevice == "LM3S5747"
        || this.$private.realDevice == "LM3S5762"
        || this.$private.realDevice == "LM3S6100"
        || this.$private.realDevice == "LM3S6730") {
        tableEntry = "3TIMERS";
    }
    else {
        tableEntry = "4TIMERS";
    }

    if (peripheralsTable[tableEntry] != null) {
        for (var i = 0; i < peripheralsTable[tableEntry].length; i++) {
            var entry = peripheralsTable[tableEntry][i];
            var modName = entry.factory;
            var mod = xdc.useModule(modName);
            var params = entry.params;

            /* construct the create arguments */
            var args = [];
            args.push(params);

            /* construct the peripheral */
            this.peripherals[params.name] = mod.create.$apply(mod, args);
        }
    }
}

/*
 *  ======== Stellaris.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device.
 */
function getMemoryMap(regs)
{
    return (null);
}
/*
 *  @(#) ti.catalog.arm.cortexm3; 1, 0, 0,; 4-24-2015 20:37:24; /db/ztree/library/trees/platform/platform-p07/src/
 */

