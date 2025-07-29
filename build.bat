@echo off
setlocal enabledelayedexpansion
cd /D "%~dp0"

set exe_name=nightup.exe

:: build.bat          -> debug mode (implicit)
:: build.bat debug    -> debug mode
:: build.bat asan     -> debug mode with asan
:: build.bat release  -> release mode

:: Unpack Arguments
for %%a in (%*) do set "%%a=1"
set L=
if not "%release%"=="1" set "debug=1"
if "%debug%"=="1"   set "release=0" && set "L=[debug mode" && if not "%asan%"=="1" set L=!L!]
if "%asan%"=="1" set "release=0" && set "debug=1" && set "L=%L% with asan]"
if "%release%"=="1" set "debug=0" && set "L=[release mode]"
echo %L%

set compiler=gcc

:: Unpack Command Line Build Arguments
set auto_compile_flags=
if "%asan%"=="1" set auto_compile_flags=%auto_compile_flags% -fsanitize=undefined -fsanitize-trap

:: Compile/Link Line Definitions
set gcc_common=   -I../vendor/ini/ -I../vendor/mlib/ -std=gnu99 -Wall -Wextra -Wpedantic -Wconversion -Wdouble-promotion -Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion -Wno-sign-compare -Wno-unused-result
set gcc_debug=    call %compiler% -g -O0 -DDEBUG %gcc_common% %auto_compile_flags%
set gcc_release=  call %compiler% -Os -s -DNDEBUG %gcc_common% -DNDEBUG -D_NDEBUG %auto_compile_flags%
set gcc_link=
set gcc_out=      -o

:: Choose Compile/Link Lines
if "%debug%"=="1"   set compile=%gcc_debug% && set compile_link=%gcc_link%
if "%release%"=="1" set compile=%gcc_release% && set compile_link=%gcc_link%

:: Prep Directories
if not exist build mkdir build

:: Build Everything
cd build
%compile% ..\src\main.c %compile_link% %gcc_out% %exe_name% || exit /b 1
cd ..
