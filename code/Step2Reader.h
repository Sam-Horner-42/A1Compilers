/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Sam Horner
* Professors: Paulo Sousa
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)		/* General error message */
#define READER_TERMINATOR	'\0'		/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE		INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE		100			/* default initial buffer reader capacity */
#define READER_DEFAULT_FACTOR	0.5f		/* default factor */


// BITS                             (    76543210)
#define READER_DEFAULT_FLAG 0x00 	// (0b00000000) = (0x00)_16 = (000)_10
// TO_DO: BIT 3: END = End of buffer flag
#define READER_SET_FLAG_END 0x08	// (0b00001000) = (0x08)_16 = (008)_10
// TO_DO: BIT 2: REL = Rellocation memory flag
#define READER_SET_FLAG_REL 0x04	// (0b00000100) = (0x04)_16 = (004)_10
// TO_DO: BIT 1: EMP = Buffer empty flag
#define READER_SET_FLAG_FUL 0x02	// (0b00000010) = (0x02)_16 = (002)_10
// TO_DO: BIT 0: FUL = Buffer full flag
#define READER_SET_FLAG_EMP 0x01	// (0b00000001) = (0x01)_16 = (001)_10

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* Offset declaration */
typedef struct position {
	digit wrte;					/* the offset to the add chars (in chars) must never be larger than size */
	digit read;					/* the offset to the get a char position (in chars) */
	digit mark;					/* the offset to the mark position (in chars) */
} Position;

/* Flags declaration */
typedef struct flag {
	duple isEmpty;					/* checks if there is no content */
	duple isFull;					/* the content is using all size */
	duple isRead;					/* all content was read */
	duple isMoved;					/* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	word		content;			/* pointer to the beginning of character array (character buffer) */
	digit		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	rad			factor;				/* factor for increase the buffer */
	Flag		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;			/* Offset / position field */
	digit		histogram[NCHAR];	/* Statistics of chars */
	digit		numReaderErrors;	/* Number of errors from Reader */
	digit		checkSum;			/* Sum of bytes(chars) */
} Buffer, * BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate(digit, rad);
BufferPointer	readerAddChar(BufferPointer const, character);
duple	readerClear(BufferPointer const);
duple	readerFree(BufferPointer const);
duple	readerIsFull(BufferPointer const);
duple	readerIsEmpty(BufferPointer const);
duple	readerSetMark(BufferPointer const, digit);
digit	readerPrint(BufferPointer const);
digit	readerLoad(BufferPointer const, word);
duple	readerRecover(BufferPointer const);
duple	readerRetract(BufferPointer const);
duple	readerRestore(BufferPointer const);
digit	readerChecksum(BufferPointer const);
empty	printChar(unsigned char theChar);

/* Getters */
character	readerGetChar(BufferPointer const);
word	readerGetContent(BufferPointer const, digit);
digit 	readerGetPosRead(BufferPointer const);
digit	readerGetPosWrte(BufferPointer const);
digit	readerGetPosMark(BufferPointer const);
digit	readerGetSize(BufferPointer const);
empty	readerPrintFlags(BufferPointer const);
empty	readerPrintStat(BufferPointer const);
digit	readerNumErrors(BufferPointer const);

#endif
