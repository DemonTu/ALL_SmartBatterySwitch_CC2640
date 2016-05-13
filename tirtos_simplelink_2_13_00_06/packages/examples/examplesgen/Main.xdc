/*
 *  Copyright 2015 by Texas Instruments Incorporated.
 *
 */

/*!
 *  ======== Main ========
 *  This tool generates TIRTOS examples from the installed product.
 *  It creates a directory structure for the examples based on toolchain with
 *  each toolchain containing folders for all boards it supports which in
 *  turn contain folders for every example supported by that board.
 *  Also makefiles to build these examples with that toolchain are provided so
 *  that a user can run a top level make, to build all examples or run make in
 *  a given example to build it specifically.
 *
 *       options:
 *       [--tirtos           tirtos_product_directory]
 *       [--toolchain        toolchain(e.g IAR)]
 *       [--devicefamily     device family (e.g. ARM, MSP430, MSP432, C2000)]
 *       [--output           output directory]
 *       [--xdctools         XDC tools install path]
 *       [--bios             BIOS install path]
 *       [--uia              UIA install path]
 *       [--ndk              NDK install path]
 *       [--tivaware         TivaWare install path]
 *       [--mware            MWare install path]
 *       [--ccware           CCWare install path]
 *       [--cc13xxware       CC13XXWare install path]
 *       [--cc26xxware       CC26XXWare install path]
 *       [--mspware          MSPWare install path]
 *       [--codegendir       Codegen install directory]
 *       [--idefiles         Generates IDE files]
 *       [--examplelistFile  Override default ExamplesList.xs]
 *       [--boardsFile       Override default Boards.xs]
 *       [--testlinkSupport  Add testlink markup text to generated examples]
 *
 */

