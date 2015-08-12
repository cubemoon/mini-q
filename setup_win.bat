@echo off
set QUICK_COCOS2D_MINI=%~dp0

set SETX=setx
ver | find "XP" > nul
if %ERRORLEVEL% == 0 (
    set SETX=%QUICK_COCOS2D_MINI%\bin\win32\setx.exe
)


echo.
echo config:
echo.
echo QUICK_COCOS2D_MINI = "%QUICK_COCOS2D_MINI%"
echo.

%SETX% QUICK_COCOS2D_MINI %QUICK_COCOS2D_MINI%

echo.

pause

