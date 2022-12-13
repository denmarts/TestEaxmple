@echo off

call "%~dp0\..\config.bat"

rem run tests
"%EditorPath%" "%ProjectPath%" -nosplash -log ^
-ExecCmds="Automation RunTests %TestNames%;Quit" -abslog="%TestOutputLogPath%"^
-ReportExportPath="%ReportOutputPath%"

rem copy test artifacts
set TestsDir=%~dp0
set TestsDataDir=%~dp0data
robocopy "%TestsDataDir%" "%ReportOutputPath%" /E

rem start local server and show report
set Port=8081
set Localhost=http://localhost:%Port%

pushd "%ReportOutputPath%"
start "" "%Localhost%"
call http-server -p="%Port%"
popd





