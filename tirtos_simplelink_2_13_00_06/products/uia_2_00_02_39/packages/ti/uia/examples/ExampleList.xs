var productGroup = environment["productGroup"];
var tirtosSupport = environment["tirtosSupport"];
var rootString = "ti.uia.examples.root";

if (tirtosSupport == "true") {
    rootString = "examples.root_" + productGroup;
}

//var singleCoreExamples = [
var examplesList = [
    /* Hello world */
    {
        groupsTI: [rootString + ".uia.targetsTI.singlecoreTI"],
        groupsGNU: [rootString + ".uia.targetsGNU.singlecoreGNU"],
        title: "Hello World",
        name: "helloWorld",
        description: "A basic example that logs a hello world string that can be viewed using System Analyzer.",
        root: "./helloworld/",
        cFile: "main.c",
        cfgFile: "helloWorld.cfg",
        fileList: [],
        isHybrid: true,
        requiredProducts: [ "com.ti.rtsc.SYSBIOS" ],
        options: "NPW,TREX",
    },
    /* JTAG StopMode */
    {
        groupsTI: [rootString + ".uia.targetsTI.singlecoreTI"],
        groupsGNU: [rootString + ".uia.targetsGNU.singlecoreGNU"],
        title: "Stairstep JTAG StopMode",
        name: "stairstepStopMode",
        description: "The stairstep example runs continuously and periodically generates log events. Different threads (Hwi, Swi, Task) run to present a CPU load to the system.  The Log data is obtained when the target is halted.  The sizes of the loggers in LoggingSetup might need to be decreased if your target does not have enough memory.",
        root: "./single/",
        cFile: "stairstep.c",
        cfgFile: "stairstep_JTAGStopMode.cfg",
        fileList: [],
        isHybrid: true,
        requiredProducts: [ "com.ti.rtsc.SYSBIOS" ],
        options: "NPW,TREX",
    },
    /* JTAG RunMode */
    {
        groupsTI: [rootString + ".uia.targetsTI.singlecoreTI"],
        groupsGNU: [rootString + ".uia.targetsGNU.singlecoreGNU"],
        family: "C6000",
        title: "Stairstep JTAG RunMode",
        name: "stairstepRunMode",
        root: "./single/",
        cFile: "stairstep.c",
        cfgFile: "stairstep_JTAGRunMode.cfg",
        fileList: [],
        description: "The stairstep example runs continuously and periodically generates log events. Different threads (Hwi, Swi, Task) run to present a CPU load to the system. The Log data is obtained by reading the running target (only works on C64X+ and C66 CPUs). The sizes of the loggers in LoggingSetup might need to be decreased if your target does not have enough memory.",
        isHybrid: true,
        requiredProducts: [ "com.ti.rtsc.SYSBIOS" ],
        options: "NPW,TREX",
    },
    /* NDK example */
    {
        groupsTI: [rootString + ".uia.targetsTI.singlecoreTI"],
        family: "C6000",
        variant: "GENERIC",
        device: "GENERIC",
        deviceId: ".*6472.*",
        title: "Stairstep Ethernet",
        name: "stairstepEthernet",
        description: "The stairstep example runs continuously and periodically generates log events. Different threads (Hwi, Swi, Task) run to present a CPU load to the system. Log data is obtained via Ethernet. The sizes of the loggers in LoggingSetup might need to be decreased if your target does not have enough memory.  This example, as is, only works for the EVM6472 board. Refer to the bottom of the .cfg file on how to use on a different board.",
        root: "./single/",
        cFile: "stairstep.c",
        cfgFile: "stairstep_Ethernet.cfg",
        fileList: [],
        isHybrid: true,
        requiredProducts: [ "com.ti.rtsc.SYSBIOS", "com.ti.rtsc.NDK" ],
        options: "NPW,TREX",
    },
    /* MessageQ example */
    {
        groups:[rootString + ".uia.multicore"],
        groupsTI: [rootString + ".uia.targetsTI.multicoreTI"],
        family: "C6000",
        variant: "GENERIC",
        device: "GENERIC",
        deviceId: ".*6472.*",
        title: "MessageQ (single image for all cores)",
        name: "MessageQ",
        description: "The message example shows how to use MessageQ on EVM6472 and UIA. It is a single-image example that must be loaded on all cores. A message is sent from core 0 to core 1 to core 2...back to core 0. The UIABenchmark_start and stop is used on core 0 to determine the duration of a round trip. The Log data is obtained via Ethernet.",
        root: "./evm6472/",
        cFile: "message.c",
        cfgFile: "message.cfg",
        fileList: [],
        isHybrid: true,
        requiredProducts: [ "com.ti.rtsc.SYSBIOS", "com.ti.rtsc.NDK", "com.ti.rtsc.IPC" ],
        options: "NPW,TREX",
    },
];
