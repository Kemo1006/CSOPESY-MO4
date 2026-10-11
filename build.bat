@echo off
REM Build and run CSOPESY (same as pressing F5 in VS Code, without the debugger).
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1"
if errorlevel 1 pause
