@echo off

REM Path to the workspace
set WORKSPACE=C:\ti\ccsv6.0.0.00156\workspace_tirtos

REM Name of the ccs project in the workspace
set PROJROOT=uartlogging
set PROJNAME=%PROJROOT%_StellarisLM4F232H5QD

REM Location of TI-RTOS, XDCTools and versions of products
set XDC_INSTALL_DIR=C:\ti\xdctools_3_25_04_88
set TIRTOS_INSTALL_DIR=C:/ti/ccsv6.0.0.00156
set TIRTOS_VER=tirtos_tivac_2_00_00_12_eng
set UIA_VER=uia_2_00_00_17_eng
set BIOS_VER=bios_6_40_00_04_eng


REM CPU variant
REM set ext=em3
set ext=em4f

REM COM Port on which the dev board is enumerated with
set COM=COM4

REM Paths to products
set UIA_DIR=%TIRTOS_INSTALL_DIR%\%TIRTOS_VER%\products\%UIA_VER%
set BIOS_DIR=%TIRTOS_INSTALL_DIR%\%TIRTOS_VER%\products\%BIOS_VER%

REM Add the java bin directory into the PATH env variable
setlocal
set PATH=C:\Program Files\Java\jre7\bin;%PATH%

REM Examines data directly from the UART (COM port)
%XDC_INSTALL_DIR%\xs.exe ^
--cp "c:\Program Files (x86)\Java\jdk1.6.0_26\jre\lib\ext\RXTXcomm.jar" ^
--xdcpath %UIA_DIR%\packages;%BIOS_DIR%\packages ^
-f %UIA_DIR%\packages\ti\uia\scripts\UIAHostFromUART.xs ^
-p %WORKSPACE%\%PROJNAME%\Debug\%PROJNAME%.out ^
-r %WORKSPACE%\%PROJNAME%\Debug\configPkg\package\cfg\%PROJROOT%_p%ext%.rta.xml ^
-c %COM% ^
-s %WORKSPACE%\%PROJNAME%

REM Examine data file from a binary Tera Term log file
REM ..\xdctools_3_25_00_48\xs.exe ^
REM --cp "c:\Program Files\Java\jre7\lib\ext\RXTXcomm.jar" ^
REM --xdcpath products\uia_1_03_00_02\packages\ ^
REM -f products\uia_1_03_00_02\packages\ti\uia\scripts\UIAHostFromUART.xs ^
REM -p %WORKSPACE%\%PROJNAME%\Debug\%PROJNAME%.out ^
REM -r %WORKSPACE%\%PROJNAME%\Debug\configPkg\package\cfg\%PROJROOT%_p%ext%.rta.xml ^
REM -d C:\Users\A0273433\Desktop\teraterm.log ^
REM -s %WORKSPACE%\%PROJNAME%
