#include "global.h"
#include "scanner.c"
#include "code-gen.c"

#define RULE(func)                     \
    do                                 \
    {                                  \
        if (func == 1)                 \
        {                              \
            printf("In: %s\n", #func); \
            return 1;                  \
        }                              \
    } while (0)

int match(int);
void Program(),
    Header(),
    Declarations(),
    ConstantDefinitions(),
    ConstantDefinition(),
    VariableDeclarations(),
    VariableDeclaration(),
    IdentifierList(),
    Type(),
    Block(),
    Statements(),
    Statement(),
    ExprList(),
    Expr(),
    SimpleExpr(),
    Term(),
    Factor();

void parse() /*  parses and translates expression list  */
{
    lookahead = lexan();
    Program();
    return 0;
}

void Program()
{
    /* Just one production for start, so we don't need to check lookahead */
    Header();
    Declarations();
    Block();
}

void Type()
{
    strcpy(current_rule, "type");
    switch (lookahead)
    {
    case REAL:
        match(REAL);
        break;

    case BOOLEAN:
        match(BOOLEAN);
        break;

    case INTEGER:
        match(INTEGER);
        break;

    case CHAR:
        match(CHAR);
        break;

    default:
        mismatch_error(404);
        break;
    }
}

void IdentifierList()
{
    switch (lookahead)
    {
    case ',':
        match(',');
        match(ID);
        IdentifierList();
        break;

    case ':':
        match(':');
        Type();
        break;

    default:
        mismatch_error(404);
        break;
    }
}

void Header()
{
    match(PROGRAM);
    match(ID);
    match('(');
    match(INPUT); // INPUT
    match(',');
    match(OUTPUT); // OUTPUT
    match(')');
    match(';');
}
void Block()
{
    strcpy(current_rule, "block");
    match(BEGIN);
    // Statements(); // THIS SEGFAULTS
    match(END);
}

void VariableDeclarations()
{
    strcpy(current_rule, "variable declaration");
    if (lookahead == BEGIN)
    {
        // epsilon
    }
    else
    {
        VariableDeclaration();
        VariableDeclarations();
    }
}

void VariableDeclaration()
{
    strcpy(current_rule, "variable declaration");
    match(ID);
    strcpy(current_rule, "identifier list");
    IdentifierList();
    strcpy(current_rule, "variable declaration");
    match(';');
}

void SimpleExpr()
{
    Term();
    if (lookahead == '+')
    {
        match('+');
        Term();
        emit('+', tokenval);
        SimpleExpr();
    }
    else if (lookahead == '-')
    {
        match('-');
        Term();
        emit('-', tokenval);
        SimpleExpr();
    }
    else
    {
        /* Empty */
    }
}

void Statements() // Not Sure it works
{
    Statement();
    if (lookahead = ';')
    {
        Statements();
    }
    else
    {
        /*Epsilon*/
    }
}

void Statement()
{
    switch (lookahead)
    {
    case ID:
        match(ID);
        match(':');
        match('=');
        Expr();
        break;
    case BEGIN:
        match(BEGIN);
        Block();
        break;
    case IF:
        match(IF);
        Expr();
        match(THEN);
        Statement();
        break;
    case REPEAT:
        match(REPEAT);
        Statement();
        match(UNTIL);
        Expr();
        break;
    case WRITELN:
        match(WRITELN);
        match('(');
        SimpleExpr();
        match(')');
        break;
    default:
        mismatch_error(404); // enter the custom error code
        break;
    }
}

void Expr()
{
    SimpleExpr();
    switch (lookahead)
    {
    case '=':
        match('=');
        SimpleExpr();
        emit('=', tokenval);
        break;
    case '<':
        match('<');
        SimpleExpr();
        emit('<', tokenval);
        break;
    case '>':
        match('>');
        SimpleExpr();
        emit('>', tokenval);
        break;
    case '<=':
        match('<=');
        SimpleExpr();
        emit('<=', tokenval);
        break;
    case '>=':
        match('>=');
        SimpleExpr();
        emit('>=', tokenval);
        break;
    case '<>>':
        match('<>');
        SimpleExpr();
        emit('<>', tokenval);
        break;
    default:
        mismatch_error(404); // enter the custom error code
        break;
    }
}

void Declarations()
{
    strcpy(current_rule, "declarations");
    switch (lookahead)
    {
    case BEGIN:
        Block();
        break;

    case CONST:
        match(CONST);
        ConstantDefinition();
        ConstantDefinitions();
        break;

    case VAR:
        match(VAR);
        VariableDeclaration();
        VariableDeclarations();
        break;

    default:
        mismatch_error(404);
        break;
    }
}

void ConstantDefinition()
{
    strcpy(current_rule, "const definition");
    match(ID);
    match('=');
    match(NUM);
    match(';');
}

void ConstantDefinitions()
{
    strcpy(current_rule, "const definitions");
    if (lookahead == VAR)
    {
        match(VAR);
        VariableDeclarations();
    }
    else if (lookahead == CONST)
    {
        match(CONST);
        ConstantDefinition();
        ConstantDefinitions();
    }
    else
    {
        // epsilon
    }
}

void list()
{
    if (lookahead == '(' || lookahead == ID || lookahead == NUM)
    {
        Expr();
        match(';');
        list();
    }
    else
    {
        /* Empty */
    }
}

void Term()
{
    /* Just one production for Term, so we don't need to check lookahead */
    Factor();
    MoreFactors();
}

void MoreFactors()
{
    if (lookahead == '*')
    {
        match('*');
        Factor();
        emit('*', tokenval);
        MoreFactors();
    }
    else if (lookahead == '/')
    {
        match('/');
        Factor();
        emit('/', tokenval);
        MoreFactors();
    }
    else if (lookahead == DIV)
    {
        match(DIV);
        Factor();
        emit(DIV, tokenval);
        MoreFactors();
    }
    else if (lookahead == MOD)
    {
        match(MOD);
        Factor();
        emit(MOD, tokenval);
        MoreFactors();
    }
    else
    {
        /* Empty */
    }
}

void Factor()
{
    switch (lookahead)
    {
    case '(':
        match('(');
        Expr();
        match(')');
        break;
    case ID:
        int id_lexeme = tokenval;
        match(ID);
        emit(ID, id_lexeme);
        break;
    case NUM:
        int num_value = tokenval;
        match(NUM);
        emit(NUM, num_value);
        break;
    default:
        mismatch_error(404); // enter the custom error code

        break;
    }
}

/**
 * if the paramter matches the current look ahead, it is consumed
 * if it does not, the program panics.
 * @param t the word to match
 */
int match(int t)
{
    if (lookahead == t)
        lookahead = lexan();
    else
        mismatch_error(t);
}