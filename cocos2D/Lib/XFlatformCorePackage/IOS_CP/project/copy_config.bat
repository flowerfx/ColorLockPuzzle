set SourceDir=%1
set TargetDir=%configDir%\%2
SET copyConfig=y;
:: Verify if TargetDir already exists
IF (%3)==(force_copy) GOTO nocheck
ELSE (
IF EXIST %TargetDir% (
   echo Folder %2 already exists.
   set /p copyConfig="Overwrite its content (y/n) ?"
)ELSE (
md "%TargetDir%"
set copyConfig=y
)
)
:: Copy all user config files in SourceDir
:nocheck
IF %copyConfig%==y (
echo Copying %2 config files
FOR %%A IN (%SourceDir%\*.*) DO (
COPY %%A %TargetDir%
)
)
