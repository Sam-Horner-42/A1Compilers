/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************                                       "
*/

/*
************************************************************
* File name: MainReader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12, A22, A32.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
* Function list: (...).
*************************************************************/

/*
 *.............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: Preprocessor directives
 * The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define directive does not have any effect on other compiler projects 
 * (Gcc, VSCode, Codeblocks, etc.).
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WRITER_H_
#include "../includes/Step5Writer.h"
#endif

/*
************************************************************
* Main function from Buffer
* This calls upon the reader, scanner and parser to guarantee no syntax errors
* Then is writes the program printing output of print:(); statements,
* calculates integer and float math, and evaluates if statements
* Parameters:
*   argc / argv = Parameters from command prompt
* Return value:
*	Success operation.
************************************************************
*/

digit main5Writer(digit argc, word* argv) {
	if (argc >= 2) {
		word source = argv[2];

		printf("Reading file %s ....Please wait\n", source);
		digit size = getSizeOfFile(source);
		word content = vigenereMem(source, STR_LANGNAME, DECYPHER);

		/* find the size of the file  */
		if (size <= 0) {
			printf("The input file %s %s\n", source, "is not completely loaded.\n");
			return EXIT_FAILURE;
		}

		// Syntax analysis
		printf("\nRunning Syntax Analyzer...\n");

		/* Initialize a temporary Reader buffer (Using size + 1 for EOF, and 0.0f for the increment factor) */
		BufferPointer myBuffer = readerCreate(size + 1, 0.0f);

		/* Allocate memory for the Scanner's string literal table */
		extern BufferPointer stringLiteralTable;
		stringLiteralTable = readerCreate(size, 0.0f);

		if (myBuffer != NULL && stringLiteralTable != NULL) {
			/* Load the decrypted content into the buffer character by character */
			digit i = 0;
			for (i = 0; i < (digit)strlen(content); i++) {
				readerAddChar(myBuffer, content[i]);
			}
			readerAddChar(myBuffer, READER_TERMINATOR); /* Add EOF terminator */

			/* Boot up the Scanner and Parser */
			startScanner(myBuffer);
			startParser();

			/* Check for syntax errors. Because the parser recovers from errors to keep reading,
			   we MUST check these global counters to know if it actually failed. */
			if (syntaxErrorNumber > 0 || numParserErrors > 0) {
				printf("\n[!] FATAL: Compilation failed with %d syntax error(s).\n", (syntaxErrorNumber + numParserErrors));
				printf("[!] Execution aborted. Please fix the code and try again.\n\n");

				readerFree(myBuffer);
				readerFree(stringLiteralTable);
				return EXIT_FAILURE;
			}

			// if we get here the code compiled
			printf("\nInput file: %s successfully compiled.\n", source);

			/* Clean up both buffers, we don't need them anymore */
			readerFree(myBuffer);
			readerFree(stringLiteralTable);
		}
		else {
			printf("Error: Could not create buffers for syntax analysis.\n");
			return EXIT_FAILURE;
		}

		// Semantic analysis the writer
		printf("\nExecuting Stelline Program...\n");
		printf("-------------------------------------------------\n");

		// The program has compiled so we can process the content for running
		process_content(content);

		printf("-------------------------------------------------\n");
	}
	return EXIT_SUCCESS;
}
