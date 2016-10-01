@echo off

set configDir=..\iOSCorePackage_config
set copyConfigFile=y;
:: Create folder "iOSCorePackage_config" to contain OnlineFramework configuration files (if does not already exists)
IF NOT EXIST %configDir% md %configDir%
IF (%1)==(force_copy) GOTO nocheck
ELSE (
:: Verify if iOSCorePackage_config.h already exists
IF EXIST "%configDir%\iOSCorePackage_config.h" (
set /p copyConfigFile="File 'iOSCorePackage_config.h' already exists. Do you want to overwrite it (y/n) ?"
)ELSE (
set copyConfigFile=y
)
)
:: Copy iOSCorePackage_config.h
:nocheck
IF  %copyConfigFile%==y (
echo Copying iOSCorePackage_config.h
copy "include\iOSCorePackage_config.h.template" "%configDir%\iOSCorePackage_config.h"
)

echo.

:: Copy user .xcconfig files
call project\copy_config.bat project\xcode4\ios\xcconfigs_iOSCorePackage\user xcode4\ios\xcconfigs_iOSCorePackage %1%

echo.

