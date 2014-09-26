@echo off
setlocal
cd /d %~dp0

echo Build xyzzy...
cd src
nmake
