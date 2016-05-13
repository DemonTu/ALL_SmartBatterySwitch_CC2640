@echo off
::----------------------------------------------------------
::
::  This batch file will build BLE SAP Library
::
::----------------------------------------------------------
echo Building SAP Lib

:: Version of IAR compiler
SET bCOM=EWARM-7.40.2

::-------------------------------------
:: Locate the compiler, WinXP or Win7
::-------------------------------------
SET bCMD="C:\Program Files\IAR Systems\%bCOM%\common\bin\IarBuild.exe"
if exist %bCMD% goto BUILD
SET bCMD="C:\Program Files (x86)\IAR Systems\%bCOM%\common\bin\IarBuild.exe"
if exist %bCMD% goto BUILD
echo Can't find the IAR %bCOM% compiler...
pause
goto EXIT

:----------------------------
: Remember current directory
:----------------------------
set IWD=%CD%

:BUILD
::-------------------------
::  Commom local variables
::-------------------------
SET bOPT=-build
SET bLOG=-log info

::-------------------------------
::  Build SAPLib
::-------------------------------
SET bCFG=SAPLib
SET bPRJ=SAPLib
SET bVAR=-varfile .\%bPRJ%.custom_argvars

%bCMD% %bPRJ%.ewp %bOPT% "%bCFG%" %bLOG% %bVAR%

::-------------------------------
::  Build SAPLib_noPM
::-------------------------------
SET bCFG=SAPLib_noPM
SET bPRJ=SAPLib
SET bVAR=-varfile .\%bPRJ%.custom_argvars

%bCMD% %bPRJ%.ewp %bOPT% "%bCFG%" %bLOG% %bVAR%

:EXIT
