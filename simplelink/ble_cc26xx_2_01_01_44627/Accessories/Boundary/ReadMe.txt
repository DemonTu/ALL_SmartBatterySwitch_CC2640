==================================================
Texas Instruments Boundary
==================================================
--------------------------------------
Boundary - (For Version 1.0.5 or higher)
--------------------------------------
Boundary is a command line tool checks the stack starting address and adjusts this boundary.

---------------------
Software Installation
---------------------
This program consists of two files Boundary.exe and BoundaryConfig.xml. 

If your system has “Microsoft .Net Framework 4” you can optionally skip the install progress.                                          

If you need to install or do not know if you have “Microsoft .Net Framework 4” you can…
Run setup.exe to install the software.

The software installer is composed of two files setup.exe and "Boundary_Setup.msi".

This readme file will appear under Texas Instruments in Start | Programs.

In the case Boundary was included in another TI software release it will be installed by that package.

---------------------
Hardware Requirements
---------------------
None

==================================================
Known Issues
==================================================
1. User’s upgrading from 1.0.3 and lower, have two mandatory command line changes.  There is a new command line parameter “-prj”, which is the project building tool, i.e. -prj “iar”.  The second renames the “-icf” to “-lcf” from a iar specific name to a more generic linker configuration/control file.


==================================================
Boundary Operation
==================================================
1. Provide all the required command line parameters.
2. Boundary provides compiler and linker configuration files that contain the stack boundary address.  These files should be used by the stack and the application builds.
3. If Boundary detects that the boundary needs to moved or the configuration files are missing, it will declare an error.  In this case the operation will be successful but you will need to rebuild the entire project for the new boundary address to take effect.
4. In the case the boundary does not have to be moved and files are not missing, no error will be displayed.
5. Boundary always displays processed map, lcf and configuration file information in the Messages area.


==================================================
IAR Boundary Operation Notes
==================================================
1. Boundary is run from a command prompt window or as a post build process in IAR.
2. In IAR set Tools -> Options ->Show Build Messages to All.
3. In IAR select the project, then right click for Options -> Build Actions, provide a command line call to Boundary in the “Post-build command line” box.


==================================================
CCS Boundary Operation Notes
==================================================
1. Boundary is run from a command prompt window or as a post build process in CCS.
2. In IAR select the project, then right click for Properties -> Build -> Steps, provide a command line call to Boundary in the “Post-build steps” box.

==================================================
Command Line Parameters
==================================================

Usage -> Boundary <-prj Project Name> <-chip Chip Name> <-lcf Linker Configuration File> <-map Map File> <-xml Xml Config File> <-cc Compiler Defines File> <-lnk Linker Defines File> [<-forceCcLnk>] [<-log Log File>]

Example: Boundary -prj "iar" -chip "CC2650" -lcf "c:\lcf_location\Linker.lcf" -map "c:\map_location\Linker.map" -xml "c:\xml_location\BoundaryConfig.xml" -cc "c:\cc_location\CompilerDefines.bcfg" -lnk "c:\lnk_location\LinkerDefines.xcl" -log "c:\log_location\Boundary.log"

         Checks and only if necessary will move the boundary.
         Generates configuration files for the compiler and linker for use 
           by the application using the stack.
         All file names should use a fully qualified path and name.

Options: <-chip Chip Name> - Required
           Desired chip name as used in the lcf file
           Example: -chip "CC2650"

         <-prj Project Build Tool> - Required
           The project build tool
           Supported Project Types:
           	iar (IAR Project)
	     	ccs (CCS Project)
           Example: -prj "iar"

         <-lcf Linker Configuration File Name> - Required
           Fully qualified path and file name for the lcf file
           Example: -lcf "c:\lcf_location\Linker.lcf"

         <-map Map File Name> - Required
           Fully qualified path and file name for the map file
           Example: -map "c:\map_location\Linker.map"

         <-xml Tool Config Xml File> - Required
           Fully qualified path and file name for the xml file
           Example: -xml "c:\xml_location\BoundaryConfig.xml"

         <-cc Compiler Defines File> - Required
           Fully qualified path and file name for the cc file
           File is always overwritten, do not modify.
           File is auto generated if the boundary moves or 
             if the file does not exist.
           Example: -cc "c:\cc_location\CompilerDefines.bcfg"

         <-lnk Linker Defines File> - Required
           Fully qualified path and file name for the lnk file
           File is always overwritten, do not modify.
           File is auto generated if the boundary moves or 
             if the file does not exist.
           Example: -lnk "c:\lnk_location\LinkerDefines.xcl"

         [<-log Log File Name>] - Optional
           Fully qualified path and file name for the log file
           File does not have to exist on initial use.
           File is circular in nature and has a limited max size.
           Example: -log "c:\log_location\Boundary.log"

         [<-forceCcLnk>] - Optional
           Force the write of the cc and lnk files.
           Example: -forceCcLnk

         [<-errPause>] - Optional
           The program will pause just before exit.
           Use this option if you are running Boundary from the command line.
           Do not use this option in IAR it will cause an exception.
           Example: -errPause

         [<-help>] - Optional
           Displays This Help Screen
           Example: -help

Returns: 0 = success - no boundary change
         1 = boundary changed successfully
         2 = failure
         The following only applies if the -errPause option is used
         Command line window is not held open on success
           and a project rebuild is not required.
         Command line window is held open on success
           and a project rebuild is required, press any key press to exit.
         Command line window is held open on fail.



IAR Example:
C:\Program Files (x86)\Texas Instruments\Boundary\Boundary.exe -prj "iar"
 -chip "CC2650" -lcf "$PROJ_DIR$\Config\cc26xx_zstack.icf" -map "$PROJ_DIR$\$CONFIG_NAME$Src\List\ZStackCoreThread$CONFIG_NAME$Src.map" -xml "C:\Program Files (x86)\Texas Instruments\Boundary\BoundaryConfig.xml" -cc "$PROJ_DIR$\Config\core$CONFIG_NAME$.bcfg" -lnk "$PROJ_DIR$\Config\core$CONFIG_NAME$.xcl"

CCS Example:
“C:\Program Files (x86)\Texas Instruments\Boundary\Boundary.exe” -prj "ccs" -chip "CC2650" -lcf "${PROJECT_LOC}/../../../../common/cc26xx/CCS/cc2650f128_tirtos_ccs_stack.cmd" -map "${PROJECT_LOC}/${ConfigName}/${ProjName}.map" -xml "C:\Program Files (x86)\Texas Instruments\Boundary\BoundaryConfig.xml" -cc "${PROJECT_LOC}/../Config/ccsCompilerDefines.bcfg" -lnk "${PROJECT_LOC}/../Config/ccsLinkerDefines.cmd" 




