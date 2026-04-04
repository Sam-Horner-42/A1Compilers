/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
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

/* TO_DO: Adjust the function header */

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
/* TO_DO: This is the function to start the parser - check your program definition */

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



empty startParser() {
	/* TO_DO: Initialize Parser data */
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
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
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
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
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
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
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
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
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
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
empty comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}


/*
 ************************************************************
 * optParams
 * BNF: <optParams> -> <paramList> | e
 * FIRST(<optParams>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
 ***********************************************************
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
 * paramList
 * BNF: <paramList> -> <opt_varlist_declarations>
 * FIRST(<paramList>) = { KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
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
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KW_T (KW_int), KW_T (KW_real), KW_T (KW_string)}.
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
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
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
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
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
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
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
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
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
		/* TO_DO: Add your input statement check here later */
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
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
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
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
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
 * The function prints statistics of BNF rules
 * Param:
 *	- Parser data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
/*
sofia_void printBNFData(ParserData psData) {
}
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

empty returnStatement() {
	psData.parsHistogram[BNF_returnStatement]++;
	matchToken(KW_T, KW_return);
	arithmeticExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Return statement parsed");
}

empty functionCallStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	argList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Function call parsed");
}

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

empty assignmentStatement() {
	psData.parsHistogram[BNF_assignmentStatement]++;
	matchToken(VID_T, NO_ATTR);
	matchToken(ASN_T, NO_ATTR);
	arithmeticExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}

empty arithmeticExpression() {
	psData.parsHistogram[BNF_arithmeticExpression]++;
	primaryExpression();
	arithmeticExpressionPrime();
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

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
