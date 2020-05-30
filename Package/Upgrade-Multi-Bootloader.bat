REM
REM Script to install the MULTI-BootReloader app on a MULTI-MODULE.
REM The BootReloader app can be used to upgrade the bootloader on a MULTI-Module without requiring a USB-to-serial adapter.
REM
REM https://github.com/benlye/MultiBootReloader
REM

@ECHO OFF 

SET COMPORT=%1
IF "%COMPORT%" EQU "" GOTO NOCOMPORT

ECHO.
ECHO Checking for MULTI-Module ...
.\tools\find-multi.exe
IF %ERRORLEVEL% GEQ 3 GOTO :EOF
IF %ERRORLEVEL% EQU 1 SET NEXTSTEP=WRITEFLASH
IF %ERRORLEVEL% EQU 2 SET NEXTSTEP=SWITCHDFU

ECHO.
ECHO The MULTI-Module firmware will be replaced by an application which will upgrade the module's bootloader.
ECHO Once begun, DO NOT interrupt the process, otherwise the bootloader will be corrupted and the module will not function.
ECHO If this happens a USB-to-serial (FTDI) adapter can be used to re-program the module.
ECHO.
ECHO The upgrade takes 4-5 seconds and will be complete once the LED goes off and remains off for at least 5 seconds.
ECHO.
PAUSE

GOTO %NEXTSTEP%

:SWITCHDFU
ECHO.
ECHO Resetting MULTI-Module to DFU mode ...
.\tools\maple-reset.exe %COMPORT% 2000
IF %ERRORLEVEL% NEQ 0 GOTO :EOF

:WRITEFLASH
ECHO.
ECHO Writing MULTI-Module Bootloader upgrade app to flash ...
ECHO.
.\tools\dfu-util-multi.exe -R -a 2 -d 1EAF:0003 -D ".\Tools\BootReloader.bin" -v
IF %ERRORLEVEL% NEQ 0 GOTO :EOF

ECHO.
ECHO.
ECHO ###################################################
ECHO #     MULTI-Module bootloader upgrade started     #
ECHO ###################################################
ECHO.
ECHO IMPORTANT: DO NOT UNPLUG THE MULTI-MODULE UNTIL THE LED GOES OFF!
ECHO.
ECHO You may unplug the module once the LED goes off and remains off for at least 5 seconds.  
ECHO.
ECHO You will need to flash new MULTI-Module firmware to the module before it can be used.
GOTO :EOF

:NOCOMPORT
ECHO.
ECHO No COM port specified.
ECHO.
ECHO Usage: Upgrade-Multi-Bootloader.bat [COM Port]
ECHO.
ECHO Example: Upgrade-Multi-Bootloader.bat COM4