@echo off

rem Engine params
set EnginePath_5.1=D:\Engines\UE_5.1
set EnginePath_5.0=D:\Engines\UE_5.0\UnrealEngine

set UBTRelativePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set VersionSelector=%EnginePath_5.1%\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Current engine path
set CurrentEnginePath=%EnginePath_5.0%

rem Editor path (Note: path for UE4: "..\UE4Editor.exe")
set EditorPath=%CurrentEnginePath%\Engine\Binaries\Win64\UnrealEditor.exe

rem !! Engine version for packaging !!
set RunUATPath=%CurrentEnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem PerfreportTool path
set PerfreportPath=%CurrentEnginePath%\Engine\Binaries\DotNET\CsvTools\PerfreportTool.exe

rem Project params
set ProjectRoot=D:\Projects\TestEaxmple
set ProjectPureName=TestExamplePr_git
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Path to CSV file and Output HTML report path
set CSVFileName=Profile
set CSVLocation=%ProjectRoot%\Saved\Profiling\CSV\%CSVFileName%.csv 
set CSVOutputLocation=%ProjectRoot%\Saved\Profiling\CSV

rem Build params
set Platform=Win64
set Configuration=Development
set ArchivePath=%ProjectRoot%\outdir

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// My game copyright
set EXTRA_MODULE_NAMES="%ProjectPureName%"
set TargetTemplateFilePath=%ProjectRoot%\devops\targets\GameModule.Target.cs.template

rem Run
set ServerExePath=%ProjectRoot%\Build\WindowsServer\%ProjectPureName%Server.exe
set ClientExePath=%ProjectRoot%\Build\WindowsClient\%ProjectPureName%Client.exe
set GameExePath=%ProjectRoot%\Build\WindowsNoEditor\%ProjectPureName%.exe

rem Tests
set TestOutputLogPath=%ProjectRoot%\Logs\Tests.log
set ReportOutputPath=%ProjectRoot%\Logs
set UEAutomationContentPath=%CurrentEnginePath%\Engine\Content\Automation
set TestNames=SomethingTest