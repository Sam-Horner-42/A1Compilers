/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Sam Horner
* Professors: Paulo Sousa
*/

/*
***********************************************************
* File name: Step2Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
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

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(digit size, rad factor) {
	BufferPointer readerPointer = NULL;
	if (!size) {
		size = READER_DEFAULT_SIZE;
	} else if (size < 1) {
		printf("Size must be positive.");
		return NULL;
	}
	else {
		readerPointer->size = size;
	}
	/* readerPointer allocation */
	/* Defensive programming: readerPointer */
	readerPointer = malloc(sizeof(Buffer));
	/* content allocation */
	word content = malloc(size);
	/* Defensive programming: content */
	if (readerPointer!=NULL && content!=NULL) {
		readerPointer->content = content;
	}
	
	/* Initialize the histogram */
	if (readerPointer) {
		// Initialize each array value to 0 when reader is created
		for (int i = 0; i < NCHAR; i++) {
			readerPointer->histogram[i] = 0;
		}
	}
	
	/* Initialize errors */
	digit numReaderErrors = 0;
	if (readerPointer != NULL && numReaderErrors != NULL) {
		readerPointer->numReaderErrors = numReaderErrors;
	}
	/* Update the properties */
	readerPointer->position.wrte = 0;
	readerPointer->position.read = sizeof(char); // The offset is the size of a single char
	readerPointer->position.mark = 0;
	numReaderErrors = 0;
	/* Initialize flags */
	readerPointer->flags.isEmpty = TRUE;
	readerPointer->flags.isFull = FALSE;
	readerPointer->flags.isMoved = FALSE;
	readerPointer->flags.isRead = FALSE;
	/* TO_DO: The created flag must be signalized as EMP */
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

