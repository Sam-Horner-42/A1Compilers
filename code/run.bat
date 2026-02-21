@echo off
cd /d "%~dp0"

echo Encoding...
:: Added "input/" to the file paths
A2Compilers.exe 1 1 "Input.stell" "Coded.txt" 

echo Decoding...
:: Added "input/" to the file paths
A2Compilers.exe 1 0 "Coded.stell" "Restored.stell"

echo Running Buffer Reader...
A2Compilers.exe 2 "Coded.stell"
echo Done.
pause