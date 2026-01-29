/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Sam Horner
* Professors: Paulo Sousa
************************************************************
* /

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
    // Defensive programming, check for missing parameters
    if (inputFileName == NULL || outputFileName == NULL || key == NULL) {
        fprintf(stderr, "Invalid parameter passed to vigenereFile.\n");
        return;
    }

    // Check to see if the encode integer is 0 or 1
    if (encode != CYPHER && encode != DECYPHER) {
        fprintf(stderr, "Incorrect cypher code. Must be CYPHER(1) or DECYPHER(0).\n");
        return;
    }

    // Open neccessary input and output files
    FILE* inputFile = fopen(inputFileName, "r");

    // Check to make sure the files were found and opened correctly
    if (inputFile == NULL) {
        fprintf(stderr, "Not able to open input file: %s\n", inputFileName);
        return;
    }

    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Not able to open output file: %s\n", outputFileName);
        fclose(inputFile); // Close input file if we can't find/open output
        return;
    }

    int keyLen = strlen(key); // Length of the key used
    int keyIndex = 0; // The current index to apply to
    int currentChar;  // Incoming character from file stored as an int to avoid truncation

    while ((currentChar = fgetc(inputFile)) != EOF) {

        // Only modify visible ASCII characters
        if (currentChar >= ASCII_START && currentChar <= ASCII_END) {

            int shift = key[keyIndex % keyLen] - ASCII_START;

            if (encode == CYPHER) {
                // Forward shift
                currentChar = ASCII_START +
                    ((currentChar - ASCII_START) + shift) % ASCII_RANGE;
            }
            else {
                // Backward shift (decode)
                currentChar = ASCII_START +
                    ((currentChar - ASCII_START) - shift + ASCII_RANGE) % ASCII_RANGE;
            }

            keyIndex++;
        }

        fputc(currentChar, outputFile); // Put the current character into the file
    }

    // Close files
    fclose(inputFile);
    fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding)
word vigenereMem(const word inputFileName, const word key, digit encode) {

    // Defensive programming
    if (inputFileName == NULL || key == NULL) {
        fprintf(stderr, "Invalid parameters passed to vigenereMem.\n");
        return NULL;
    }

    if (encode != CYPHER && encode != DECYPHER) {
        fprintf(stderr, "Incorrect cypher code.\n");
        return NULL;
    }

    // Get file size
    digit size = getSizeOfFile(inputFileName);
    if (size <= 0) {
        fprintf(stderr, "File is empty or could not be opened.\n");
        return NULL;
    }

    // Allocate memory for output using malloc
    word output = (word)malloc(size + 1);   // +1 for null terminator char
    // Check if the allocation failed
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Load entire file into memory
    FILE* file = fopen(inputFileName, "r");
    // Check if file failed to open
    if (file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", inputFileName);
        free(output);
        return NULL;
    }


    digit bytesRead = fread(output, 1, size, file); // Read the whole file into the buffer
    fclose(file); // avoid memory leaks

    // All the bytes were read into the buffer successfully
    if (bytesRead != size) {
        fprintf(stderr, "Error reading file contents.\n");
        free(output);
        return NULL;
    }

    output[size] = '\0';  // Null-terminate for safety

    // Apply cypher to buffer
    digit keyLen = strlen(key);
    digit keyIndex = 0;

    for (digit i = 0; i < size; i++) {
        digit currentChar = output[i];

        // Only change visible ASCII 32–126
        if (currentChar >= ASCII_START && currentChar <= ASCII_END) {

            digit shift = key[keyIndex % keyLen] - ASCII_START;

            if (encode == CYPHER) {
                currentChar = ASCII_START + ((currentChar - ASCII_START + shift) % ASCII_RANGE);
            }
            else {
                currentChar = ASCII_START + ((currentChar - ASCII_START - shift + ASCII_RANGE) % ASCII_RANGE);
            }

            output[i] = currentChar;
            keyIndex++;
        }
    }

    // Return buffer
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