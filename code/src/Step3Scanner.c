/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2026
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: March 21 2026
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "../includes/Compilers.h"
#endif

#ifndef BUFFER_H_
#include "../includes/Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "../includes/Step3Scanner.h"
#endif

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
digit line;								/* Current line number of the source code */
digit errorNumber;				/* Defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

digit scannerNumErrors = 0;
ScannerData scData;

/* Keywords */
word tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"ART_OP_T",
	"REL_OP_T",
	"LOG_OP_T",
	"VID_T",
	"COL_T",
	"ASN_T",
	"CMA_T",
	"FPL_T"
};
/* Transition table - type of states defined in separate table */
digit transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*       [A-z],	[0-9], _,      :,      \',   SEOF,    #, other
		     L(0),  D(1),  U(2),   M(3),   Q(4), E(5),  C(6),  O(7),  "(8), =(9), .(10)*/
	/* S0  */{ 1,   10,    ESNR,   ESNR,   4,    ESWR,   6,     14,   12,    14,	ESNR},	// NOAS
	/* S1 */ { 1,   1,     1,      2,      3,    3,      3,     3,    3,     3,		3},			// NOAS (ID chars)
	/* S2 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR (MNID)
	/* S3 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSWR (KEY)
	/* S4 */ { 4,   4,     4,      4,      5,    ESWR,    4,     4,    4,    4,		4},			// NOAS (single-quote string)
	/* S5 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR (SL single-quote)
	/* S6 */ { 6,   6,     6,      6,      6,    ESWR,    7,    6,    6,     6,		6},			// NOAS (comment)
	/* S7 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR (CMT)
	/* S8 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR (Err1 no retract)
	/* S9 */ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSWR (Err2 retract)
	/* S10*/ { 11,  10,    11,     11,     11,   11,     11,    11,   11,    11,	17},			// NOAS (digit accumulator)
	/* S11*/ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSWR (IL accept)
	/* S12*/ { 12,  12,    12,     12,     12,   ESWR,   12,    12,   13,    12,	12},			// NOAS (double-quote string)
	/* S13*/ { FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR (SL double-quote)
	/* S14 */ {16,  16,    16,     16,     16,   16,     16,    16,   16,    15,	16},			// NOAS — lookahead; only '=' (col 9) S15
	/* S15 */ {FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},			// FSNR — 2-char accept (==, <=, >=, !=)
	/* S16 */ {FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,	FS},	// FSWR — 1-char accept (<, >, =, !)
	/* S17 */ {18,  17,    18,     18,     18,   18,     18,    18,   18,    18,    18}, // NOFS (float accum)
	/* S18 */ {FS,  FS,    FS,     FS,     FS,   FS,     FS,    FS,   FS,    FS,    FS}  // FSWR (float accept)
};

