@echo off 
SET MAKETOOL=..\..\premake\release\premake5.exe

rem generates the .pbxproj files
%MAKETOOL% --arch=ios  xcode4

rem pause helps us to actually see the execution of the script and deal with it if there is a problem
pause