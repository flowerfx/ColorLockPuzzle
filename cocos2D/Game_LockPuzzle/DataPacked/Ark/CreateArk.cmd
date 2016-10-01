@echo off

set PLATFORM=custom
set DATA_DIR=%PLATFORM%data
set ARCHIVED_DIR=%DATA_DIR%.archived
set PROJECT_DIR=dlcproj.%PLATFORM%
set ARKLIST_FILENAME=0000_Test.arklist

rem Create an arklist
%~dp0\ArkWriter\bin\ArkWriter.exe         ^
  -function      create_arklist                 ^
  -datadir       %~dp0%DATA_DIR%                ^
  -outdir        %~dp0%PROJECT_DIR%             ^
  -filename      %ARKLIST_FILENAME%             ^
  -excludelist   %~dp0%PROJECT_DIR%\exclude.txt
pause

rem Create an archive
rem Must have arklists in the project directory
rem Must have data matching the arklist contents in the data directory

%~dp0\ArkWriter\bin\ArkWriter.exe           ^
  -function      create_arks                      ^
  -datadir       %~dp0%DATA_DIR%                  ^
  -outdir        %~dp0%ARCHIVED_DIR%              ^
  -arklistdir    %~dp0%PROJECT_DIR%               ^
  -externlist    %~dp0%PROJECT_DIR%\external.txt  ^
  -extractlist   %~dp0%PROJECT_DIR%\extract.txt   ^
  -encryptlist   %~dp0%PROJECT_DIR%\encrypt.txt   ^
  -platform      %PLATFORM%                       ^
  -allowdupes    "false"
pause