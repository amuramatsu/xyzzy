@echo off
setlocal
cd /d %~dp0

echo Build xyzzy...
cd src
nmake %1 %2 %3 %4 %5 %6 %7 %8 %9
