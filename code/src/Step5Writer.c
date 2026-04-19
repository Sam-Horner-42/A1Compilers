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
***********************************************************
* File name: Writer.c
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

#ifndef CODER_H_
#include "../includes/Step5Writer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Global variables */
Variable variables[MAX_VARS];
digit var_count = 0;
digit initial_phase = 1; // Flag to track the initial phase
character output_buffer[MAX_EXPR_LEN * 10] = { 0 }; // Buffer to store write output
digit execute_block = 1; // 1 means execute, 0 means skip lines
digit skip_next_else = 0; // Flag to check if the current conditional evaluates to true to skip the rest

/*
 ************************************************************
 * Function name: find_variable
 * Author: Sam Horner 040935005
 * Description: Searches the global variables array to locate an
 * existing variable by its identifier name. Returns the array
 * index if a match is found, or -1 if the variable does not exist.
 ***********************************************************
 */
digit find_variable(const word name) {
    digit i = 0;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Assign numeric variable */
empty assign_numeric_variable(const word name, bigrad value) {
    digit idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
		// Defensive programming: Prevent array out-of-bounds crash
		if (var_count >= MAX_VARS) {
			printf("Error: Maximum number of variables (%d) exceeded!\n", MAX_VARS);
			return;
		}
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = value;
}

/* Assign string variable */
empty assign_string_variable(const word name, const word value) {
    digit idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = STRING;
    strncpy_s(variables[idx].value.str_value, sizeof(variables[idx].value.str_value), value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
}

/* Assign boolean variable */
empty assign_boolean_variable(const word name, digit value) {
    digit idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = BOOLEAN;
    variables[idx].value.bool_value = value;
}

/* Get boolean variable */
digit get_boolean_value(const word name) {
    digit idx = find_variable(name);
    if (idx != -1 && variables[idx].type == BOOLEAN) {
        return variables[idx].value.bool_value;
    }
    return 0; // Default to false if variable not found
}

/* Assign char variable */
empty assign_char_variable(const word name, character value) {
    digit idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = CHAR;
    variables[idx].value.char_value = value;
}

/* Gets char variable */
character get_char_value(const word name) {
    digit idx = find_variable(name);
    if (idx != -1 && variables[idx].type == CHAR) {
        return variables[idx].value.char_value;
    }
    return EOS; // Default to null character if variable not found
}

/* Gets numerical value */
bigrad get_numeric_value(const word name) {
	digit idx = find_variable(name);
    if (idx != -1 && variables[idx].type == NUMERIC) {
        return variables[idx].value.num_value;
    }
    return ZERO;
}

/* Gets string variable */
word get_string_value(const word name) {
    digit idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) {
        return variables[idx].value.str_value;
    }
    return STREMPTY;
}

/*
 ************************************************************
 * Function name: parse_term
 * Author: Sam Horner 040935005
 * Description: Processes terms in an arithmetic expression,
 * handling multiplication, division, parentheses, and variable
 * lookups. It recursively calls parse_expression for nested
 * groupings and ensures proper order of operations for high-
 * precedence mathematical operators.
 ***********************************************************
 */
bigrad parse_term(word* expr) {
    bigrad value = 0.0;
    while (isspace(**expr)) (*expr)++;
    if (**expr == LPAR) {
        (*expr)++;
        value = parse_expression(expr);
        if (**expr == RPAR) {
            (*expr)++;
        }
    }
    else if (isalpha(**expr)) {
        character var_name[32] = { 0 };
        digit i = 0;
		while (isalnum(**expr) || **expr == '_') {
			var_name[i++] = *(*expr)++;
		}
		var_name[i] = EOS; // Finishes with alpha and number
       
        value = get_numeric_value(var_name);
    }
    else {
        value = strtod(*expr, expr);
    }
    while (isspace(**expr)) (*expr)++;
    while (**expr == MULT || **expr == DIV) {
        character op = *(*expr)++;
        bigrad next_value = parse_term(expr);
        if (op == MULT) {
            value *= next_value;
        }
        else {
            if (next_value != 0)
                value /= next_value;
            else {
                printf("Error: division by zero! ");
                value = 0;
            }
        }
    }
    return value;
}

/*
 ************************************************************
 * Function name: parse_expression
 * Author: Sam Horner 040935005
 * Description: Evaluates additive arithmetic expressions by
 * processing terms separated by addition or subtraction
 * operators. It maintains left-to-right associativity and
 * relies on parse_term to handle higher-precedence operations,
 * returning the final calculated bigrad value.
 ***********************************************************
 */
bigrad parse_expression(word* expr) {
    bigrad value = parse_term(expr);
    while (isspace(**expr)) (*expr)++;
    while (**expr == PLUS || **expr == MINUS) {
        character op = *(*expr)++;
        bigrad next_value = parse_term(expr);
        if (op == PLUS) {
            value += next_value;
        }
        else {
            value -= next_value;
        }
    }
    return value;
}

/*
 ************************************************************
 * Function name: handle_write
 * Author: Sam Horner 040935005
 * Description: Processes output statements by parsing the
 * contents of parentheses. It extracts string literals and
 * retrieves variable values (numeric, string, boolean, or char)
 * from the global table, concatenating them into a buffer
 * for either immediate display or storage in the global
 * output buffer.
 ***********************************************************
 */
empty handle_write(word expression) {
    character buffer[MAX_EXPR_LEN] = { 0 };
    word start = strchr(expression, LPAR) + 1;
    word end = strrchr(expression, RPAR);
    if (start != NULL && end != NULL && start < end) {
        *end = EOS;
        while (*start != EOS) {
            if (*start == QUOTES) {
                start++;
                while (*start != QUOTES && *start != EOS) {
                    strncat_s(buffer, sizeof(buffer), start, 1);
                    start++;
                }
                start++;
            }
            else if (isalpha(*start)) {
				character var_name[32] = { 0 };
                digit i = 0;
                while (isalnum(*start)) {
                    var_name[i++] = *start++;
                } // Finishes with alpha and number
                var_name[i++] = *start++; // Includes the suffix
                digit var_idx = find_variable(var_name);
                if (var_idx != -1) {
                    if (variables[var_idx].type == STRING) {
                        strcat_s(buffer, sizeof(buffer), get_string_value(var_name));
                    }
                    else if (variables[var_idx].type == NUMERIC) {
                        character num_str[256];
                        snprintf(num_str, sizeof(num_str), "%.2lf", get_numeric_value(var_name));
                        strcat_s(buffer, sizeof(buffer), num_str);
                    }
                    else if (variables[var_idx].type == BOOLEAN) {
						strcat_s(buffer, sizeof(buffer), get_boolean_value(var_name) ? "true" : "false");
                    }
                    else if (variables[var_idx].type == CHAR) {
                        character char_str[2] = { get_char_value(var_name), EOS };
                        strcat_s(buffer, sizeof(buffer), char_str);
                    }
                }
            }
            else if (isspace(*start)) {
                strncat_s(buffer, sizeof(buffer), start, 1);
                start++;
            }
            else {
                start++;
            }
        }
    }
    if (initial_phase) {
		if (strlen(output_buffer) + strlen(buffer) + 2 >= sizeof(output_buffer)) {
			printf("Error: Output buffer is full. Cannot print any more text.\n");
			return;
		}
        strcat_s(output_buffer, sizeof(output_buffer), buffer);
        character replacement[] = "[Undefined]";
        if (strlen(buffer) == 0) {
            strcpy(buffer, replacement);
            strcat_s(output_buffer, sizeof(output_buffer), buffer);
        }
        strcat_s(output_buffer, sizeof(output_buffer), "\n");
    }
    else {
        printf("%s\n", buffer);
    }
}

/*
 ************************************************************
 * Function name: evaluate_value
 * Author: Sam Horner 040935005
 * Description: Converts a string operand into a numeric value
 * for logical or mathematical evaluation. It handles boolean
 * literals (TRUE/FALSE), performs lookups for variable
 * identifiers to retrieve their stored values, or converts
 * standard numeric strings into bigrad format.
 ***********************************************************
 */
bigrad evaluate_value(character* operand_string) {
	
	if (operand_string == NULL || strlen(operand_string) == 0) {
		return 0.0;
	}
	// Check if the string is a literal TRUE or FALSE
	if (strcmp(operand_string, "TRUE") == 0) return 1.0;
	if (strcmp(operand_string, "FALSE") == 0) return 0.0;

	// Check if the string starts with a letter (meaning it's a variable name like 'myTrueBool')
	if (isalpha(operand_string[0])) {
		digit idx = find_variable(operand_string);
		if (idx != -1) {
			if (variables[idx].type == BOOLEAN) return variables[idx].value.bool_value;
			if (variables[idx].type == NUMERIC) return variables[idx].value.num_value;
		}
		return 0.0; // Default if variable not found
	}

	// Otherwise, it must be a standard number string (like "42.5")
	return strtod(operand_string, NULL);
}

/* Handles <, >, <=, >=, != */
/* Handle if comparisons */
empty handle_comparison(word expr) {
	character var1[32] = { 0 }, op[3] = { 0 }, var2[32] = { 0 };
	word start = strchr(expr, LPAR);
	word end = strchr(expr, RPAR);

	if (start && end) {
		start++; // Move past '('
		character condition[128] = { 0 };
		strncpy_s(condition, sizeof(condition), start, end - start);

		// Extract Left Side, Operator, and Right Side
		if (sscanf_s(condition, "%31s %2s %31s", var1, (unsigned)_countof(var1), op, (unsigned)_countof(op), var2, (unsigned)_countof(var2)) == 3) {

			// Convert the extracted strings into values we can do math on
			bigrad val1 = evaluate_value(var1);
			bigrad val2 = evaluate_value(var2);

			// Evaluate the condition
			if (strcmp(op, "==") == 0) execute_block = (val1 == val2);
			else if (strcmp(op, "!=") == 0) execute_block = (val1 != val2);
			else if (strcmp(op, "<=") == 0) execute_block = (val1 <= val2);
			else if (strcmp(op, ">=") == 0) execute_block = (val1 >= val2);
			else if (strcmp(op, "<") == 0) execute_block = (val1 < val2);
			else if (strcmp(op, ">") == 0) execute_block = (val1 > val2);
			else execute_block = 0; // Default to false if operator is unknown
		}
	}
}

/*
 ************************************************************
 * Function name: calculate
 * Author: Sam Horner 040935005
 * Description: The core interpreter engine that processes
 * individual lines of code. It manages control flow logic
 * (if/else if/else), handles variable assignments for multiple
 * data types, routes output to the writer handler, and
 * evaluates standalone arithmetic expressions. It uses
 * state flags to track nested block execution and skip
 * logic as necessary.
 ***********************************************************
 */
empty calculate(word expression) {
	character var_name[32] = { 0 };

	// Strip leading whitespace to make parsing easier
	word expr = expression;
	while (isspace(*expr)) expr++;

	// Handle the end of a block '}', which might be attached to an 'else'
	if (*expr == '}') {
		execute_block = 1;             // Reset block execution for the next lines
		expr++;                        // Move past the '}'
		while (isspace(*expr)) expr++; // Skip any spaces after '}'

		// If the line was ONLY a '}', we are done with this line
		if (*expr == EOS || *expr == '\0') {
			return;
		}
	}

	// If we are inside a failed block, skip executing this line entirely
	if (!execute_block) {
		return;
	}

	// Handle 'else if'
	if (strncmp(expr, "else if", 7) == 0) {
		if (skip_next_else) {
			execute_block = 0; // The previous 'if' was true, so we skip this block
		}
		else {
			handle_comparison(expr); // Evaluate this new condition!
			skip_next_else = execute_block; // If this evaluates to true, skip following elses
		}
		return;
	}

	// Handle 'else'
	else if (strncmp(expr, "else", 4) == 0) {
		if (skip_next_else) {
			execute_block = 0; // The previous 'if' was true, so skip the else
		}
		else {
			execute_block = 1; // The previous 'if' was false, so execute the else
		}
		skip_next_else = 0; // Reset the memory flag
		return;
	}

	// Handle standard 'if'
	else if (strncmp(expr, "if", 2) == 0) {
		handle_comparison(expr);
		skip_next_else = execute_block; // Remember if this was true or false for the next 'else'
		return;
	}

	// Existing Assignment Logic (Modified to ignore '==' and handle TRUE/FALSE)
	word equals_ptr = strchr(expr, EQUALS);

	// Make sure we found an '=', AND make sure the next character isn't also an '='
	if (equals_ptr && *(equals_ptr + 1) != EQUALS) {
		character lhs[128] = { 0 };
		strncpy_s(lhs, sizeof(lhs), expr, equals_ptr - expr);

		character token1[32] = { 0 };
		character token2[32] = { 0 };
		digit tokens_read = sscanf_s(lhs, "%31s %31s", token1, (unsigned)_countof(token1), token2, (unsigned)_countof(token2));

		if (tokens_read == 2) {
			strcpy_s(var_name, sizeof(var_name), token2);
		}
		else if (tokens_read == 1) {
			strcpy_s(var_name, sizeof(var_name), token1);
		}

		word rhs_expr = equals_ptr + 1;
		while (isspace(*rhs_expr)) rhs_expr++;

		if (*rhs_expr == QUOTES) {
			rhs_expr++;
			character str_value[256] = { 0 };
			digit i = 0;
			while (*rhs_expr != QUOTES && *rhs_expr != EOS) {
				str_value[i++] = *rhs_expr++;
			}
			assign_string_variable(var_name, str_value);
			if (!initial_phase) {
				printf("%s = \"%s\"\n", var_name, str_value);
			}
		}
		else if (strncmp(rhs_expr, "TRUE", 4) == 0 || strncmp(rhs_expr, "FALSE", 5) == 0) {
			digit bool_value = strncmp(rhs_expr, "TRUE", 4) == 0 ? 1 : 0;
			assign_boolean_variable(var_name, bool_value);
			if (!initial_phase) {
				printf("%s = %s\n", var_name, bool_value ? "TRUE" : "FALSE");
			}
		}
		else if (*rhs_expr == QUOTE) {
			rhs_expr++;
			character char_value = *rhs_expr;
			assign_char_variable(var_name, char_value);
			if (!initial_phase) {
				printf("%s = '%c'\n", var_name, char_value);
			}
		}
		else {
			bigrad value = parse_expression(&rhs_expr);
			assign_numeric_variable(var_name, value);
			if (!initial_phase) {
				printf("%s = %.2lf\n", var_name, value);
			}
		}
	}
	else if (strstr(expr, WRITE)) {
		handle_write(expr);
	}
	else {
		word rhs_expr = expr;
		bigrad result = parse_expression(&rhs_expr);
		if (!initial_phase) {
			printf("Result: %.2lf\n", result);
		}
	}
}

/*
 ************************************************************
 * Function name: process_file
 * Author: Sam Horner 040935005
 * Description: Reads a source file line by line and passes
 * each valid line to the calculate function for interpretation.
 * After the file is fully processed, it transitions out of
 * the initial phase, flushes the output buffer to the console,
 * and prints a final report of all variable names and their
 * final stored values across all supported types.
 ***********************************************************
 */
empty process_file(const word filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    character line[MAX_EXPR_LEN];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == NEWLINE || line[0] == RETURN) {
            continue; // Skip empty lines
        }
        line[strcspn(line, "\n")] = 0; // Remove newline character
        calculate(line);
    }
    fclose(file);
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
	digit i = 0;
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
}

