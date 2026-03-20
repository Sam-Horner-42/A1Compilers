:: A3 SCRIPT
echo off
echo '--------------------------------------------'
echo '-      ALGONQUIN COLLEGE - COM - 25F       -'
echo '--------------------------------------------'
echo '-                                          -'
echo '-    ====                                  -'
echo '-   =                                      -'
echo '-  =       ==   = =  ==  = =   =   ==  ==  -'
echo '-   =     =  = = = = = = = =   ==  =  ==   -'
echo '-    ====  ==  = = = =   = === === =   ==  -'
echo '-                                          -'
echo '--------------------------------------------'
echo '-  [A3: Scanner - Team: Paulo Sousa / God] -'
echo '--------------------------------------------'
set "arg=%1"
set "param=2"
if "%arg%"=="" set "arg=input/CODED.txt"
:: del CODED.txt RESTORED.txt

gcc src/Compilers.c src/Main1Coder.c src/Main2Reader.c src/Main3Scanner.c src/Step1Coder.c src/Step2Reader.c src/Step3Scanner.c -o A3Compilers
timeout /t 1 /nobreak >nul 

A3Compilers 1 1 README.txt %arg%
timeout /t 1 /nobreak >nul

A3Compilers 3 %arg% > output/out.txt 2> output/err.txt
timeout /t 1 /nobreak >nul

dir out.txt err.txt
type out.txt