digit stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MNID) */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL single-quote) */
	NOFS, /* 06 */
	FSNR, /* 07 (CMT) */
	FSNR, /* 08 (Err1 no retract) */
	FSWR, /* 09 (Err2 retract) */
	NOFS, /* 10 (digit accumulator) */
	FSWR, /* 11 (IL accept) */
	NOFS, /* 12 (double-quote string) */
	FSNR, /* 13 (SL double-quote) */
	NOFS, /* 14 (relational lookahead) */
	FSNR, /* 15 (2-char relational) */
	FSWR, /* 16 (1-char relational) */
	NOFS,
	FSWR, /* 17 (FPL accept) */

};
/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */
PTR_ACCFUN finalStateTable[NUM_STATES] = {
	funcErr, /* 00 */
	funcErr, /* 01 */
	funcID,  /* 02 MNID */
	funcKEY, /* 03 KEY */
	funcErr, /* 04 */
	funcSL,  /* 05 SL single-quote */
	funcErr, /* 06 */
	funcCMT, /* 07 CMT */
	funcErr, /* 08 ERR1 no retract */
	funcErr, /* 09 ERR2 retract */
	funcErr, /* 10 digit accum */
	funcIL,  /* 11 IL */
	funcErr, /* 12 */
	funcSL,  /* 13 SL double-quote */
	funcErr, /* 14 */
	funcREL, /* 15 Accepting State for relational */
	funcREL, /* 16 Accepting State for relational */
	funcErr, /* 17 FPL accum (Not a final state) */
	funcFPL  /* 18 FPL accept */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* The list of keywords */
word keywordTable[KWT_SIZE] = {
	"data",		/* KW00 */
	"code",		/* KW01 */
	"digit",	/* KW02 */
	"rad",		/* KW03 */
	"word",		/* KW04 */
	"if",		/* KW05 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do",		/* KW09 */
	"and",		/* KW10 */
	"or",		/* KW11 */
	"return",	/* KW12 */
	"character",/* KW13 */
	"bigdigit", /* KW14 */
	"bigrad",	/* KW15 */
	"empty",	/* KW16 */
	"duple",	/* KW17 */
	"byte",	/* KW18 */
};
/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
*/

digit startScanner(BufferPointer psc_buf) {
	if (psc_buf == NULL) {
		return STELL_ERR; 
	}
	/* Start histogram */
	for (digit i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(empty) {

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	character c;			/* input symbol */
	digit state = 0;	/* initial state of the FSM */
	digit lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	digit lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	digit lexLength;	/* token length */
	digit i;			/* counter */

	/* Starting lexeme */
	word lexeme;	/* lexeme (to check the function) */
	lexeme = (word)malloc(VID_LEN * sizeof(character));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/
		switch (c) {

		/* Cases for spaces */
		case SPC_CHR:
			
		case TAB_CHR:
			break;
		case NWL_CHR:
			line++;
			break;
		case RET_CHR: // Carriage return
			break;
		case CMA_CHR:
			currentToken.code = CMA_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for symbols */
		case SCL_CHR:
			currentToken.code = EOS_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LPR_CHR:
			currentToken.code = LPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RPR_CHR:
			currentToken.code = RPR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case LBR_CHR:
			currentToken.code = LBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		case RBR_CHR:
			currentToken.code = RBR_T;
			scData.scanHistogram[currentToken.code]++;
			return currentToken;
		/* Cases for END OF FILE */
		case EOS_CHR:
			currentToken.code = SEOF_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		

		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/
		
		// Arithmetic operators:
		/* NEW: Cases for Arithmetic Operators */
		case ADD_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			return currentToken;
		case SUB_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			return currentToken;
		case MUL_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;
		case DIV_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;
		case MOD_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_MOD;
			return currentToken;
		case POW_CHR:
			currentToken.code = ART_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.arithmeticOperator = OP_POW;
			return currentToken;
		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((digit)lexLength + 2, READER_DEFAULT_FACTOR);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			lexeme = readerGetContent(lexemeBuffer, 0);
			// Defensive programming
			if (!lexeme)
				return currentToken;
			currentToken = (*finalStateTable[state])(lexeme);
			readerRestore(lexemeBuffer);
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */

digit nextState(digit state, character c) {
	digit col;
	digit next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */

digit nextClass(character c) {
	digit val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	case COL_CHR:
		val = 3;
		break;
	case QUT_CHR:
		val = 4;
		break; 
	case HST_CHR:
		val = 6;
		break;
	case EOS_CHR:
		val = 5;
		break;
	case EQU_CHR:
		val = 9;
		break;
	case NOT_CHR:
	case LESS_CHR:
	case MORE_CHR:
		val = 7;
		break;
	//case EQU_CHR:
		
		//break;
	case DQT_CHR:
		val = 8;
		break;
	case FPT_CHR:
		val = 10;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 7;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */

Token funcCMT(word lexeme) {
	Token currentToken = { 0 };
	digit i = 0, len = (digit)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
	}
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */

Token funcIL(word lexeme) {
	//printf("DEBUG: funcIL received lexeme: [%s]\n", lexeme);
	Token currentToken = { 0 };
	bigdigit tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (digit)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}

Token funcFPL(word lexeme) {
	Token currentToken = { 0 };
	float val = (float)strtod(lexeme, NULL);
	currentToken.code = FPL_T;
	if ((val<FLT_MIN || val>FLT_MAX) && strtod(lexeme, NULL) != 0.0) {
		return funcErr(lexeme);
	}
	currentToken.attribute.floatValue = val;
	return currentToken;
}

/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */

// Updated to use : for method identifiers
Token funcID(word lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	character lastch = lexeme[length - 1];
	digit isID = FALSE;
	switch (lastch) {
	case COL_CHR:
		currentToken.code = MNID_T;
		scData.scanHistogram[currentToken.code]++;
		isID = TRUE;
		break;
	default:
		// Test Keyword
		///lexeme[length - 1] = EOS_CHR;
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}
	return currentToken;
}



/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(word lexeme) {
	Token currentToken = { 0 };
	digit i = 0, len = (digit)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			printf("DEBUG: Adding to string literal table failed for %c", lexeme[i]);
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		printf("DEBUG: Couldn't append EOS_CHR\n");
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */

Token funcKEY(word lexeme) {
	//printf("Func Key Lexeme: %s", lexeme);
	Token currentToken = { 0 };
	digit kwindex = -1, j = 0;
	///lexeme[len - 1] = EOS_CHR;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0]))
			kwindex = j;
	if (kwindex != -1) {
		if (!strcmp(lexeme, "and")) {
			currentToken.code = LOG_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.logicalOperator = OP_AND;
		}
		else if (!strcmp(lexeme, "or")) {
			currentToken.code = LOG_OP_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.logicalOperator = OP_OR;
		}
		else {
			currentToken.code = KW_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.codeType = kwindex;
		}
	}
	else {
		currentToken.code = VID_T;
		scData.scanHistogram[currentToken.code]++;
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	}
	return currentToken;
}

Token funcREL(word lexeme) {
	Token t = { 0 };
	if (!strcmp(lexeme, "==")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_EQ;
	}
	else if (!strcmp(lexeme, "!=")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_NE;
	}
	else if (!strcmp(lexeme, "<=")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_LE;
	}
	else if (!strcmp(lexeme, ">=")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_GE;
	}
	else if (!strcmp(lexeme, "<")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_LT;
	}
	else if (!strcmp(lexeme, ">")) {
		t.code = REL_OP_T;
		t.attribute.relationalOperator = OP_GT;
	}
	else if (!strcmp(lexeme, "=")) {
		t.code = ASN_T; /* Assignment operator */
	}
	else if (!strcmp(lexeme, "!")) {
		t.code = LOG_OP_T;
		t.attribute.logicalOperator = OP_NOT;
	}
	return t;
}

/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */

Token funcErr(word lexeme) {
	Token currentToken = { 0 };
	digit i = 0, len = (digit)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

empty printToken(Token t) {
	extern word keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (digit)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (digit)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case INL_T:
		printf("INL_T\t\t%d\n", t.attribute.intValue);
		break;
	case FPL_T:
		printf("FPL_T\t\t%f\n", t.attribute.floatValue);
		break;
	case LOG_OP_T: 
		char* logSymbols[] = { "and", "or", "!" };
		printf("LOG_OP_T\t%s\n", logSymbols[t.attribute.logicalOperator]);
		break;
	case REL_OP_T: 
		char* relSymbols[] = { "==", "!=", ">", "<", ">=", "<=" };
		printf("REL_OP_T\t%s\n", relSymbols[t.attribute.relationalOperator]);
		break;
	case COL_T:
		printf("COL_T\t\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case ASN_T:
		printf("ASN_T\n");
		break;
	case CMA_T:
		printf("CMA_T\n");
		break;
	// Arithmetic operator
	case ART_OP_T:
		char* opSymbols[] = { "+", "-", "*", "/", "%", "^"};
		printf("ART_OP_T\t%s\n", opSymbols[t.attribute.arithmeticOperator]);
		break;
	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
empty printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}
