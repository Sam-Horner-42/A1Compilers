/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************
#
# "=---------------------------------------="
# "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# "=---------------------------------------="
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "    @@                             @@    ”
# "    @@           %&@@@@@@@@@@@     @@    ”
# "    @@       @%% (@@@@@@@@@  @     @@    ”
# "    @@      @& @   @ @       @     @@    ”
# "    @@     @ @ %  / /   @@@@@@     @@    ”
# "    @@      & @ @  @@              @@    ”
# "    @@       @/ @*@ @ @   @        @@    ”
# "    @@           @@@@  @@ @ @      @@    ”
# "    @@            /@@    @@@ @     @@    ”
# "    @@     @      / /     @@ @     @@    ”
# "    @@     @ @@   /@/   @@@ @      @@    ”
# "    @@     @@@@@@@@@@@@@@@         @@    ”
# "    @@                             @@    ”
# "    @@         S O F I A           @@    ”
# "    @@                             @@    ”
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

#ifndef COMPILERS_H_
#include "../includes/Compilers.h"
#endif

#ifndef SCANNER_H_
#include "../includes/Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "../includes/Step4Parser.h"
#endif

/* Actual Definitions (Memory Allocation) */
Token lookahead;
digit syntaxErrorNumber = 0;
digit numParserErrors = 0;
ParserData psData;


/*
************************************************************
 * Process Parser
 ***********************************************************
 */

/* List of BNF statements */
enum BNF_RULES {
	BNF_error, BNF_codeSession, BNF_comment, BNF_dataSession,
	BNF_optVarListDeclarations, BNF_optionalStatements, BNF_outputStatement,
	BNF_outputVariableList, BNF_program, BNF_statement, BNF_statements,
	BNF_statementsPrime, BNF_optParams, BNF_returnStatement,
	BNF_assignmentStatement, BNF_arithmeticExpression,
	BNF_arithmeticExpressionPrime, BNF_primaryExpression, BNF_variableDeclarationStatement
};

static word BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error", 
	"BNF_codeSession", 
	"BNF_comment", 
	"BNF_dataSession",
	"BNF_optVarListDeclarations", 
	"BNF_optionalStatements", 
	"BNF_outputStatement",
	"BNF_outputVariableList", 
	"BNF_program", 
	"BNF_statement", 
	"BNF_statements",
	"BNF_statementsPrime", 
	"BNF_optParams", 
	"BNF_returnStatement",
	"BNF_assignmentStatement", 
	"BNF_arithmeticExpression",
	"BNF_arithmeticExpressionPrime", 
	"BNF_primaryExpression",
	"BNF_variableDeclarationStatement"
};


/*
 ************************************************************
 * Function name: startParser
 * Author: Sam Horner 040935005
 * Description: Initializes the parser statistics array, grabs the
 * first lookahead token from the scanner, and kicks off the syntax
 * analysis by calling the root program() function.
 ***********************************************************
 */
