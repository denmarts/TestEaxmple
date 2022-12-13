@echo off

call "%~dp0\..\config.bat"

rem build sources
call "%RunUATPath%" BuildCookRun -project="%ProjectPath%" -platform="%Platform%" -clientconfig="%Configuration%" ^
-build -cook







