/*
 *  Copyright 2015 by Texas Instruments Incorporated.
 *
 */

var tool;
var deviceFamily;
var root;
var tirtosRoot;
var tirtosExamplesPath;
var dest;
var xdctools;
var bios;
var uia;
var ipc;
var ndk;
var tivaware;
var mware;
var ccware;
var cc13xxware;
var cc26xxware;
var mspware;
var toolChaindir;
var productGroup;
var ideFileGen;
var ipcf = [];
var extraExamplesGenCopts;
var extraExamplesGenLopts;
var testlinkSupport;
var extraXDCPATH;

/* capsules used by Boards.xs and Examplelist.xs */
exampleListDir = "";
boardsFileDir = "";
Board = "";
Example = "";

function run(cmdr, args) {
    /* Check for required command line parameters */
    if (this.tirtosProductDir == undefined ||
        this.toolChain == undefined ||
        this.outputDir == undefined ||
        this.bios == undefined ||
        this.xdctools ==  undefined) {
        cmdr.usage();
        return (null);
    }

    /* Retrieve args */
    tool = this.toolChain;
    deviceFamily = this.deviceFamily;
    dest = this.outputDir;
    xdctools = this.xdctools;

    bios = this.bios + "/packages";
    uia = (this.uia == "" ) ? "" : this.uia + "/packages";
    ipc = (this.ipc == "" ) ? "" : this.ipc + "/packages";
    ndk = (this.ndk == "" ) ? "" : this.ndk + "/packages";
    tirtosRoot = this.tirtosProductDir + "/packages";
    tivaware = this.tivaware;
    mware = this.mware;
    ccware = this.ccware;
    cc13xxware = this.cc13xxware;
    cc26xxware = this.cc26xxware;
    mspware = this.mspware;
    toolChaindir = this.toolChaindir;
    extraExamplesGenCopts = this.optCopts;
    extraExamplesGenLopts = this.optLopts;
    testlinkSupport = this.testlinkSupport;
    extraXDCPATH = this.extraXDCPATH;

    var curDir = new java.io.File(".").getCanonicalPath();
    tirtosExamplesPath = curDir + "/packages/examples/";
    if (!java.io.File(tirtosExamplesPath).exists()) {
        tirtosExamplesPath = curDir + "/src/examples/";
    }

    var exampleListFile = new java.io.File(this.examplelistFile);
    if (exampleListFile.exists()) {
        exampleFile = exampleListFile.getCanonicalPath();
        exampleListDir = exampleListFile.getParent();
    }
    else {
        exampleFile = tirtosExamplesPath + "ExampleList.xs";
        exampleListDir = tirtosExamplesPath;
    }

    var boardFile = new java.io.File(this.boardsFile);
    if (boardFile.exists()) {
        boardsFile = boardFile.getCanonicalPath();
        boardsFileDir = boardFile.getParent() + "/";
    }
    else {
        boardsFile = tirtosExamplesPath + "Boards.xs";
        boardsFileDir = tirtosExamplesPath;
    }

    /* Extract TI-RTOS version to determine if we are in an installed product */
    var tirtosVersion = this.tirtosProductDir.match(/[^\/]?tirtos_[a-z0-9]*_[_\deng]*$/);
    if ((this.toolChain == "IAR") && (tirtosVersion != null)) {
        ideFileGen = this.idefiles;
    }
    else {
        ideFileGen = false;
    }

    /* create generated file directory */
    root = new java.io.File(dest);
    root.mkdir();

    /* create directory for toolchain */
    var toolRoot = new java.io.File(root.getPath() + "/" + tool);
    toolRoot.mkdir();

    /* Load Boards.xs script for board data */
    Boards = xdc.loadCapsule(boardsFile);

    /* Load ExampleList script for example data; default defined in Main.xdc */
    Example = xdc.loadCapsule(exampleFile);

    var boards = Boards.examplesgenBoards;
    var example = Example.allExamples;

    var boardRoot;
    var exampleRoot;
    for (var i = 0; i < boards.length; i++) {
        /* check if board is available for that toolchain */
        if (Boards.supportsTool(boards[i], tool)) {
            /* See if we're building for all boards.. */
            if ((this.productGroup != null) &&
                Boards.inProductGroup(boards[i], this.productGroup) == false) {
                continue;
            }

            /* Ensure tool chain types match */
            if (deviceFamily != null &&
                (deviceFamily != boards[i].filter[0].deviceFamily)) {
                continue;
            }

            /* Create directory for board */
            boardRoot =  new java.io.File(toolRoot.getPath() + "/" + boards[i].name);
            boardRoot.mkdir();
            copyMakefiles(boardRoot.getPath(), boards[i], "makedefs", "");
            copyMakefiles(boardRoot.getPath(), boards[i], "topmake", "");

            for (var k = 0; k < example.length; k++) {
                /* if example is available on board and is supported by a tool */
                if (Example.supportsBoard(example[k], boards[i]) &&
                    Example.supportsTool(example[k], tool)) {

                    /* create directory for example */
                    if ((testlinkSupport == true) && (example[k].hwAsset != undefined)){
                        exampleRoot =  new java.io.File(boardRoot.getPath() + "/" +
                                   example[k].hwAsset + "_" + example[k].name);
                    }
                    else {
                        exampleRoot =  new java.io.File(boardRoot.getPath() + "/" +
                                   example[k].name);
                    }
                    exampleRoot.mkdir();
                    /* copy all board files */
                    copyBoardFiles(boards[i], exampleRoot.getPath());
                    /* copy all example files */
                    copyExampleFiles(example[k], boards[i],
                            exampleRoot.getPath());
                    /* copy necessary makefiles */
                    if ('compilerBuildOptions' in example[k]) {
                        if (example[k].compilerBuildOptions[tool + "_makefile"] == undefined) {
                            throw("No "+ tool + "_makefile defined in " +
                                    example[k].name + ".compilerBuildOptions")
                        }
                        copyMakefiles(exampleRoot.getPath(), boards[i], "lowmake",
                                    example[k].compilerBuildOptions[tool + "_makefile"]);
                    }
                    else {
                        copyMakefiles(exampleRoot.getPath(), boards[i], "lowmake", "");
                    }
                    /* generate necessary ipcf files */
                    if (ideFileGen) {
                        genIARFiles(exampleRoot.getPath(), "ipcf", example[k],
                            boards[i]);
                    }

                }
            }
        }
    }

    if (ideFileGen) {
        genIARFiles(toolRoot, "html", tirtosVersion);

        var iconDir = new java.io.File(toolRoot + "/icons");
        iconDir.mkdir();
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/chip_closed.png",
            iconDir.getPath() + "/chip_closed.png");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/chip_open.png",
            iconDir.getPath() + "/chip_open.png");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/cubeAqua_closed.png",
            iconDir.getPath() + "/cubeAqua_closed.png");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/cubeAqua_open.png",
            iconDir.getPath() + "/cubeAqua_open.png");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/examplefile_s.png",
            iconDir.getPath() + "/examplefile_s.png");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/tilogo.gif",
            iconDir.getPath() + "/tilogo.gif");
        filecopy(tirtosExamplesPath + "examplesgen/iar/icons/titagline.gif",
            iconDir.getPath() + "/titagline.gif");
    }
}