empty startParser() {
	digit i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Function name: matchToken
 * Author: Sam Horner 040935005
 * Description: Verifies that the current lookahead token matches the
 * grammar's expected token code and attribute. If they match, it consumes
 * the token and advances the scanner. If they do not match, it triggers
 * the error handler to report the syntax violation.
 ***********************************************************
 */
empty matchToken(digit tokenCode, digit tokenAttribute) {
	if (DEBUG) {
		printf("matchToken called, expected Token code: %d, expected Attribute: %d\n", tokenCode, tokenAttribute);
		printf("Current lookahead code: %d\n", lookahead.code);
	}
	digit matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
		if(DEBUG)
			printf("Match Token switch, case KW_T, lookahead codeType: %d, tokenAttribute: %d\n", lookahead.attribute.codeType, tokenAttribute);
			
		break;
	case LOG_OP_T:
		if (tokenAttribute != NO_ATTR && (digit)lookahead.attribute.logicalOperator != tokenAttribute) {
			matchFlag = 0;
		}
		break;
	case REL_OP_T:
		if (tokenAttribute != NO_ATTR && (digit)lookahead.attribute.relationalOperator != tokenAttribute) {
			matchFlag = 0;
		}
		break;
	case ART_OP_T:
		if (tokenAttribute != NO_ATTR && (digit)lookahead.attribute.arithmeticOperator != tokenAttribute) {
			matchFlag = 0;
		}
		break;
	default:
		if(DEBUG)
			printf("Match Token switch, default case, lookahead codeType: %d, tokenAttribute: %d\n", lookahead.attribute.codeType, tokenAttribute);
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	/* Successful match and end of file */
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		// Scanner found runtime error in the middle of parsing
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Function name: syncErrorHandler
 * Author: Sam Horner 040935005
 * Description: Implements panic-mode error recovery. When a syntax
 * error occurs, this function continuously requests new tokens from the
 * scanner until it finds a synchronizing token (like End of File),
 * preventing infinite loops and cascading errors.
 ***********************************************************
 */
empty syncErrorHandler(digit syncTokenCode) {
	if(DEBUG)
		printf("syncErrorHandler called, syncTokenCode: %d\n", syncTokenCode);
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Function name: printError
 * Author: Sam Horner 040935005
 * Description: Outputs detailed, human-readable error messages based
 * on the specific token that caused a syntax failure. It prints the
 * line number, token code, and string value to aid in script debugging.
 ***********************************************************
 */
empty printError() {
	if (DEBUG)
		printf("printError called.\n");
	extern digit numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	if (DEBUG) { printToken(t); printf("\n"); }
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
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
	case EOS_T:
		printf("NA\n");
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
	case ASN_T:
		printf("ASN_T\n");
		break;
	case ART_OP_T:
		printf("ART_OP_T\n"); /* You can expand this with the symbols array if you want */
		break;
	case REL_OP_T:
		printf("REL_OP_T\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Function name: program
 * Author: Sam Horner 040935005
 * Description: The root non-terminal function. It loops through the
 * entire source file, routing top-level constructs (like data type
 * declarations, control keywords, and function calls) to their appropriate
 * parsing pathways until the End of File is reached.
 ***********************************************************
 */

empty program() {
	if (DEBUG)
		printf("Program Called.\n");
	psData.parsHistogram[BNF_program]++;

	while (lookahead.code != SEOF_T) {
		switch (lookahead.code) {
		case CMT_T:
			comment();
			break;
		case KW_T:
			/* Check if the keyword is a Data Type */
			if (lookahead.attribute.codeType == KW_digit ||
				lookahead.attribute.codeType == KW_rad ||
				lookahead.attribute.codeType == KW_word ||
				lookahead.attribute.codeType == KW_character ||
				lookahead.attribute.codeType == KW_duple ||
				lookahead.attribute.codeType == KW_byte ||
				lookahead.attribute.codeType == KW_bigdigit ||
				lookahead.attribute.codeType == KW_bigrad ||
				lookahead.attribute.codeType == KW_empty) {

				/* It's a declaration, consume the type and build the definition */
				matchToken(KW_T, lookahead.attribute.codeType);
				topLevelDeclaration();
			}
			else {
				/* It's a control keyword like 'if' or 'while', route to statement() */
				statement();
			}
			break;
		case MNID_T:
		case VID_T:
			/* Route top-level method calls and assignments to statement() */
			statement();
			break;
		default:
			printError();
			lookahead = tokenizer();
			return; /* Exit to prevent infinite loop on bad syntax */
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * Function name: comment
 * Author: Sam Horner 040935005
 * Description: Consumes comment tokens (CMT_T). This allows the
 * parser to gracefully ignore developer comments embedded in the source
 * code without disrupting the grammatical flow.
 ***********************************************************
 */
empty comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/*
 ************************************************************
 * Function name: optParams
 * Author: Sam Horner 040935005
 * Description: Checks if a function signature contains parameters by
 * looking for data type keywords. It routes to paramList() if arguments
 * exist, or takes the epsilon (empty) path if the parentheses are empty.
 ***********************************************************
 */*******************************************************
 */
empty optParams() {
	psData.parsHistogram[BNF_optParams]++;
	/* If the lookahead is a data type keyword, we have a parameter list */
	if (lookahead.code == KW_T && (lookahead.attribute.codeType == KW_digit ||
		lookahead.attribute.codeType == KW_rad ||
		lookahead.attribute.codeType == KW_word)) {
		paramList();
	}
	/* Else, epsilon (empty) is valid */
	printf("%s%s\n", STR_LANGNAME, ": Optional param list parsed");
}

 /*
  ************************************************************
  * Function name: paramList
  * Author: Sam Horner 040935005
  * Description: Parses a sequence of function parameters. It expects
  * a data type followed by an identifier, and uses recursion to handle
  * multiple arguments separated by commas.
  ***********************************************************
  */
empty paramList() {
	/* Match Type */
	matchToken(KW_T, lookahead.attribute.codeType);
	/* Match Variable Name */
	matchToken(VID_T, NO_ATTR);

	/* Handle multiple parameters separated by commas */
	if (lookahead.code == CMA_T) {
		matchToken(CMA_T, NO_ATTR);
		paramList();
	}
	printf("%s%s\n", STR_LANGNAME, ": Param list parsed");
}


/*
 ************************************************************
 * Function name: optVarListDeclarations
 * Author: Sam Horner 040935005
 * Description: An epsilon pathway function designed to handle
 * optional variable declarations within specific data blocks.
 ***********************************************************
 */
empty optVarListDeclarations() {
	psData.parsHistogram[BNF_optVarListDeclarations]++;
	switch (lookahead.code) {
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
 ************************************************************
 * Function name: optionalStatements
 * Author: Sam Horner 040935005
 * Description: Checks for the presence of executable statements
 * inside blocks (like function bodies or loops). It routes valid
 * statement tokens to statements() or takes the epsilon path if empty.
 ***********************************************************
 */
empty optionalStatements() {
	psData.parsHistogram[BNF_optionalStatements]++;

	/* Consume any comments */
	while (lookahead.code == CMT_T) {
		comment();
	}

	switch (lookahead.code) {
	case MNID_T:
	case VID_T:
	case KW_T:
		statements();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Function name: statements
 * Author: Sam Horner 040935005
 * Description: The entry point for parsing a block of code. It
 * processes a single statement and then immediately chains into
 * statementsPrime() to look for subsequent lines of code.
 ***********************************************************
 */
empty statements() {
	psData.parsHistogram[BNF_statements]++;
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Function name: statementsPrime
 * Author: Sam Horner 040935005
 * Description: Recursively parses subsequent statements in a block.
 * It loops through valid starter tokens (identifiers, keywords) to
 * allow consecutive lines of executable code.
 ***********************************************************
 */
empty statementsPrime() {
	psData.parsHistogram[BNF_statementsPrime]++;

	/* Consume any comments */
	while (lookahead.code == CMT_T) {
		comment();
	}

	switch (lookahead.code) {
	case MNID_T:
	case VID_T:
	case KW_T:
		statements();
		break;
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Function name: statement
 * Author: Sam Horner 040935005
 * Description: The main router for execution logic. It analyzes the
 * current token and directs the parser to specific grammatical rules
 * such as assignments, output commands, variables, conditionals, or loops.
 ***********************************************************
 */
empty statement() {
	psData.parsHistogram[BNF_statement]++;
	switch (lookahead.code) {
	case CMT_T:
		comment();
		break;
	case VID_T:
		// Example: myVal = param1;
		assignmentStatement();
		break;

	case MNID_T:
		// Example: print:("Hello World!");
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		else {
			/* Any other MNID_T is a standard function call */
			functionCallStatement();
		}
		break;

	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_return:
			returnStatement();
			break;
		case KW_digit:
		case KW_rad:
		case KW_word:
		case KW_character:
		case KW_duple:
		case KW_byte:
		case KW_bigdigit:
		case KW_bigrad:
			variableDeclarationStatement();
			break;
		case KW_if:
		case KW_else:
			selectionStatement();
			break;
		case KW_while:
		case KW_do:
			iterationStatement();
			break;
		default:
			printError();
		}
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Function name: outputStatement
 * Author: Sam Horner 040935005
 * Description: Parses the specific 'print:' command sequence. It
 * ensures the method name is followed by parentheses, valid output
 * variables, and a terminating semicolon.
 ***********************************************************
 */
empty outputStatement() {
	psData.parsHistogram[BNF_outputStatement]++;
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Function name: outputVariableList
 * Author: Sam Horner 040935005
 * Description: Validates the contents passed into an output
 * statement, ensuring they are valid printable types like string
 * literals or variable identifiers.
 ***********************************************************
 */
empty outputVariableList() {
	psData.parsHistogram[BNF_outputVariableList]++;
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

/*
 ************************************************************
 * Function name: printBNFData
 * Author: Sam Horner 040935005
 * Description: Iterates through the parser's histogram array and
 * prints a statistical summary of how many times each BNF grammar
 * rule was successfully matched and parsed.
 ***********************************************************
 */
empty printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
 ************************************************************
 * Function name: variableDeclarationStatement
 * Author: Sam Horner 040935005
 * Description: Parses local variable declarations. It verifies the
 * data type and identifier, handles optional inline assignments
 * (e.g., variable = value), and enforces the semicolon terminator.
 ***********************************************************
 */
empty variableDeclarationStatement() {
	psData.parsHistogram[BNF_variableDeclarationStatement]++;

	/* Match the Data Type (digit, rad, etc.) */
	matchToken(KW_T, lookahead.attribute.codeType);

	/* Match the Variable Name */
	matchToken(VID_T, NO_ATTR);

	// Determines if this is declaration or assignment
	switch (lookahead.code) {
	case ASN_T:  /* If it is '=', this is assignment */
		matchToken(ASN_T, NO_ATTR);
		arithmeticExpression();
		matchToken(EOS_T, NO_ATTR); /* Catch the ';' at the end of the math */
		break;

	case EOS_T:  /* End of a statement ; */
		matchToken(EOS_T, NO_ATTR);
		break;

	default:
		printError(); /* It wasn't = or ;, so it's a syntax error */
	}

	printf("%s%s\n", STR_LANGNAME, ": Variable declaration parsed");
}

/*
 ************************************************************
 * Function name: topLevelDeclaration
 * Author: Sam Horner 040935005
 * Description: Handles definitions at the global scope. It uses
 * lookahead to distinguish between function definitions (expecting
 * parentheses and braces), function prototypes (expecting a semicolon),
 * and global variables.
 ***********************************************************
 */
empty topLevelDeclaration() {
	if (lookahead.code == MNID_T) {
		matchToken(MNID_T, NO_ATTR);
		matchToken(LPR_T, NO_ATTR);
		optParams();
		matchToken(RPR_T, NO_ATTR);

		/* Check for function body { ... } OR declaration ; */
		if (lookahead.code == LBR_T) {
			matchToken(LBR_T, NO_ATTR);
			optionalStatements();
			matchToken(RBR_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Function definition parsed");
		}
		else if (lookahead.code == EOS_T) {
			matchToken(EOS_T, NO_ATTR);
			printf("%s%s\n", STR_LANGNAME, ": Function declaration parsed");
		}
		else {
			printError();
		}
	}
	else if (lookahead.code == VID_T) {
		/* It's a Global Variable Definition */
		matchToken(VID_T, NO_ATTR);

		/* Check for optional assignment = */
		if (lookahead.code == ASN_T) {
			matchToken(ASN_T, NO_ATTR);
			arithmeticExpression();
		}

		matchToken(EOS_T, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Global variable parsed");
	}
	else {
		printError();
	}
}

/*
 ************************************************************
 * Function name: returnStatement
 * Author: Sam Horner 040935005
 * Description: Parses the 'return' keyword followed by an
 * arithmetic expression, ensuring that functions can correctly pass
 * values back to the caller.
 ***********************************************************
 */
empty returnStatement() {
	psData.parsHistogram[BNF_returnStatement]++;
	matchToken(KW_T, KW_return);
	arithmeticExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}

/*
 ************************************************************
 * Function name: functionCallStatement
 * Author: Sam Horner 040935005
 * Description: Parses standalone function executions. It verifies
 * that a method identifier is followed by an argument list enclosed
 * in parentheses and terminated by a semicolon.
 ***********************************************************
 */
empty functionCallStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	argList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function call parsed");
}

/*
 ************************************************************
 * Function name: argList
 * Author: Sam Horner 040935005
 * Description: Recursively parses a comma-separated list of
 * arguments (variables, numbers, or strings) being passed into a
 * function call.
 ***********************************************************
 */
empty argList() {
	/* If the lookahead is an identifier, number, or string, we have arguments */
	if (lookahead.code == VID_T || lookahead.code == INL_T || lookahead.code == STR_T) {
		arithmeticExpression(); /* Parses the '5' or 'param1' */

		/* If there's a comma, match it and recursively check for more arguments */
		if (lookahead.code == CMA_T) {
			matchToken(CMA_T, NO_ATTR);
			argList();
		}
	}
}

/*
 ************************************************************
 * Function name: assignmentStatement
 * Author: Sam Horner 040935005
 * Description: Handles the reassignment of existing variables.
 * It matches a variable identifier, the equals sign operator, and
 * the mathematical or literal expression being assigned to it.
 ***********************************************************
 */
empty assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	matchToken(VID_T, NO_ATTR);
	matchToken(ASN_T, NO_ATTR);
	arithmeticExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

/*
 ************************************************************
 * Function name: arithmeticExpression
 * Author: Sam Horner 040935005
 * Description: The starting node for mathematical equations. It
 * calls primaryExpression for the left side of the operation and
 * then arithmeticExpressionPrime to build the right side.
 ***********************************************************
 */
empty arithmeticExpression() {
	psData.parsHistogram[BNF_arithmeticExpression]++;
	primaryExpression();
	arithmeticExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

/*
 ************************************************************
 * Function name: arithmeticExpressionPrime
 * Author: Sam Horner 040935005
 * Description: Handles the recursive right-hand side of arithmetic
 * operations, checking for math operators (+, -, *, /) and chaining
 * further expressions together.
 ***********************************************************
 */
empty arithmeticExpressionPrime() {
	psData.parsHistogram[BNF_arithmeticExpressionPrime]++;
	switch (lookahead.code) {
	case ART_OP_T:
		matchToken(ART_OP_T, NO_ATTR);
		primaryExpression();
		arithmeticExpressionPrime();
		break;
	default:
		; // empty string (epsilon)
	}
}

/*
 ************************************************************
 * Function name: primaryExpression
 * Author: Sam Horner 040935005
 * Description: The smallest foundational unit of an expression.
 * It validates terminal leaf values like variable identifiers,
 * integer literals, floating-point literals, and strings.
 ***********************************************************
 */
empty primaryExpression() {
	psData.parsHistogram[BNF_primaryExpression]++;
	switch (lookahead.code) {
	case VID_T:
		matchToken(VID_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	case STR_T: 
		matchToken(STR_T, NO_ATTR);
		break;
	case FPL_T: 
		matchToken(FPL_T, NO_ATTR);
		break;
	default:
		printError();
	}
}

/*
 ************************************************************
 * Function name: selectionStatement
 * Author: Sam Horner 040935005
 * Description: Parses 'if' and 'else' conditional blocks. It
 * ensures the conditional expression is wrapped in parentheses and
 * processes the resulting executable statements inside the braces.
 ***********************************************************
 */
empty selectionStatement() {
	/* Match: if */
	matchToken(KW_T, KW_if);

	/* Match: ( condition ) */
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);

	/* Match: { statements } */
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	/* OPTIONAL: else { statements } */
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_else) {
		matchToken(KW_T, KW_else);
		matchToken(LBR_T, NO_ATTR);
		optionalStatements();
		matchToken(RBR_T, NO_ATTR);
	}

	printf("%s%s\n", STR_LANGNAME, ": Selection statement parsed");
}

/*
 ************************************************************
 * Function name: conditionalExpression
 * Author: Sam Horner 040935005
 * Description: Evaluates the logic inside control structures.
 * It parses an arithmetic expression, followed optionally by a
 * relational operator (==, <, >, !=) and a second expression to compare.
 ***********************************************************
 */
empty conditionalExpression() {
	/* Match the left side (e.g., 'param1') */
	arithmeticExpression();

	/* If there is a relational operator (==, !=, <, >, etc.), match it and the right side */
	if (lookahead.code == REL_OP_T) {
		matchToken(REL_OP_T, NO_ATTR);
		arithmeticExpression(); /* e.g., '10' */
	}

	printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");
}

/*
 ************************************************************
 * Function name: iterationStatement
 * Author: Sam Horner 040935005
 * Description: Parses both 'while' and 'do-while' loops. It ensures
 * the conditional expressions and looped executable bodies are
 * properly formatted and grammatically correct.
 ***********************************************************
 */
empty iterationStatement() {
	/* Pathway 1: Standard 'while' loop */
	if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_while) {
		matchToken(KW_T, KW_while);

		matchToken(LPR_T, NO_ATTR);
		conditionalExpression();
		matchToken(RPR_T, NO_ATTR);

		matchToken(LBR_T, NO_ATTR);
		optionalStatements();
		matchToken(RBR_T, NO_ATTR);
	}
	/* Pathway 2: 'do-while' loop */
	else if (lookahead.code == KW_T && lookahead.attribute.codeType == KW_do) {
		matchToken(KW_T, KW_do);

		matchToken(LBR_T, NO_ATTR);
		optionalStatements();
		matchToken(RBR_T, NO_ATTR);

		matchToken(KW_T, KW_while);
		matchToken(LPR_T, NO_ATTR);
		conditionalExpression();
		matchToken(RPR_T, NO_ATTR);

		matchToken(EOS_T, NO_ATTR); /* do-while loops end with a semicolon */
	}
	else {
		printError();
	}

	printf("%s%s\n", STR_LANGNAME, ": Iteration statement parsed");
}
