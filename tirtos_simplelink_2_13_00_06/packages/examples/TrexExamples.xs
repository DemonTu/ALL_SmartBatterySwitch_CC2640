Boards = xdc.loadCapsule("Boards.xs");
var ExamplesList = xdc.loadCapsule("ExampleList.xs");

/* Get the xdctools version from the passed in environment variable */
var XDCversion = environment["xdcver"];
var productGroup = environment["productGroup"];
var tirtosSupport = environment["tirtosSupport"];

XDCtoolsVer = "";
if ((XDCversion != "") && typeof XDCversion != 'undefined') {
    XDCtoolsVer =  XDCversion.match(/((\d+)_)+\w*/g)[0].replace(/_(\d+)/g, ".$1");
}
else {
    print("remark: XDCtools version: \'" + XDCversion + "\'");
}

var biosExamples = xdc.module("ti.sysbios.examples.Examples");
var uiaExamples = xdc.module("ti.uia.examples.Examples");
var rootString = "examples.root_" + productGroup;

/*
 *  ======== parseExamples ========
 */
function parseExamples(Examples, board, tool)
{
    for (var i = 0; i < ExamplesList.allExamples.length; i++) {
        var example = ExamplesList.allExamples[i];
        if ((ExamplesList.supportsTool(example, tool))  &&
            (ExamplesList.supportsBoard(example, board)) &&
            (Boards.supportsTool(board, tool))) {

            /* Add the one example to the one board */
            addExamples(Examples, board, example, tool);
        }

    }
}

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    var Examples = this;
    var found = false;

    Examples.templateGroupArr.$add({
        id          : rootString,
        name        : "TI-RTOS Examples",
        description : "TI-RTOS Templates and Examples"
    });

    Examples.templateGroupArr.$add({
        id          : rootString + ".driver",
        name        : "Driver Examples",
        description : "TI-RTOS Driver examples",
        groups      : [rootString]
    });

    /* Generate Board Trex Tree */
    for (var i = 0; i < Boards.allBoards.length; i++) {

        if (Boards.supportsTool(Boards.allBoards[i], "TI")) {
            found = createBoardRoot(Examples, Boards.allBoards[i], productGroup);
            if (found == true) {
                createBoardTree(Examples, Boards.allBoards[i], "TI");
                parseExamples(Examples, Boards.allBoards[i], "TI");
            }
        }
        if (Boards.supportsTool(Boards.allBoards[i], "GNU")) {
            found = createBoardRoot(Examples, Boards.allBoards[i], productGroup);
            if (found == true) {
                createBoardTree(Examples, Boards.allBoards[i], "GNU");
                parseExamples(Examples, Boards.allBoards[i], "GNU");
            }
        }
    }
    for(var i = 0; i < biosExamples.templateArr.length; i++) {
        if (biosExamples.templateArr[i].name == "benchmark") {
            continue;
        }

        /* Change the BIOS_CG_ROOT path to point to the TIRTOS BIOS installation */
        if (biosExamples.templateArr[i].linkerBuildOptions != undefined &&
            biosExamples.templateArr[i].linkerBuildOptions != "") {
            var tirtosBIOSDir = "{COM_TI_RTSC_TIRTOS" + productGroup.toUpperCase()
                                + "_INSTALL_DIR}/products/" + Boards.BIOSVer;

            while (biosExamples.templateArr[i].linkerBuildOptions.indexOf("{xdc_find:") != -1) {
                biosExamples.templateArr[i].linkerBuildOptions =
                    biosExamples.templateArr[i].linkerBuildOptions.replace("{xdc_find:",
                    tirtosBIOSDir + "/");
            }
            while (biosExamples.templateArr[i].linkerBuildOptions.indexOf(":${ProjName}}") != -1) {
                biosExamples.templateArr[i].linkerBuildOptions =
                    biosExamples.templateArr[i].linkerBuildOptions.replace(":${ProjName}}", "");
            }
        }

        for (var j = 0; j < (biosExamples.templateArr[i].fileList.length); j++) {
            biosExamples.templateArr[i].fileList[j].path = "ti/sysbios/examples/" +
                biosExamples.templateArr[i].fileList[j].path;
        }
        Examples.templateArr.$add(biosExamples.templateArr[i]);
    }

    for(var i = 0; i < biosExamples.templateGroupArr.length; i++) {
        Examples.templateGroupArr.$add(biosExamples.templateGroupArr[i]);
    }

    for(var i = 0; i < uiaExamples.templateArr.length; i++) {
        if (uiaExamples.templateArr[i].name == "benchmark") {
            continue;
        }

        /* TI-RTOS has all the products needed for UIA examples */
        uiaExamples.templateArr[i].requiredProducts = [];

        /* Change the BIOS_CG_ROOT path to point to the TIRTOS BIOS installation */
        if (uiaExamples.templateArr[i].linkerBuildOptions != undefined &&
            uiaExamples.templateArr[i].linkerBuildOptions != "") {
            var tirtosBIOSDir = "{COM_TI_RTSC_TIRTOS" + productGroup.toUpperCase()
                                + "_INSTALL_DIR}/products/" + Boards.BIOSVer;

            while (uiaExamples.templateArr[i].linkerBuildOptions.indexOf("{xdc_find:") != -1) {
                uiaExamples.templateArr[i].linkerBuildOptions =
                    uiaExamples.templateArr[i].linkerBuildOptions.replace("{xdc_find:",
                    tirtosBIOSDir + "/");
            }
            while (uiaExamples.templateArr[i].linkerBuildOptions.indexOf(":${ProjName}}") != -1) {
                uiaExamples.templateArr[i].linkerBuildOptions =
                    uiaExamples.templateArr[i].linkerBuildOptions.replace(":${ProjName}}", "");
            }
        }

        for (var j = 0; j < (uiaExamples.templateArr[i].fileList.length); j++) {
            uiaExamples.templateArr[i].fileList[j].path = "ti/uia/examples/" +
                uiaExamples.templateArr[i].fileList[j].path;
        }
        Examples.templateArr.$add(uiaExamples.templateArr[i]);
    }

    for(var i = 0; i < uiaExamples.templateGroupArr.length; i++) {
        Examples.templateGroupArr.$add(uiaExamples.templateGroupArr[i]);
    }

    /* EK-TM4C123GXL Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.EK_TM4C123GXL.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the EK-TM4C123GXL",
        groups      : [rootString + ".driver.EK_TM4C123GXL"]
    });
    /* CC3200_LP Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.CC3200_LP.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the CC3200_LP",
        groups      : [rootString + ".driver.CC3200_LP"]
    });
    /* CC1350DK Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.CC1350DK.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the CC1350EM on the SmartRF06 evalation board",
        groups      : [rootString + ".driver.CC1350DK"]
    });
    /* CC2650DK Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.CC2650DK.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the CC2650EM on the SmartRF06 evaluation board",
        groups      : [rootString + ".driver.CC2650DK"]
    });
    /* CC2650STK_BLE Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.CC2650STK_BLE.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the CC2650 Sensor Tag evaluation board",
        groups      : [rootString + ".driver.CC2650STK_BLE"]
    });
    /* DK-TM4C123G Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.DK_TM4C123G.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the DK_TM4C123G",
        groups      : [rootString + ".driver.DK_TM4C123G"]
    });
    /* DK-TM4C129X Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.DK_TM4C129X.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the DK-TM4C129X",
        groups      : [rootString + ".driver.DK_TM4C129X"]
    });
    /* EK-TM4C1294XL Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.EK_TM4C1294XL.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the EK-TM4C1294XL",
        groups      : [rootString + ".driver.EK_TM4C1294XL"]
    });
    /* MSP-EXP430F5529LP Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.MSP_EXP430F5529LP.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the MSP-EXP430F5529LP",
        groups      : [rootString + ".driver.MSP_EXP430F5529LP"]
    });
    /* MSP-EXP430F5529 Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.MSP_EXP430F5529.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the MSP_EXP430F5529",
        groups      : [rootString + ".driver.MSP_EXP430F5529"]
    });
    /* MSP-EXP430FR5969LP Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.MSP_EXP430FR5969LP.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the MSP-EXP430FR5969LP",
        groups      : [rootString + ".driver.MSP_EXP430FR5969LP"]
    });
    /* MSP-EXP432P401R Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.MSP_EXP432P401RLP.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the MSP_EXP432P401RLP",
        groups      : [rootString + ".driver.MSP_EXP432P401RLP"]
    });
    /* TMDXDOCK28M36 Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.TMDXDOCK28M36_M3.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the TMDXDOCK28M36",
        groups      : [rootString + ".driver.TMDXDOCK28M36_M3"]
    });
    /* TMDXDOCKH52C1 Groups */
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver.TMDXDOCKH52C1_M3.example",
        name        : "Example Projects",
        description : "TI-RTOS Example Projects for the TMDXDOCKH52C1",
        groups      : [rootString + ".driver.TMDXDOCKH52C1_M3"]
    });
}

