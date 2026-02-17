@echo off
cd /d "%~dp0"

echo Compiling...
:: Ensure you use the same files as your Ubuntu command
gcc *.c -o A1Compilers.exe

echo Encoding...
:: Added "input/" to the file paths
A2Compilers.exe 1 1 "README.txt" "CODED.txt" 

echo Decoding...
:: Added "input/" to the file paths
A2Compilers.exe 1 0 "CODED.txt" "RESTORED.txt"

echo Done.
pause