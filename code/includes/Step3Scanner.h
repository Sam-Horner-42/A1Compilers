/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2025
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
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
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "../includes/Compilers.h"
#endif

#ifndef READER_H_
#include "../includes/Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* The number of tokens */
#define NUM_TOKENS 20

#define KWT_SIZE 18
/* Token codes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token ends with ( */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	LBR_T,		/*  6: Left brace token */
	RBR_T,		/*  7: Right brace token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of statement (semicolon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T,		/* 12: Comment token */
	ART_OP_T,	/* 13: Arithmetic operator token */
	REL_OP_T,	/* 14: Relational operator token */
	LOG_OP_T,	/* 15: Logical operator token */
	VID_T,		/* 16: Variable Identifier token */
	COL_T,		/* 17: Col Identifier token defines the beginning of a function */
	ASN_T,		/* 18: Single equals sign, assigment token*/
	CMA_T,		/* 19: Comma token */

};

/* Keywords */
extern word tokenStrTable[NUM_TOKENS];

/* Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* Data structures for the token and its attributes */
typedef union TokenAttribute {
	digit codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	digit intValue;				/* integer literal attribute (value) */
	digit keywordIndex;			/* keyword index in the keyword table */
	digit contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	rad floatValue;				/* floating-point literal attribute (value) */
	character idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	character errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	byte flags;			/* Flags information */
	union {
		digit intValue;			/* Integer value */
		rad floatValue;			/* Float value */
		word stringContent;		/* String value */
		bigdigit longValue;
		bigrad doubleValue;
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	digit code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	digit scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* Lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define AMP_CHR '&'		// CH03
#define QUT_CHR '\''	// CH04
#define HST_CHR '#'		// CH05
#define TAB_CHR '\t'	// CH06
#define SPC_CHR ' '		// CH07
#define NWL_CHR '\n'	// CH08
#define RET_CHR '\r'
#define SCL_CHR ';'		// CH09
#define LPR_CHR '('		// CH10
#define RPR_CHR ')'		// CH11
#define LBR_CHR '{'		// CH12
#define RBR_CHR '}'		// CH13

/* Arithmetic definitions */
#define ADD_CHR '+'		// CH14
#define SUB_CHR '-'		// CH15
#define MUL_CHR '*'		// CH16
#define DIV_CHR '/'		// CH17

/* Logical Operators */
#define NOT_CHR '!'		// CH18
#define LESS_CHR '<'
#define MORE_CHR '>'
/* String Handling */
#define DQT_CHR '"'

#define COL_CHR ':'

/* Relational Operators */
#define EQU_CHR '='

#define LESS_CHR '<'
#define GRT_CHR '>'

#define CMA_CHR ','



/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

 /* Error states and illegal state */
#define ESNR	8		/* Error state with no retract */
#define ESWR	9		/* Error state with retract */
#define FS		18  /* Illegal state */

 /* State transition table definition */
#define NUM_STATES    17
#define CHAR_CLASSES  10

/* Transition table - type of states defined in separate table */
extern digit transitionTable[NUM_STATES][CHAR_CLASSES];

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

extern digit stateType[NUM_STATES];

/* Static (local) function  prototypes */
digit			startScanner(BufferPointer psc_buf);
digit	nextClass(character c);					/* character class function */
digit	nextState(digit, character);		/* state machine function */
empty			printScannerData(ScannerData scData);
Token			tokenizer(empty);


/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(word lexeme);

/* Declare accepting states functions */
Token funcSL	(word lexeme);
Token funcIL	(word lexeme);
Token funcID	(word lexeme);
Token funcCMT   (word lexeme);
Token funcKEY	(word lexeme);
Token funcErr	(word lexeme);
Token funcREL	(word lexeme);

extern digit transitionTable[NUM_STATES][CHAR_CLASSES];
extern digit stateType[NUM_STATES];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern word keywordTable[KWT_SIZE];
extern word tokenStrTable[NUM_TOKENS];


/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	character indentationCharType;
	digit indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
extern digit numScannerErrors;

/* Scanner data */
extern ScannerData scData;

#endif
