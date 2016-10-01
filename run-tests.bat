@echo off
setlocal
cd /d %~dp0

rem Usage: run-tests.bat [TESTS...]
rem
rem Example:
rem  ���ׂẴe�X�g�����s
rem  run-tests.bat
rem
rem  lisp-tests.l ���������s
rem  run-tests.bat unittest/lisp-tests.l
rem
rem  lisp-tests.l �� editor-tests.l ���������s
rem  run-tests.bat unittest/lisp-tests.l unittest/editor-tests.l

set TESTDIR=%~dp0unittest
set XYZZYHOME=%TESTDIR%\..
set XYZZYINIFILE=
set XYZZYCONFIGPATH=
"%XYZZYHOME%\xyzzy.exe" -q -trace -l "%TESTDIR%\run-tests-helper.l" %*
type run-tests.log
findstr "Failed$" run-tests.log >NUL:
IF ERRORLEVEL 1 GOTO OK
exit 1
:OK
exit 0
