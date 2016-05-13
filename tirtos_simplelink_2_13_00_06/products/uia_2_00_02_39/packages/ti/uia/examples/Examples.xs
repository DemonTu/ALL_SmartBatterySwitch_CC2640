var exampleCapsule = xdc.loadCapsule("ExampleList.xs");

var ProductGroup = environment["productGroup"];
var TirtosSupport = environment["tirtosSupport"];

//var devices                     = deviceCapsule.options;
var singleCoreExamples          = exampleCapsule.singleCoreExamples;
var NDKExamples                 = exampleCapsule.NDKExamples;
var MessageQExamples            = exampleCapsule.MessageQExamples;
var EVMTI816X_DSPExamples       = exampleCapsule.EVMTI816X_DSPExamples;
var EVMTI816X_VideoM3Examples   = exampleCapsule.EVMTI816X_VideoM3Examples;
var EVMTI816X_VpssM3Examples    = exampleCapsule.EVMTI816X_VpssM3Examples;
var examplesC6000               = exampleCapsule.examplesC6000;
var c28Examples                 = exampleCapsule.c28Examples;
var msp430Examples              = exampleCapsule.msp430Examples;
var rootString;

/* Read Target Options */
var TargetCapsule = xdc.loadCapsule("ti/sysbios/examples/TargetOptions.xs");
var TargetOptions = TargetCapsule.targetOptions;
var BiosExampesCapsule = xdc.loadCapsule("ti/sysbios/examples/Examples.xs");

/* Read Example List */
var ExampleCapsule = xdc.loadCapsule("ExampleList.xs");
var ExamplesList = ExampleCapsule.examplesList;


/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    var Examples = this;
    var curGroup = "";

//    deviceCapsule.makeFilterMSP430();
    /* Generate MSP430 deviceId */
//    var myTargetOptions = TargetCapsule.targetOptions;
    TargetCapsule.populateDeviceIdMSP430();

    /*
     *  Add special 'devices' to targetOptions for  UIA C6000-specific
     *  examples (stairstep JTAGRunMode) and evm6472 message example.
     */
    TargetOptions["TI"]["C6000"]["GENERIC"].devices["C6000"] = {
        cfgPrefix: "c6000/",
        deviceId:  "~.*(TMS320C670|TMS320C671|TMS320C672|" +
                       "DSK6713|GenericC62xxDevice|" +
                       "GenericC64xxDevice|GenericC67xxDevice|" +
                       "GenericC67xPlusDevice|C641).*"
    };

    TargetOptions["TI"]["C6000"]["GENERIC"].devices["C6472"] = {
        cfgPrefix: "evm6472/",
        deviceId:   ".*6472.*",
    };

    if (TirtosSupport == "false") {
        rootString = "ti.uia.examples.root";
        Examples.templateGroupArr.$add({
            id          : rootString,
            name        : "System Analyzer (UIA)",
            description : "System Analyzer (UIA) Tutorials, Templates and Examples"
        });
    }
    else {
        rootString = "examples.root_" + ProductGroup;
        Examples.templateGroupArr.$add({
            id          : rootString,
            name        : "TI-RTOS for " + ProductGroup,
            description : "TI-RTOS Templates and Examples"
        });
    }

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia",
        name        : "Instrumentation Examples",
        description : "Instrumentation Example Projects",
        groups      : [rootString]
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsTI",
        name        : "TI Target Examples",
        description : "System Analyzer (UIA) Examples for TI targets.",
        groups      : [rootString + ".uia"],
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsGNU",
        name        : "GNU Target Examples",
        description : "System Analyzer (UIA) Examples for GNU targets.",
        groups      : [rootString + ".uia"],
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsTI.singlecoreTI",
        name        : "Single-core Examples",
        description : "System Analyzer (UIA) Single-core Examples",
        groups      : [rootString + ".uia.targetsTI"],
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsGNU.singlecoreGNU",
        name        : "Single-core Examples",
        description : "System Analyzer (UIA) Single-core Examples",
        groups      : [rootString + ".uia.targetsGNU"],
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsTI.multicoreTI",
        name        : "Multi-core Examples",
        description : "System Analyzer (UIA) Multi-core Examples",
        groups      : [rootString + ".uia.targetsTI"]
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".uia.targetsGNU.multicoreGNU",
        name        : "Multi-core Examples",
        description : "System Analyzer (UIA) Multi-core Examples",
        groups      : [rootString + ".uia.targetsGNU"]
    });

    var ExModuleName = 'ti.uia.examples.Examples';
    BiosExampesCapsule.generateExampleTemplates(Examples, TargetOptions,
            ExamplesList, ExModuleName, ProductGroup, TirtosSupport);
}
