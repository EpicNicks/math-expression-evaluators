@echo off
setlocal enabledelayedexpansion

:: Change to the build directory
cd .\build

:: Purge the build directory
echo Purging build directory...
for /D %%D in (*) do rmdir /S /Q "%%D"
del /F /Q *

:: Reconfigure the project
echo Reconfiguring project...
cmake ..

:: Build the project
echo Building project...
cmake --build .

:: Check if build was successful
if %ERRORLEVEL% neq 0 (
    echo Build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

:: Combine all arguments into a single string
set "args="
:combine_args
if "%~1"=="" goto :end_args
set "args=!args! %~1"
shift
goto :combine_args
:end_args

:: Remove leading space
if defined args set "args=!args:~1!"

:: Run the program with arguments
echo Running math_evaluator with arguments: %args%
.\Debug\math_evaluator.exe %args%

:: Capture the exit code of the C program
set exit_code=%ERRORLEVEL%

:: Optionally, you can do something with the exit code
if %exit_code% neq 0 (
    echo C program exited with code %exit_code%
)

:: Change back to the original directory
cd ..

:: Exit with the same code as the C program
exit /b %exit_code%