/*
 *  ======== createBoardRoot =======
 */
function createBoardRoot(Examples, Board, productGroup)
{
    if (Boards.inProductGroup(Board, productGroup) == true) {
        Examples.templateGroupArr.$add({
           id          : rootString + ".driver." + Board.name,
           name        : Board.trexName,
           description : "TI-RTOS Projects for " + Board.name,
           groups      : [rootString + ".driver"]
        });
        return(true);
    }
    return(false);
}

/*
 *  ======== createBoardTree =======
 */
function createBoardTree(Examples, Board, tool)
{
    Examples.templateGroupArr.$add({
        id          : rootString + ".driver." + Board.name + "." + tool,
        name        : tool + " Target Examples",
        description : tool + " Projects for " + Board.name,
        groups      : [rootString + ".driver." + Board.name]
    });

    for(var i = 0; i < Board.peripherals.length; i++) {
        Examples.templateGroupArr.$add({
            id          : rootString + ".driver." + Board.name + "." + tool + "." + Board.peripherals[i],
            name        : Board.peripherals[i] + " Examples",
            description : Board.peripherals[i] + " Example Projects for the " + Board.name,
            groups      : [rootString + ".driver." + Board.name + "." + tool]
        });
    }
}

/*
 *  ======== addExamples ========
 */
