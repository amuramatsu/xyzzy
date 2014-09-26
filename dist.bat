@echo off
rem
rem _dist/ に xyzzy のソースをチェックアウトしてクリーンビルドを行い、
rem xyzzy-(VERSION).zip と xyzzy-src-(VERSION).zip を作成する。
rem

setlocal
cd /d %~dp0

if "%1"=="" goto usage

set VERSION=%1

set TAG=v%VERSION%
set APPNAME=xyzzy

set BASEDIR=%~dp0
set GIT_REPO=%BASEDIR%
set DISTROOT=%BASEDIR%\_dist
set DISTDIR=%BASEDIR%\_dist\%APPNAME%
set SRCDIR=%BASEDIR%\_dist\%APPNAME%-src-%VERSION%
set DIST_ARCHIVE=%DISTROOT%\%APPNAME%-%VERSION%.zip
set SRC_ARCHIVE=%DISTROOT%\%APPNAME%-src-%VERSION%.zip

call git tag %TAG% -a -m "%APPNAME% %VERSION% released!" || exit /b 1
call git tag

cd %BASEDIR%
rd /S /Q %DISTROOT% 2> nul

mkdir %DISTROOT%
mkdir %SRCDIR%
mkdir %DISTDIR%
mkdir %DISTDIR%\lisp
mkdir %DISTDIR%\etc
mkdir %DISTDIR%\docs
mkdir %DISTDIR%\reference
mkdir %DISTDIR%\site-lisp

cd %SRCDIR%
call git clone %GIT_REPO% %SRCDIR% || exit /b 1
call git checkout %TAG% || git tag exit /b 1
rd /S /Q .git 2> nul
cd %DISTROOT%
rem 7za a %SRC_ARCHIVE% %SRCDIR%
zip -r -9 %SRC_ARCHIVE% %APPNAME%-src-%VERSION%
cd %BASEDIR%
call build.bat || exit /b 1
call bytecompile.bat || exit /b 1

xcopy /F /G /H /R /K /Y *.exe %DISTDIR%
xcopy /F /G /H /R /K /Y LICENSE %DISTDIR%\docs\
xcopy /F /G /H /R /K /Y LEGAL.md %DISTDIR%\docs\
xcopy /F /G /H /R /K /Y *.txt %DISTDIR%\docs\
xcopy /F /G /H /R /K /Y /S /E lisp %DISTDIR%\lisp\
xcopy /F /G /H /R /K /Y /S /E etc %DISTDIR%\etc\
xcopy /F /G /H /R /K /Y /S /E docs %DISTDIR%\docs\
xcopy /F /G /H /R /K /Y /S /E reference %DISTDIR%\reference\

cd %DISTROOT%
rem 7za a %DIST_ARCHIVE% %DISTDIR%
zip -r -9 %DIST_ARCHIVE% %APPNAME%
goto :eof

:usage
echo Usage: %0 VERSION
goto :eof
