@echo off

call "%~dp0\..\config.bat"

rem converting
call %PerfreportPath% -csv %CSVLocation% -o %CSVOutputLocation% -readAllStats -showHiddenStats
pause








