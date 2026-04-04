/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "../includes/Compilers.h"
#endif
#ifndef READER_H_
#include "../includes/Step2Reader.h"
#endif
#ifndef SCANNER_H_
#include "../includes/Step3Scanner.h"
#endif

/* Global vars */
extern Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern digit		line;
extern Token			tokenizer(empty);
extern word		keywordTable[KWT_SIZE];
extern digit		syntaxErrorNumber;

// Defined method names unique to this language
// All methods/functions must be ended in ':'
#define LANG_WRTE		"print:"
#define LANG_READ		"input:"
#define LANG_MAIN		"main:"


/* Expand BNF Rules to support arithmetic expressions (Rubric Minimum) */
#define NUM_BNF_RULES 19

/* Parser */
typedef struct parserData {
	digit parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
extern digit numParserErrors;

/* Scanner data */
extern ParserData psData;

/* Function definitions */
empty startParser();
empty matchToken(digit, digit);
empty syncErrorHandler(digit);
empty printError();
empty printBNFData(ParserData psData);

/* Keyword Enums matching Stelline's Scanner keywordTable indices */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_data = 0,
	KW_code = 1,
	KW_digit = 2,
	KW_rad = 3,
	KW_word = 4,
	KW_if = 5,
	KW_else = 6,
	KW_while = 7,
	KW_do = 8,
	KW_and = 9,
	KW_or = 10,
	KW_return = 11,
	KW_character = 12,
	KW_bigdigit = 13,
	KW_bigrad = 14,
	KW_empty = 15,
	KW_duple = 16,
	KW_byte = 17
};



/* Add these new non-terminal function declarations to the bottom of the file */
empty assignmentStatement();
empty arithmeticExpression();
empty arithmeticExpressionPrime();
empty primaryExpression();
empty variableDeclarationStatement();

/* ALL non-terminal function declarations */
empty codeSession();
empty comment();
empty dataSession();
empty optVarListDeclarations();
empty optionalStatements();
empty outputStatement();
empty outputVariableList();
empty program();
empty statement();
empty statements();
empty statementsPrime();
empty optParams();
empty paramList();
empty selectionStatement();
empty conditionalExpression();
empty iterationStatement();

empty topLevelDeclaration();
empty returnStatement();
empty functionCallStatement();
empty argList();

#endif
