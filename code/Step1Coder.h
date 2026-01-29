/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Sam Horner
* Professors: Paulo Sousa
************************************************************
                                        "
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#define CODER_H_

 // ASCII range for visible characters
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

#define CYPHER 1
#define DECYPHER 0

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
empty vigenereFile(const word inputFileName, const word outputFileName, const word key, digit encode);
word vigenereMem(const word inputFileName, const word key, digit encode);

empty cypher(const word inputFileName, const word outputFileName, const word key);
empty decypher(const word inputFileName, const word outputFileName, const word key);
digit getSizeOfFile(const word fileName);

#endif
