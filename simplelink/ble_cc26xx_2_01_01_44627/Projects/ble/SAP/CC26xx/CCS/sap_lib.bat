@echo off
::----------------------------------------------------------
::
::  This batch file will build CC26xx BLE SAP Library
::
::----------------------------------------------------------
echo Building SAP Lib


::-------------------------------------
:: Locate the compiler, WinXP or Win7
::-------------------------------------
SET bCMD="C:\ti\ccsv6\eclipse\eclipsec.exe"
if exist %bCMD% goto BUILD
echo CCS build command not in expected location...
pause
goto EXIT

:BUILD
::----------------------------
:: Create _Unique_ Temporary Workspace
::----------------------------
set bWork="C:\ti\ccsv6\remove_workspace_4_8_15_16_23_42"
if not exist %bWork% mkdir %bWork%

::-------------------------
::  Commom local variables
::-------------------------
SET bPRJ=.

::-------------------------------
::  Build Library
::-------------------------------
%bCMD% -noSplash -data %bWork% -application com.ti.ccstudio.apps.projectImport  -ccs.referencedProjectSearch Directory -ccs.autoImportReferencedProjects -ccs.autoBuild -ccs.overwrite -ccs.location %bPRJ%

::----------------------------
:: Delete Temporary Workspace
::----------------------------
if exist %bWork% rmdir %bWork% /s /q

:EXIT