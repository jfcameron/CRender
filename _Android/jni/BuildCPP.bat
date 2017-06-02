:: ************************************************************
:: Name: BuildCPP.bat
:: Purpose: Builds C/CPP libraries for android NDK projects
:: Usage: To be used in Eclipse + ADK, android NDK project
::  Simply run the script, read compiler output and hit
::  any key to recompile.
:: IMPORTANT: Be sure to set up the directory to your ndk-build
::  properly
:: ************************************************************

@Echo Off

:: User prompt
::echo ***************
::echo Compile C & CPP
::echo ***************
:Compile
cls
CALL C:\Users\Joe\Desktop\android-ndk-r10e\ndk-build -C ..

echo.
echo Press any key to recompile...
PAUSE >nul
GOTO Compile

:BadInputPrompt
::echo invalid scene name
::GOTO CreateScene

:END