/*
 *  filecopy - used to copy from one file to new destination
 */
function filecopy(source, target)
{
    var length;
    var inputStream = new java.io.FileInputStream(source);
    var outputStream = new java.io.FileOutputStream(target);
    var buffer = java.lang.reflect.Array.newInstance(java.lang.Byte.TYPE, 1024);

    while ((length = inputStream.read(buffer)) > 0) {
        outputStream.write(buffer, 0, length);
    }

    inputStream.close();
    outputStream.close();
}

/*
 *  copyBoardFiles - copy board related files
 */
function copyBoardFiles(Board, dest)
{
    var srcPath = boardsFileDir + Board.root;
    var srcHandle;
    var destHandle;

    for(var i = 0; i < Board.fileList.length; i++){
        srcHandle = new java.io.File(srcPath + Board.fileList[i]);
        destHandle = new java.io.File(dest + "/" + Board.fileList[i]);

        filecopy(srcHandle, destHandle);
    }
    srcHandle = new java.io.File(srcPath + Board.linkercmd[tool]);
    destHandle = new java.io.File(dest + "/" + Board.linkercmd[tool]);

    filecopy(srcHandle, destHandle);
}

/*
 *  copyExampleFiles - copy example files
 */
function copyExampleFiles(Example, Board, dest)
{
    var srcPath;
    var destPath;
    var srcHandle;
    var destHandle;
    var fileName;
    var basePath;

    if ((Example.name == "empty") || (Example.name == "empty_min")) {
        basePath = boardsFileDir + Board.root + "/";
    }
    else {
        basePath = exampleListDir + "/";
    }

    if (Example.root == undefined) {
        Example.root = "";
    }

    /* copy .c file */
    srcPath = basePath + Example.root + Example.cFile;

    if (Example.type == "Demo") {
        var nameArray = Example.cFile.split('/');
        fileName = nameArray[nameArray.length - 1];
        destPath = dest + "/" + fileName;
    }
    else {
        destPath = dest + "/" + Example.cFile;
    }

    srcHandle = new java.io.File(srcPath);
    destHandle = new java.io.File(destPath);
    filecopy(srcHandle, destHandle);

    /* copy .cfg file */
    srcPath = basePath + Example.root + Example.cfgFile;

    if(Example.type == "Demo") {
        var nameArray = Example.cfgFile.split('/');
        fileName = nameArray[nameArray.length - 1];
        destPath = dest + "/" + fileName;
    }
    else {
        /*
         * If testlink support has been enabled it causes the generated makefile
         * to generate a .out file that that the testlink scripts can parse.
         */
        if ((testlinkSupport == true) && (Example.hwAsset != undefined)) {
            destPath = dest + "/" + Example.hwAsset + "_" + Example.cfgFile;
        }
        else {
            destPath = dest + "/" + Example.cfgFile;
        }
    }

    srcHandle = new java.io.File(srcPath);
    destHandle = new java.io.File(destPath);
    filecopy(srcHandle, destHandle);

    for (var i = 0; i < Example.fileList.length; i++) {
        var targetDirectory = Example.fileList[i].targetDirectory;
        if (targetDirectory === undefined) {
            targetDirectory = ".";
        }

        if (targetDirectory != ".") {
            var dirs = new java.io.File(dest + "/" + targetDirectory);
            dirs.mkdirs();
        }

        srcPath = basePath + Example.fileList[i].path;

        var nameArray = Example.fileList[i].path.split('/');
        var fileName = nameArray[nameArray.length - 1];
        if (targetDirectory != ".") {
            destPath = dest + "/" + targetDirectory + "/" + fileName;
        }
        else {
            destPath = dest + "/" + fileName;
        }

        srcHandle = new java.io.File(srcPath);
        destHandle = new java.io.File(destPath);
        filecopy(srcHandle, destHandle);
    }
}