function addExamples(Examples, opts, example, tool)
{
    /* For empty examples */
    if ((example.name == "empty") || (example.name == "empty_min")) {
        /* We don't have GNU in NPW */
        if (tool == "GNU") {
            return;
        }

        Examples.templateArr.$add({
            buildProfile: "release",
            filterArr: opts.filter,
            target: opts.targets["TI"],
            platform: opts.platforms["TI"],
            compilerBuildOptions: opts.compilerBuildOptions["TI"],
            linkerCommandFile: "",
            linkerBuildOptions: opts.linkerBuildOptions["TI"],
            title: example.title,
            name: example.name,
            description: example.description,
            fileList: [
                {path: "./" + opts.root + example.cFile},
                {path: "./" + opts.root + example.cfgFile},
                {path: "./" + opts.root + example.readme},
                {path: "./makefile.defs"}
            ],
            isHybrid: true,
            groups: [rootString + ".driver." + opts.name + "." + example.type],
            options: example.options,
            xdcToolsVersion: XDCtoolsVer,
            configuroOptions: "--compileOptions &quot;${COMPILER_FLAGS} &quot; ",
        });

        var ex = Examples.templateArr[Examples.templateArr.length - 1];

        /* Add Linker command file */
        if (opts.linkercmd) {
            ex.fileList.$add({path: "./" + opts.root + opts.linkercmd["TI"]});
        }

        /* Add any additional files to the list */
        for (k = 0; k < opts.fileList.length; k++) {
           ex.fileList.$add({path: "./" + opts.root + opts.fileList[k]});
        }

        /* Add any additional files to the list */
        for (k = 0; k < example.fileList.length; k++) {
            var targetDirectory = example.fileList[k].targetDirectory;
            if (targetDirectory == undefined || targetDirectory == null) {
                targetDirectory = "";
            }
            ex.fileList.$add({path: "./" + opts.root + example.fileList[k].path, targetDirectory: "./" + targetDirectory});
        }
    }
    else {
        var group;
        if ('group' in example) {
            group = rootString + ".driver." + opts.name + "." + tool + "." + example.group;
        }
        else {
            group = rootString + ".driver." + opts.name + "." + tool + "." + example.type;
            opts.filter[0].toolChain = tool;
        }

        opts.filter[0].toolChain = tool;
        Examples.templateArr.$add({
            buildProfile: "release",
            filterArr: opts.filter,
            target: opts.targets[tool],
            platform: opts.platforms[tool],
            compilerBuildOptions: opts.compilerBuildOptions[tool],
            linkerCommandFile: "",
            linkerBuildOptions: opts.linkerBuildOptions[tool],
            title: example.title,
            name: example.name,
            description: example.description,
            fileList: [
                {path: "./" + example.root + example.cFile},
                {path: "./" + example.root + example.cfgFile},
                {path: "./makefile.defs"}
            ],
            isHybrid: true,
            groups: [group],
            requiredProducts: example.requiredProducts,
            options: example.options,
            xdcToolsVersion: XDCtoolsVer,
            configuroOptions: "--compileOptions &quot;${COMPILER_FLAGS} &quot; ",
        });

        var ex = Examples.templateArr[Examples.templateArr.length - 1];

        /* Add Linker command file */
        if (opts.linkercmd) {
            ex.fileList.$add({path: "./" + opts.root + opts.linkercmd[tool]});
        }

        /* Add any additional files to the list */
        for (k = 0; k < opts.fileList.length; k++) {
            ex.fileList.$add({path: "./" + opts.root + opts.fileList[k]});
        }

        /* Add any additional files to the list */
        for (k = 0; k < example.fileList.length; k++) {
            var targetDirectory = example.fileList[k].targetDirectory;
            if (targetDirectory == undefined || targetDirectory == null) {
                targetDirectory = "";
            }
            ex.fileList.$add({path: "./" + example.fileList[k].path, targetDirectory: "./" + targetDirectory});
        }

        /* Add any additional compiler options specific to example */
        if (example.compilerBuildOptions) {
            ex.compilerBuildOptions += example.compilerBuildOptions[tool];
        }

        /* Add any additional linker options specific to example */
        if (example.linkerOptions) {
            ex.linkerBuildOptions += example.linkerOptions;
        }
    }
}