/* Split lines into components */
word* splitIntoLines(const word content, digit* lineCount) {
    word* lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) {
        perror("Error allocating memory");
        return NULL;
    }
    *lineCount = 0;
    const character* start = content;
    const character* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines\n");
            break;
        }
        digit lineLength = (digit) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        // Use strncpy_s for safer copying
        if (strncpy_s(lines[*lineCount], lineLength + 1, start, lineLength) != 0) {
            perror("Error copying string using strncpy_s");
            free(lines[*lineCount]);
            break;
        }
        lines[*lineCount][lineLength] = EOS; // Null-terminate the string
        (*lineCount)++;
        start = end + 1;
    }
    // Handle the last line if it doesn't end with a newline
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
        }
        else {
            // Use _strdup to copy the remaining content
            lines[*lineCount] = _strdup(start);
            if (!lines[*lineCount]) {
                perror("Error copying string using _strdup");
            }
            else {
                (*lineCount)++;
            }
        }
    }
    return lines;
}

/* Free lines */
empty freeLines(word* lines, digit lineCount) {
    digit i = 0;
    for (i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}

/*
 ************************************************************
 * Function name: process_content
 * Author: Sam Horner 040935005
 * Description: Interprets the program logic from a raw string
 * of file content. It splits the content into individual
 * lines, iterates through them for calculation, and manages
 * memory by freeing the line array after execution. Finally,
 * it outputs the results of all write statements and a
 * comprehensive table of variable states.
 ***********************************************************
 */
empty process_content(word fileContent) {
    digit lineCount = 0;
    word* lines = splitIntoLines(fileContent, &lineCount);

    word line = malloc(MAX_EXPR_LEN);
    if (!lines || !line) {
        return;
    }
    printf("Lines from content:\n");
    digit i = 0;
	for (i = 0; i < lineCount; i++) {
		line = lines[i];
		calculate(line);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }
	freeLines(lines, lineCount); // free the lines to avoid memory leaks
}
