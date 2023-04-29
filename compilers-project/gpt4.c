#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Token definitions
#define PROGRAM "program"
#define INPUT "input"
#define OUTPUT "output"
#define CONST "const"
#define INTEGER "integer"
#define REAL "real"
#define CHAR "char"
#define BOOLEAN "boolean"
#define BEGIN "begin"
#define END "end"
#define IF "if"
#define THEN "then"
#define REPEAT "repeat"
#define UNTIL "until"
#define WRITELN "writeln"
#define NOT "not"

// Function prototypes
void program();
void header();
void declarations();
void const_declarations();
void const_def();
void var_declarations();
void var_decl();
void id_list();
void id_list_prime();
void type();
void block();
void statements();
void statements_prime();
void statement();
void if_statement();
void repeat_statement();
void writeln_statement();
void expression();
void expression_prime();
void simple_expression();
void simple_expression_prime();
void term();
void term_prime();
void factor();

// Helper functions
bool match(const char *expected);
char* get_next_token();
void syntax_error(const char *message);

// Global variable to hold the current token
char *current_token = NULL;
FILE *in_file, *out_file;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Expected 2 arguments, recieved %d.", argc);
        exit(1);
    }
    // Begin parsing
    in_file = freopen(argv[1], "r", stdin);
    out_file = freopen(argv[2], "w", stdout);
    current_token = get_next_token();
    program();

    // Check for successful parsing
    if (match(".")) {
        printf("Parsing successful\n");
    } else {
        syntax_error("Expected '.' at the end of the program");
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}

void program() {
    header();
    declarations();
    block();
}

void header() {
    if (match(PROGRAM)) {
        current_token = get_next_token();
        if (current_token == NULL) {
            syntax_error("Expected identifier after 'program'");
        }
        current_token = get_next_token();
        if (match("(")) {
            if (match(INPUT)) {
                if (match(",")) {
                    if (match(OUTPUT)) {
                        if (match(")")) {
                            if (match(";")) {
                                current_token = get_next_token();
                            } else {
                                syntax_error("Expected ';' after header");
                            }
                        } else {
                            syntax_error("Expected ')' in header");
                        }
                    } else {
                        syntax_error("Expected 'output' in header");
                    }
                } else {
                    syntax_error("Expected ',' in header");
                }
            } else {
                syntax_error("Expected 'input' in header");
            }
        } else {
            syntax_error("Expected '(' in header");
        }
    } else {
        syntax_error("Expected 'program' keyword");
    }
}

void declarations() {
    if (match(CONST)) {
        const_declarations();
    }

    var_declarations();
}

void const_declarations() {
    if (match(CONST)) {
        current_token = get_next_token();
        const_def();

        if (match(";")) {
            current_token = get_next_token();
            const_declarations();
        } else {
            syntax_error("Expected ';' after const definition");
        }
    }
}

void const_def() {
    if (current_token != NULL) {
        current_token = get_next_token();
        if (match("=")) {
            current_token = get_next_token();
            if (current_token == NULL) {
                syntax_error("Expected number after '=' in const definition");
            }
            current_token = get_next_token();
        } else {
            syntax_error("Expected '=' in const definition");
        }
    } else {
        syntax_error("Expected identifier in const definition");
    }
}

void var_declarations() {
    var_decl();

    if (match(";")) {
        current_token = get_next_token();
        var_declarations();
    }
}

void var_decl() {
    id_list();

    if (match(":")) {
        current_token = get_next_token();
        type();
    } else {
        syntax_error("Expected ':' in variable declaration");
    }
}

void id_list() {
    if (current_token != NULL) {
        current_token = get_next_token();
        id_list_prime();
    } else {
        syntax_error("Expected identifier in id list");
    }
}

void id_list_prime() {
    if (match(",")) {
        current_token = get_next_token();
        id_list();
    }
}

void type() {
    if (match(INTEGER) || match(REAL) || match(CHAR) || match(BOOLEAN)) {
        current_token = get_next_token();
    } else {
        syntax_error("Expected a valid type in variable declaration");
    }
}

void block() {
    if (match(BEGIN)) {
        current_token = get_next_token();
        statements();
        if (match(END)) {
            current_token = get_next_token();
        } else {
            syntax_error("Expected 'end' keyword in block");
        }
    } else {
        syntax_error("Expected 'begin' keyword in block");
    }
}

void statements() {
    statement();
    statements_prime();
}

void statements_prime() {
    if (match(";")) {
        current_token = get_next_token();
        statement();
        statements_prime();
    }
}

void statement() {
    if (current_token != NULL) {
        if (match(":=")) {
            current_token = get_next_token();
            expression();
        } else if (match(BEGIN)) {
            block();
        } else if (match(IF)) {
            if_statement();
        } else if (match(REPEAT)) {
            repeat_statement();
        } else if (match(WRITELN)) {
            writeln_statement();
        } else {
            syntax_error("Invalid statement");
        }
    } else {
        syntax_error("Expected identifier or keyword in statement");
    }
}

void if_statement() {
    current_token = get_next_token();
    expression();
    if (match(THEN)) {
        current_token = get_next_token();
        statement();
    } else {
        syntax_error("Expected 'then' in if statement");
    }
}

void repeat_statement() {
    current_token = get_next_token();
    statement();
    if (match(UNTIL)) {
        current_token = get_next_token();
        expression();
    } else {
        syntax_error("Expected 'until' in repeat statement");
    }
}

void writeln_statement() {
    if (match("(")) {
        current_token = get_next_token();
        simple_expression();
        if (match(")")) {
            current_token = get_next_token();
        } else {
            syntax_error("Expected ')' in writeln statement");
        }
    } else {
        syntax_error("Expected '(' in writeln statement");
    }
}

void expression() {
    simple_expression();
    expression_prime();
}

void expression_prime() {
    if (current_token != NULL) {
        current_token = get_next_token();
        simple_expression();
    }
}

void simple_expression() {
    term();
    simple_expression_prime();
}

void simple_expression_prime() {
    if (current_token != NULL) {
        current_token = get_next_token();
        term();
        simple_expression_prime();
    }
}

void term() {
    factor();
    term_prime();
}

void term_prime() {
    if (current_token != NULL) {
        current_token = get_next_token();
        factor();
        term_prime();
    }
}

void factor() {
    if (current_token != NULL) {
        current_token = get_next_token();
    } else if (match("(")) {
        current_token = get_next_token();
        expression();
        if (match(")")) {
            current_token = get_next_token();
        } else {
            syntax_error("Expected ')' in factor");
        }
    } else if (match(NOT)) {
        current_token = get_next_token();
        factor();
    } else {
        syntax_error("Expected identifier, number, '(', or 'not' in factor");
    }
}

bool match(const char *expected) {
    return current_token != NULL && strcmp(current_token, expected) == 0;
}

char* get_next_token() {
    // This function should be implemented to read the next token from the input.
    // For the sake of this example, it is left unimplemented.
    return NULL;
}

void syntax_error(const char *message) {
    fprintf(stderr, "Syntax error: %s\n", message);
    exit(1);
}