metaonly module Main inherits xdc.tools.ICmd
{
    override config String usage[] = [
        '[--tirtos           tirtos_product_directory]',
        '[--toolchain        toolchain(e.g IAR)]',
        '[--devicefamily     device family (e.g. ARM, MSP430, MSP432, C2000)]',
        '[--output           output directory]',
        '[--xdctools         XDC tools install path]',
        '[--bios             BIOS install path]',
        '[--uia              UIA install path]',
        '[--ipc              IPC install path]',
        '[--ndk              NDK install path]',
        '[--tivaware         TivaWare install path]',
        '[--mware            MWare install path]',
        '[--ccware           CCWare install path]',
        '[--cc13xxware       CC13XXWare install path]',
        '[--cc26xxware       CC26XXWare install path]',
        '[--mspware          MSPWare install path]',
        '[--codegendir       Codegen install directory]',
        '[--idefiles         Generates IDE files]',
        '[--examplelistFile  Override default ExamplesList.xs]',
        '[--boardsFile       Override default Boards.xs]',
        '[--testlinkSupport  Add testlink markup text to generated examples]'
    ];

instance:

    /*!
     *  ======== tirtosproductDir ========
     *  TI-RTOS Product root directory
     *
     *  This option names the tirtos root directory that is used by the
     *  tool to generate the examples and makefiles.
     *
     *  This option should be specified
     *
     */
    @CommandOption("tirtos")
    config String tirtosProductDir;

    /*!
     *  ======== toolChain ========
     *  Option specifying the tool chain to be used build examples
     *
     *  This is a required option
     */
    @CommandOption("toolchain")
    config String toolChain;

    /*!
     *  ======== deviceFamily ========
     *  Optional device family (ARM, MSP430, MSP432, C2000, etc.)
     *  This field is used to filter based on device family and prevent
     *  incompatible board-compiler pairs.
     */
    @CommandOption("devicefamily")
    config String deviceFamily = null;

    /*!
     *  ======== outputDir ========
     *  Output directory in which the examples will be generated.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("output")
    config String outputDir;

    /*!
     *  ======== xdctools ========
     * Path to installed xdctools.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("xdctools")
    config String xdctools;

    /*!
     *  ======== bios ========
     * Path to installed bios.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("bios")
    config String bios;

    /*!
     *  ======== uia ========
     * Path to installed UIA.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("uia")
    config String uia = "";

    /*!
     *  ======== ipc ========
     * Path to installed UIA.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("ipc")
    config String ipc = "";

    /*!
     *  ======== ndk ========
     *  Path to installed NDK.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("ndk")
    config String ndk = "";

    /*!
     *  ======== tivaware ========
     *  Path to installed TivaWare.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("tivaware")
    config String tivaware = "";

    /*!
     *  ======== mware ========
     *  Path to installed MWare.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("mware")
    config String mware = "";

    /*!
     *  ======== ccware ========
     *  Path to installed CCWare.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("ccware")
    config String ccware = "";

    /*!
     *  ======== cc13xxware ========
     *  Path to installed CC13XXWare.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("cc13xxware")
    config String cc13xxware = "";

    /*!
     *  ======== cc26xxware ========
     *  Path to installed CC26XXWare.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("cc26xxware")
    config String cc26xxware = "";

    /*!
     *  ======== msp430 ========
     *  Path to installed MSP40Ware.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("mspware")
    config String mspware = "";

    /*!
     *  ======== armToolsDir ========
     *  Path to codegen installation directory.
     *
     *  This option is required and paths should be specified with UNIX style
     *  i.e no forward slashes.
     */
    @CommandOption("toolChaindir")
    config String toolChaindir = null;

    /*!
     *  ======== optCopts ========
     *  Optional Copts to be added to examples
     *  This field is used to additional compiler options to examples
     */
    @CommandOption("optCopts")
    config String optCopts = "";

    /*!
     *  ======== optLopts ========
     *  Optional Lopts to be added to examples
     *  This field is used to additional linker options to examples
     */
    @CommandOption("optLopts")
    config String optLopts = "";

    /*!
     *  ======== extraXDCPATH ========
     *  Optional xdcpath to be added to examples
     *  This field is used to additional linker options to examples
     */
    @CommandOption("extraXDCPATH")
    config String extraXDCPATH = "";

    /*!
     *  ======== productGroup ========
     *  Optional arguments to build for boards in a specific Business Unit
     *
     *  The argment name must match a javascript array name that contains a list
     *  of desired boards names within "Boards.xs"
     *
     */
    @CommandOption("productGroup")
    config String productGroup = null;

    /*!
     *  ======== idefiles ========
     *  Generate IDE files.
     *
     *  This option is optional and when this option is specified, IDE
     *  integration files are generated. This is supported IAR toolchain only.
     */
    @CommandOption("idefiles")
    config Bool idefiles = true;

    /*!
     *  ======== examplelistFile ========
     *  Specify an alternate ExamplesList.xs file where examplesgen will find
     *  the description of which examples should be generated.
     *  It assumes that the source files are located with this file's parent
     *  directory.
     *
     *  This is optional. If not specified, we will be reading in a default
     *  ExampleList.xs in the parent ./examples directory.
     *  The associated example source files need to reside in this directory.
     */
    @CommandOption("examplelistFile")
    config String examplelistFile = "";

    /*!
     *  ======== boardsFile ========
     *  Specify an alternate Boards.xs file where examplesgen will find
     *  the description of which boards should be generated.
     *  It assumes that the board directories are located with this file's
     *  parent directory.
     *
     *  This is optional. If not specified, we will be reading in a default
     *  Boards.xs in the parent ./examples directory.
     *  The associated board directories need to reside in this directory.
     */
    @CommandOption("boardsFile")
    config String boardsFile = "";

    /*!
     *  ======== testlinkSupport ========
     *  Boolean to specifify whether or not to add testlink prefix tags to the
     *  generated examples
     */
    @CommandOption("testlinkSupport")
    config Bool testlinkSupport = false;

    /*!
     *  ======== run ========
     */
    override Any run(xdc.tools.Cmdr.Instance cmdr, String args[]);
}
