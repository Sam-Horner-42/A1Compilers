/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Sam Horner 040935005
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
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
# ECHO "[WRITER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Writer.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "../includes/Compilers.h"
#endif

#ifndef READER_H_
#include "../includes/Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "../includes/Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "../includes/Step4Parser.h"
#endif

#ifndef WRITER_H_
#define WRITER_H_

#define MAX_VARS 100
#define MAX_EXPR_LEN 256
#define MAX_LINES 100

#define WRITE "print:"

#define EOS '\0'
#define ZERO 0.0
#define STREMPTY ""
#define LPAR '('
#define RPAR ')'
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'

#define EQUALS '='
#define MOD '%'
#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define RETURN '\r'
#define QUOTES '"'
#define QUOTE '\''

typedef enum { NUMERIC, STRING, BOOLEAN, CHAR } VarType;

typedef struct {
    character name[32];
    VarType type;
    union {
        bigrad num_value;
        character str_value[256];
        duple bool_value; // For BOOLEAN type (1 for true, 0 for false)
        character char_value; // For CHAR type
    } value;
} Variable;

digit find_variable(const word name);

empty assign_boolean_variable(const word name, digit value);
empty assign_char_variable(const word name, character value);
empty assign_numeric_variable(const word name, bigrad value);
empty assign_string_variable(const word name, const word value);

digit get_boolean_value(const word name);
character get_char_value(const word name);
bigrad get_numeric_value(const word name);
const word get_string_value(const word name);

bigrad parse_term(word* expr);
bigrad parse_expression(word* expr);
empty calculate(word expression);

empty handle_write(word expression);
empty handle_comparison(word expr);
empty process_file(const word filename);
empty process_content(word content);

empty executeFunctionCall(word callLine);
empty storeFunctionDefinition(word* lines, digit* i, digit lineCount);

#endif
