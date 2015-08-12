@echo off
set QMM_ROOT=%~dp0

set SETX=setx
ver | find "XP" > nul
if %ERRORLEVEL% == 0 (
    set SETX=%QMM_ROOT%\bin\win32\setx.exe
)


echo.
echo config:
echo.
echo QMM_ROOT = "%QMM_ROOT%"
echo.

%SETX% QMM_ROOT %QMM_ROOT%

echo.

pause

