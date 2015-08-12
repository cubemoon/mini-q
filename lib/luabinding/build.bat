@echo off
set DIR=%~dp0
set TOLUA=%QMM_ROOT%\bin\win32\tolua++.exe

cd /d "%DIR%"
%TOLUA% -L "%DIR%basic.lua" -o "%QMM_ROOT%\lib\cocos2d-x\scripting\lua\cocos2dx_support\LuaCocos2d.cpp" Cocos2d.tolua
