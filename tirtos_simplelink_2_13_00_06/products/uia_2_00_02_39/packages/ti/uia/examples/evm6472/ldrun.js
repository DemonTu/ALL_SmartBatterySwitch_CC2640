//
// Use this script for loading BIOS remote example servers and apps
// on evmC6472:
// Copy this file to the example directory, eg
//     examples/C6472/ex01_universalcopy_bios_remote
// Edit 'dirbase' below to have the correct path to the example
// In CCS5, open the scripting console
//     Window -> Show View -> Scripting Console
// In Scripting Console window, type
//     loadJSFile i:\jeh\ce-u\....\ldrun.js
//
//print(env.getCurrentDirectory());
var numCores = ds.getSessionCount();
var core = new Array(numCores);
var coreNames = ds.getSessionNames();

/*
 *  coreNames is random(!), so sort it alphabetically for some order
 */
coreNames.sort();

/* connect to all cores */
for (var i = 0; i < numCores; i++) {
//  print (coreNames[i]);
  core[i] = ds.openSession(coreNames[i]);
  core[i].target.disconnect();
  core[i].target.connect();
  core[i].target.reset();
}

/* Set this to the location of the executables */
var dirbase = "v:/jeh/uia-h/src/ti/uia/examples/evm6472/bin/ti_platforms_evm6472_core0/debug/";

core[0].memory.loadProgram(dirbase + "message.x64P");
for (i = 1; i < numCores; i++) {
    core[i].memory.loadProgram(dirbase + "message.x64P");
}

/* run all cores - servers first */
for (i = 1; i < numCores; i++) {
//    core[i].target.run();
}
//core[0].target.run();