/*
 *  copyMakefiles - copy makefiles
 */
function copyMakefiles(dest, Board, type, opt)
{
    var srcPath;
    var destPath;
    var srcHandle;
    var destHandle;

    switch (type) {
        case "topmake":
        srcPath = tirtosExamplesPath + "examplesgen/makefiles" + "/makefile";
        destPath = dest + "/" + "makefile";
        srcHandle = new java.io.File(srcPath);
        destHandle = new java.io.File(destPath);
        filecopy(srcHandle, destHandle);
        break;

        case "lowmake":
        srcPath = tirtosExamplesPath + "examplesgen/makefiles/makefile.xdt";
        destPath = dest + "/" + "makefile";
        var template = xdc.loadTemplate(srcPath);
        template.genFile(destPath, this, [tool, opt]);
        break;

        case "makedefs":
        srcPath = tirtosExamplesPath + "examplesgen/makefiles/makedefs.xdt";
        destPath = dest + "/" + "makedefs";
        var template = xdc.loadTemplate(srcPath);
        template.genFile(destPath, this, [Board, tool, tirtosRoot, xdctools,
            bios, uia, ipc, ndk, tivaware, mware, ccware, cc13xxware,
            cc26xxware, mspware, toolChaindir, extraExamplesGenCopts,
            extraExamplesGenLopts, extraXDCPATH]);
        break;
     }
}

/*
 *  genIARFiles - generate IAR files
 */
function genIARFiles(dest, type, example, board)
{
    var srcPath;
    var destPath;

    switch (type) {
        case "ipcf":
        srcPath = tirtosExamplesPath + "examplesgen/iar/ipcf.xdt";
        destPath = dest + "/" + example.name + ".ipcf";
        ipcf[ipcf.length++] = {
            path:     destPath,
            example:  example,
            board:    board,
        };
        var template = xdc.loadTemplate(srcPath);
        template.genFile(destPath, this, [example, board, dest]);
        break;

        case "html":
        srcPath = tirtosExamplesPath + "examplesgen/iar/html.xdt";
        destPath = dest + "/" + "Examples.html";
        var template = xdc.loadTemplate(srcPath);
        template.genFile(destPath, this, [ipcf]);

        srcPath = tirtosExamplesPath + "examplesgen/iar/args.xdt";
        destPath = dest + "/" + example + ".custom_argvars";
        var template = xdc.loadTemplate(srcPath);
        template.genFile(destPath, this, [xdctools, dest, example]);

        break;
    }
}
