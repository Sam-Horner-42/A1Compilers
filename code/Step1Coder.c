/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Sam Horner
* Professors: Paulo Sousa
************************************************************
*/

/*
***********************************************************
* File name: Step1Coder.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A1 - Coder Implementation.
* Date: Sep 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
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

// Function to perform the Vigenère cipher (encoding or decoding) [3]
empty vigenereFile(const word inputFileName, const word outputFileName, const word key, digit encode) {
    // Defensive programming, check for missing parameters
    if (inputFileName == HOLLOW || outputFileName == HOLLOW || key == HOLLOW) {
        fprintf(stderr, "Invalid parameter passed to vigenereFile.\n");
        return;
    }

    // Check to see if the encode integer is 0 or 1
    if (encode != CYPHER && encode != DECYPHER) {
        fprintf(stderr, "Incorrect cypher code. Must be CYPHER(1) or DECYPHER(0).\n");
        return;
    }


    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == HOLLOW) {
        fprintf(stderr, "Not able to open output file: %s\n", outputFileName);
        //fclose(outputFile); // Close input file if we can't find/open output
        return;
    }

    word output = vigenereMem(inputFileName, key, encode);
    if(output)
        fputs(output, outputFile); // Put the current character into the file
    
    fclose(outputFile);
}

// Function to perform the Vigenère cipher (encoding or decoding) [3]
word vigenereMem(const word inputFileName, const word key, digit encode) {

    // Defensive programming
    if (inputFileName == HOLLOW || key == HOLLOW) {
        fprintf(stderr, "Invalid parameters passed to vigenereMem.\n");
        return HOLLOW;
    }

    if (encode != CYPHER && encode != DECYPHER) {
        fprintf(stderr, "Incorrect cypher code.\n");
        return HOLLOW;
    }

    // Get file size
    digit size = getSizeOfFile(inputFileName);
    if (size <= 0) {
        fprintf(stderr, "File is empty or could not be opened.\n");
        return HOLLOW;
    }

    // Allocate memory for output using malloc
    word output = (word)malloc(size + 1);   // +1 for null terminator char
    // Check if the allocation failed
    if (output == HOLLOW) {
        free(output);
        fprintf(stderr, "Memory allocation failed.\n");
        return HOLLOW;
    }

    // Load entire file into memory
    FILE* file = fopen(inputFileName, "rb");
    // Check if file failed to open
    if (file == HOLLOW) {
        fprintf(stderr, "Unable to open file: %s\n", inputFileName);
        free(output);
        return HOLLOW;
    }


    digit bytesRead = fread(output, 1, size, file); // Read the whole file into the buffer
    fclose(file); // Avoid memory leaks

    // All the bytes were read into the buffer successfully

    if (bytesRead != size) {
        fprintf(stderr, "Error reading file contents.\n");
        free(output);
        return HOLLOW;
    }

    output[size] = EOF_CHAR;  // Null-terminate for safety

    // Apply cypher to buffer
    digit keyLen = strlen(key); // Length of the key to creat the array
    // Check to make sure the key is not empty
    if (strlen(key) == 0) {
        fprintf(stderr, "Key cannot be empty.\n");
        return HOLLOW;
    }
    
    // The first index in the key
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

            output[i] = currentChar; // Append the char to the buffer
            keyIndex++; // Increment the key value
        } else if (currentChar < 0 || currentChar > ASCII_END) {
            fprintf(stderr, "Invalid ASCII value detected in file.\n");
            free(output);
            return HOLLOW;
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

// Get file size (util method)
digit getSizeOfFile(const word fileName) {
	digit size = 0; // The size of the file
	FILE* inputFile; // The file coming in

	inputFile = fopen(fileName, "r");
	// [1]
	// Print some text if the file does not exist
	if (inputFile == HOLLOW) {
		printf("Not able to open the input file: %s", fileName);
		return 0; // Size of the file is 0
	}

	//[2]
	fseek(inputFile, 0L, SEEK_END); // Go through to the end of the file

	// Calculating the size of the file
	size = ftell(inputFile);
    if (size < 0) {
        fprintf(stderr, "Error determining file size.\n");
        fclose(inputFile);
        return 0;
    }

	// Closing the file
	fclose(inputFile);

    return size;
}

// References:
// [1] W3Schools, "C Read Files," www.w3schools.com.https://www.w3schools.com/c/c_files_read.php‌
// [2] GeeksforGeeks, “C Program to find size of a File, ” GeeksforGeeks, Apr. 28, 2018. https://www.geeksforgeeks.org/c/c-program-find-size-file/ (accessed Jan. 27, 2026).‌
// [3] GeeksforGeeks, “Vigenere Cipher, ” GeeksforGeeks, Oct. 07, 2016. https://www.geeksforgeeks.org/dsa/vigenere-cipher/‌