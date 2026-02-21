@echo off
cd /d "%~dp0"

echo Compiling...
:: Ensure you use the same files as your Ubuntu command
gcc *.c -o A1Compilers.exe

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