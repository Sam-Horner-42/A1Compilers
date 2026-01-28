/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@           %&@@@@@@@@@@@     @@    ”
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    ”
# ECHO "    @@      @& @   @ @       @     @@    ”
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    ”
# ECHO "    @@      & @ @  @@              @@    ”
# ECHO "    @@       @/ @*@ @ @   @        @@    ”
# ECHO "    @@           @@@@  @@ @ @      @@    ”
# ECHO "    @@            /@@    @@@ @     @@    ”
# ECHO "    @@     @      / /     @@ @     @@    ”
# ECHO "    @@     @ @@   /@/   @@@ @      @@    ”
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@         S O F I A           @@    ”
# ECHO "    @@                             @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

// Function to perform the Vigenère cipher (encoding or decoding)
empty vigenereFile(const word inputFileName, const word outputFileName, const word key, digit encode) {
	// TO_DO: Define the input and output files (ex: FILE* inputFile, FILE* outputFile
	FILE* inputFile;
	FILE* outputFile;
	inputFile = fopen(inputFileName, 'r');
	outputFile = fopen(outputFileName, 'w');
	// [1]
	// Print some text if the file does not exist
	if (inputFile == NULL) {
		printf("Not able to open the input file: %s", inputFileName);
	}

	// [1]
	// Print some text if the file does not exist
	if (outputFile == NULL) {
		fprintf(stderr, "Not able to open the output file: %s", outputFileName);
	}
	// TO_DO: Use defensive programming (checking files)
	if (encode != CYPHER && encode != DECYPHER) { printf("Incorrect cypher code"); }
	else if (encode == CYPHER) { cypher(inputFileName, outputFileName, key); }
	else if (encode == CYPHER) { decypher(inputFileName, outputFileName, key); }
	
	// TO_DO: Define local variables
	word convertedText;

	// TO_DO: Logic: check if it is encode / decode to change the char (using Vigenere algorithm) - next function
	// TO_DO: Close the files
}

// Function to perform the Vigenère cipher (encoding or decoding)
word vigenereMem(const word inputFileName, const word key, digit encode) {
	// TO_DO define the return type and local variables
	word output = NULL;
	// TO_DO: Check defensive programming
	if (encode == CYPHER) {

	}
	else if (encode == DECYPHER) {

	}
	
	// TO_DO: Use the logic to code/decode - consider the logic about visible chars only
	return output;
}

// Function to encode (cypher)
void cypher(const word inputFileName, const word outputFileName, const word key) {
    vigenereFile(inputFileName, outputFileName, key, CYPHER);
}

// Function to decode (decypher)
void decypher(const word inputFileName, const word outputFileName, const word key) {
    vigenereFile(inputFileName, outputFileName, key, DECYPHER);
}

// TO_DO: Get file size (util method)
digit getSizeOfFile(const word fileName) {
	digit size = 0;
    // TO_DO: Use the logic to get the size of the file
	FILE* inputFile;

	inputFile = fopen(fileName, "r");
	// [1]
	// Print some text if the file does not exist
	if (inputFile == NULL) {
		printf("Not able to open the input file: %s", fileName);
		return 1; // Error code 1
	}

	//[2]
	fseek(inputFile, 0L, SEEK_END); // Go through to the end of the file

	// Calculating the size of the file
	size = ftell(inputFile);

	// Closing the file
	fclose(inputFile);

    return size;
}

// References:
// [1] W3Schools, "C Read Files," www.w3schools.com.https://www.w3schools.com/c/c_files_read.php‌
// [2] GeeksforGeeks, “C Program to find size of a File, ” GeeksforGeeks, Apr. 28, 2018. https://www.geeksforgeeks.org/c/c-program-find-size-file/ (accessed Jan. 27, 2026).‌