BufferPointer readerAddChar(BufferPointer const readerPointer, character ch) {
	word tempReader = NULL;
	digit newSize = 0;
	/* Defensive programming */
	if (!readerPointer) return HOLLOW;

	/* Check for invalid ASCII (0-127) */
	if ((unsigned char)ch < 0 || (unsigned char)ch > 127) {
		readerPointer->numReaderErrors++;
		return readerPointer; // Return without adding the bad char
	}

	/* Check if full */
	if (readerPointer->position.wrte >= readerPointer->size) {

		/* Attempt to resize */
		digit newSize = readerPointer->size + (digit)readerPointer->factor;
		word tempContent = (word)realloc(readerPointer->content, newSize);

		if (!tempContent) {
			readerPointer->numReaderErrors++;
			return HOLLOW;
		}

		/* Update flags and pointers */
		if (tempContent != readerPointer->content)
			readerPointer->flags.isMoved = TRUE;

		readerPointer->content = tempContent;
		readerPointer->size = newSize;
		readerPointer->flags.isFull = FALSE; // No longer full after resize
	}

	/* Add to content and update histogram */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->histogram[(unsigned char)ch]++;

	/* Update empty flag because the buffer is not empty */
	readerPointer->flags.isEmpty = FALSE;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerClear(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Adjust positions to zero */
		readerPointer->position.wrte = 0;
		readerPointer->position.read = sizeof(char); // The offset is the size of a single char
		readerPointer->position.mark = 0;
		/* Adjust flags original */
		readerPointer->flags.isEmpty = TRUE;
		readerPointer->flags.isFull = FALSE;
		readerPointer->flags.isMoved = FALSE;
		readerPointer->flags.isRead = FALSE;
		return TRUE;
	}
	
	return FALSE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer) {
		free(readerPointer);
		return TRUE;
	}
	/* Free memory (buffer/content) */
	return FALSE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerIsFull(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		if (readerPointer->flags.isFull == TRUE) {
			printf("The reader is full.");
		}
		else {
			printf("The reader is not full.");
		}
		return TRUE;
	}
	
	return FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerIsEmpty(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		printf("The reader pointer must be initialized");
		return FALSE;
	}
	/* Check flag if buffer is EMP */
	if (readerPointer->flags.isEmpty) {
		return TRUE;
	}
	return FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerSetMark(BufferPointer const readerPointer, digit mark) {
	
	/* Adjust mark */
	if (readerPointer) {
		readerPointer->position.mark = mark;
	}
	return FALSE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerPrint(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming (including invalid chars) */
	int numCharsRead = 0;
	/* Print the buffer content */
	if (readerPointer) {
		readerPointer->position.read = 0;
		while (readerPointer->position.read != "\0") {
			int i = readerPointer->position.read;
			if ((digit)readerPointer->position.read >= 0 &&
				(digit)readerPointer->position.read <= 127) {
				printf("%c", readerPointer->content[i]);
			}
				
		}
	}
	
	return 0;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerLoad(BufferPointer const readerPointer, word fileName) {
	// Defensive Programming
	if (!readerPointer || !fileName) return READER_ERROR;

	/* Decrypt the file into a string in memory */
	/* Using STR_LANGNAME ("Stelline") as the key per  */
	word decodedContent = vigenereMem(fileName, STR_LANGNAME, DECYPHER);

	if (!decodedContent) return READER_ERROR;

	digit count = 0;
	/* Loop through the decrypted string and add to buffer */
	for (int i = 0; decodedContent[i] != '\0'; i++) {
		if (readerAddChar(readerPointer, decodedContent[i])) {
			count++;
		}
	}

	/* Free the temporary string allocated by vigenereMem */
	free(decodedContent);
	return count;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerRecover(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Adjust read and mark to zero */
		readerPointer->position.read = 0;
		readerPointer->position.mark = 0;
		return TRUE;
	}
	return FALSE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Retract (return 1 pos read) */
	return FALSE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
duple readerRestore(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Restore read to mark */
		readerPointer->position.read = readerPointer->position.mark;
		return TRUE;
	}
	return FALSE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
character readerGetChar(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer || !readerPointer->content) {
		return '\0';
	}

	/* Check if we have reached the end of the written data */
	if (readerPointer->position.read == readerPointer->position.wrte) {
		readerPointer->flags.isRead = TRUE; // Signal end of buffer
		return '\0';
	}

	/* Reset end-of-buffer flag just in case */
	readerPointer->flags.isRead = FALSE;

	/* 1. Get the character at the current read position */
	character ch = readerPointer->content[readerPointer->position.read];

	/* 2. Update the read position for the next call */
	readerPointer->position.read++;

	return ch;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
word readerGetContent(BufferPointer const readerPointer, digit pos) {
	/* Defensive programming */
	if (readerPointer)
		/* Return content (string) */
		return readerPointer->content;
	return NULL;
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerGetPosRead(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Return read */
		return readerPointer->position.read;
	}
	
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerGetPosWrte(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Return wrte */
		return readerPointer->position.wrte;
	}
	
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerGetPosMark(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) 
		/* Return mark */
		return readerPointer->position.mark;
	
	return 0;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer) {
		return readerPointer->size;
	}
	/* TO_DO: Return size */
	return 0;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
Flag readerPrintFlags(BufferPointer const readerPointer) {
	Flag emptyFlags = { 0,0,0,0 };
	/* Defensive programming */
	if (!readerPointer) return emptyFlags; // All 0
	/* Return flags */
	return readerPointer->flags;
}
#else
#define bGetFlags(readerPointer) ((readerPointer)?(readerPointer->flags):(RT_FAIL_1))
#endif

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
empty readerPrintStat(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return;
	}

	/* Use the NCHAR constant defined in the header */
	for (int i = 0; i < NCHAR; i++) {
		/* Only print if the character actually appeared in the file */
		if (readerPointer->histogram[i] > 0) {
			printf("B[%.3d]=%d, ", i, readerPointer->histogram[i]);
		}
	}
	printf("\n");
	
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
digit readerNumErrors(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer) {
		/* Return the number of errors */
		return readerPointer->numReaderErrors;
	}
	
	return 0;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	the value of bytes in the reader
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

digit readerChecksum(BufferPointer readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer) {
		return readerPointer->checkSum;
	}
	/* TO_DO: Return the checksum (given by the content) */
	return 0;
}
