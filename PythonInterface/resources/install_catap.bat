@echo off
if [%1]==[] goto usage
C:\Python36\Scripts\pip.exe uninstall %1
C:\Python36\Scripts\pip.exe install %1
goto :eof
:usage
@echo Please supply a .whl to install
exit /B 1


REM SETLOCAL 
REM @setlocal EnableDelayedExpansion 
REM set VERSION=%1
REM @echo on
REM @echo "================="
REM echo "Removing Previous Wheel..."
REM @echo "================="
REM @echo off
REM C:\Python36\Scripts\pip.exe uninstall CATAP
REM @echo on
REM @echo "================="
REM echo "Installing Wheel..."
REM @echo off
REM IF EXIST CATAP-%VERSION%*.whl (
    REM @echo on
    REM @echo "-----WHEEL LIST-----"
    REM echo CATAP-%VERSION%*.whl
    REM echo %VERSION%
    REM @echo off
    REM C:\Python36\Scripts\pip.exe uninstall CATAP-%VERSION%
    REM @echo on
    REM @echo "------------------"
    REM @echo off
REM ) ELSE (
    REM @echo on
    REM @echo "------------------"
    REM echo "NOT FOUND"
    REM @echo "------------------"
    REM @echo off
REM )
REM @echo on
REM @echo "================="
REM @echo "Wheel Installed!"
REM @echo "================="
REM @echo off
REM ENDLOCAL