@echo off
color 0b
rem used memory manager: possible managers - STD, TBB, FASTMM
rem compilers supported: VC11, VC10
cls
echo.
echo ======================================
echo *           BlizzLikeCore            *
echo ======================================
echo.
SET /p BUILD_TYPE=What build type use?		[Release]: 
if %BUILD_TYPE%. == . SET BUILD_TYPE=Release
SET /p CORE_NUMBER=What is your Core Number?	[4]: 
if %CORE_NUMBER%. == . SET CORE_NUMBER=4
SET /p compiler=What is your Compiler?		[VC11]: 
if %compiler%. == . SET compiler=VC11
SET /p BUILD_PLATFORM=What is your Build Platform?	[Win32]: 
if %BUILD_PLATFORM%. == . SET BUILD_PLATFORM=Win32
SET /p MEMORY_MANAGER=What memory manager use?	[FASTMM]: 
if %MEMORY_MANAGER%. == . SET MEMORY_MANAGER=FASTMM
SET /p INSTALL_PATH=What install path use?	["C:\\BlizzLikeCore"]: 
if %INSTALL_PATH%. == . SET INSTALL_PATH="C:\\BlizzLikeCore"
echo.
if %compiler%==VC11 goto :vc11
if %compiler%==VC10 goto :vc10
goto :help

:vc11
SET COMPILER="Visual Studio 11"
if %BUILD_PLATFORM%==Win64 (SET COMPILER="Visual Studio 11 Win64")
SET COMPILER_PATH="C:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/bin/cl.exe"
SET LINKER_PATH="C:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/bin/link.exe"
SET VC_VARS="C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\"
goto :common

:vc10
SET COMPILER="Visual Studio 10"
if %BUILD_PLATFORM%==Win64 (SET COMPILER="Visual Studio 10 Win64")
SET COMPILER_PATH="C:/Program Files/Microsoft Visual Studio 10.0/VC/bin/cl.exe"
SET LINKER_PATH="C:/Program Files/Microsoft Visual Studio 10.0/VC/bin/link.exe"
SET VC_VARS="C:\\Program Files\\Microsoft Visual Studio 10.0\\VC\\"
goto :common

:help
cls
color 0c
echo Wrong parameters!
echo Restart and set the parameters correctly.
echo.
pause
exit

:common
SET MEMMAN_STR1="0"
SET MEMMAN_STR3="0"
SET MEMMAN_STR2="0"
if %MEMORY_MANAGER%==STD    (SET MEMMAN_STR2="1")
if %MEMORY_MANAGER%==TBB    (SET MEMMAN_STR3="1")
if %MEMORY_MANAGER%==FASTMM (SET MEMMAN_STR1="1")
SET C_FLAGS="/DWIN32 /D_WINDOWS /W3 /EHsc /GR"
goto :begin

:begin
if not exist build (
    mkdir build
)

if not exist %INSTALL_PATH% (
mkdir %INSTALL_PATH%
    if not exist %INSTALL_PATH% (
    echo Please, make output directory %INSTALL_PATH%
    pause
    exit
    )
)

if %BUILD_PLATFORM%==Win32 goto :win32
if %BUILD_PLATFORM%==Win64 goto :win64
goto :help

:win32
cd build
cmake -G %COMPILER% -DPCH=1 -DCMAKE_CXX_COMPILER=%COMPILER_PATH% -DCMAKE_CXX_FLAGS=%C_FLAGS% -DCMAKE_C_FLAGS=%C_FLAGS% -DCMAKE_CXX_COMPILER=%COMPILER_PATH% -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH% -DUSE_FASTMM_MALLOC=%MEMMAN_STR1% -DUSE_STD_MALLOC=%MEMMAN_STR2% -DUSE_TBB_MALLOC=%MEMMAN_STR3% ..
call %VC_VARS%vcvarsall.bat
MSBuild INSTALL.vcxproj /m:%CORE_NUMBER% /t:Rebuild /p:Configuration=%BUILD_TYPE%;Platform=%BUILD_PLATFORM%
goto :end

:win64
cd build
cmake -G %COMPILER% -DPCH=1 -DPLATFORM=X64 -DCMAKE_CXX_FLAGS=%C_FLAGS% -DCMAKE_C_FLAGS=%C_FLAGS% -DCMAKE_CXX_COMPILER=%COMPILER_PATH% -DCMAKE_CXX_COMPILER=%COMPILER_PATH% -DCMAKE_INSTALL_PREFIX=%INSTALL_PATH% -DUSE_FASTMM_MALLOC=%MEMMAN_STR1% -DUSE_STD_MALLOC=%MEMMAN_STR2% -DUSE_TBB_MALLOC=%MEMMAN_STR3% ..
call %VC_VARS%vcvarsall.bat
MSBuild INSTALL.vcxproj /m:%CORE_NUMBER%  /t:Rebuild /p:Configuration=%BUILD_TYPE%;Platform=x64
goto :end

:end
cd ..
pause