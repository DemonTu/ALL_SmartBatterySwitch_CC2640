/* 
 *  Copyright (c) 2013 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */
/*!
 *  ======== Main ========
 *  Command-line configuration tool for IAR Embedded Workbench
 *
 *  This command-line tool computes the appropriate configuro options from the
 *  set of options passed from the IAR Workbench (for ex. target/platform
 *  from compile options). With these options, the xdc.tools.configuro tool
 *  is executed to compute the set of libraries, command-line flags and
 *  the other artifacts needed to build the application in IAR Workbench.
 *
 *  @a(INPUTS)
 *  @p(dlist)
 *    - `-o outdir (Optional)`
 *      Optionally an output directory can be provided.
 *    - `-c codegen_dir`
 *      Root directory of the code generation tools.
 *    - `--device device_name_string`
 *      The name of the device.
 *    - `--compileOptions compile_options_string`
 *      The command line options passed to the compiler.
 *    - `--linkOptions linker_options_string`
 *      The command line options passed to the linker.
 *    - `--profile profile`
 *      The build profile.
 *    - `--projFile project_file (Optional)`
 *      IAR Embedded Workbench project file. This is optional when 
 *      infile.cfg is passed on command line.
 *    - `infile.cfg (Optional)`
 *      A user-supplied configuration script that names a set of RTSC
 *      modules, and optionally changes their configuration settings. 
 *      If provided, it will override the --projFile option.
 *  @p
 *
 *  @a(OUTPUTS)
 *  @p(dlist)
 *    - `outdir/`
 *      A directory containing all generated build artifacts.
 *    - `outdir/compiler.defs`
 *      A file containing C compiler command-line flags. These flags must
 *      included on the compiler command line for any C source file that
 *      directly accesses the RTSC content. The flags define the header file
 *      and include paths to ensure object code compatibility between all
 *      all included content.
 *    - `outdir/linker.cmd`
 *      A file containing linker command-line flags. These flags must be
 *      included on the linker command line for the final link of the
 *      application. The flags list needed libraries and object files,
 *      and on some platforms define the embedded system memory map.
 *  @p
 *
 *  For example:
 *  @p(code)
 *      xs iar.tools.configuro -c "<codegen_dir>" --cc "<compiler>"
 *         --device "<device_name>" -compileOptions "<compiler_options>"
 *         --linkOptions "<link_options>" --profile "<profile>" infile.cfg
 *  @p
 */
metaonly module Main inherits xdc.tools.ICmd {

    /*!
     * usage help message
     */
    override config String usage[] = [
        '[-o outdir (optional)]',
        '[-c codegen_dir]',
        '[--cc compiler_name_string]',
        '[--device device_name]',
        '[--compileOptions compile_options_string]',
        '[--linkOptions linker_options_string]',
        '[--profile profile]',
        '[--projFile project_file (optional)]',
        'infile.cfg (optional)'
    ];

instance:

    /*!
     *  ======== output ========
     *  Pathname of the output directory
     *
     *  A directory containing the generated build artifacts, in particular
     *  the `compiler.defs` and `linker.cmd` files.
     *
     *  The last component of the output directory path must be a valid
     *  ANSI C identifier; i.e., it must consist entirely of alphanumeric or
     *  '_' characters and must not start with a number.  So, the names
     *  '0app' and 'app-test' are not valid but '0app/config' and
     *  'app-test/config' are valid.
     *
     *  This is optional parameter. By default, the output directory has the
     *  name `configPkg` and will be  within the `{#cfgDir}` directory.
     */
    @CommandOption("o")
    config String output = "";

    /*!
     *  ======== rootDir ========
     *  Root directory of the code generation tools
     *
     *  The path to the installation directory of the compiler and linker
     *  for the selected target. The definition of "installation directory"
     *  can vary from compiler to compiler, but is most commonly the
     *  directory that contains a "bin" subdirectory.
     */
    @CommandOption("c")
    config String rootDir = null;

    /*!
     *  ======== compiler ========
     *  @_nodoc
     *  The name of the compiler
     *
     *  The compiler name is required to find the target and platform
     *  xdc.tools.configuro options.
     */
    @CommandOption("cc")
    config String compiler = "";

    /*!
     *  ======== device ========
     *  The name of the device
     *
     *  The device name has to be passed to the xdc.tools.configuro tool.
     */
    @CommandOption("device")
    config String device = null;

    /*!
     *  ======== compileOptions ========
     *  Compile options used for building C files
     *
     *  The compiler options are required to find the target and platform
     *  options for xdc.tools.configuro. 
     */
    @CommandOption("compileOptions")
    config String compileOptions = null;

    /*!
     *  ======== linkOptions ========
     *  Linker options used for linking libraries
     *
     *  The linker options are required to pull in the correct libraries 
     *  during link. 
     */
    @CommandOption("linkOptions")
    config String linkOptions = null;

    /*!
     *  ======== profile ========
     *  Build profile
     */
    @CommandOption("profile")
    config String profile = "";

    /*!
     *  ======== projFile ========
     *  IAR Embedded project file.
     *
     *  This file is searched for the RTSC configuration file (.cfg).
     */
    @CommandOption("projFile")
    config String projFile = "";
}
/*
 *  @(#) iar.tools.configuro; 1, 0, 0,38; 3-22-2015 09:30:54; /db/ztree/library/trees/xdctools/xdctools-f61x/src/
 */

