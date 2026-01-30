@echo off
cd /d "%~dp0"

echo Compiling...
:: Ensure you use the same files as your Ubuntu command
gcc Compilers.c Main1Coder.c Step1Coder.c -o A1Compilers.exe

echo Encoding...
:: Added "input/" to the file paths
A1Compilers.exe 1 1 "input/README.txt" "input/CODED.txt" 

echo Decoding...
:: Added "input/" to the file paths
A1Compilers.exe 1 0 "input/CODED.txt" "input/RESTORED.txt"

echo Done.
pause