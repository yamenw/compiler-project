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

void Term()
{
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
    // TODO: implement
    Term();
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
    if (lookahead == ID)
    {
        match(ID);
        match(':');
        match('=');
        Expr();
    }
    else if (lookahead == BEGIN)
    {
        match(BEGIN);
        Block();
    }
    else if (lookahead == IF)
    {
        match(IF);
        Expr();
        match(THEN);
        Statement();
    }
    else if (lookahead == REPEAT)
    {
        match(REPEAT);
        Statement();
        match(UNTIL);
        Expr();
    }
    else if (lookahead == WRITELN)
    {
        match(WRITELN);
        match('(');
        SimpleExpr();
        match(')');
    }
    else
    {
        /*Epsilon*/
    }
}

void ExprList()
{
    Expr();
    /*wip*/
}

void Expr()
{
    /* Just one production for Expr, so we don't need to check lookahead */
    SimpleExpr();
    if (lookahead == '=')
    {
        match('=');
        SimpleExpr();
        emit('=', tokenval);
    }
    else if (lookahead == '<')
    {
        match('<');
        if (lookahead == '>')
        {
            match('>');
            SimpleExpr();
            emit('>', tokenval);
        }
        else if (lookahead == '=')
        {
            match('=');
            SimpleExpr();
            emit('=', tokenval);
        }
    }
    else if (lookahead == '>')
    {
        match('>');
        if (lookahead == '=')
        {
            match('=');
            SimpleExpr();
            emit('=', tokenval);
        }
    }
    else
    {
        /* Empty */
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

void moreterms()
{
    if (lookahead == '+')
    {
        match('+');
        term();
        emit('+', tokenval);
        moreterms();
    }
    else if (lookahead == '-')
    {
        match('-');
        term();
        emit('-', tokenval);
        moreterms();
    }
    else
    {
        /* Empty */
    }
}

void term()
{
    /* Just one production for term, so we don't need to check lookahead */
    factor();
    morefactors();
}

void morefactors()
{
    if (lookahead == '*')
    {
        match('*');
        factor();
        emit('*', tokenval);
        morefactors();
    }
    else if (lookahead == '/')
    {
        match('/');
        factor();
        emit('/', tokenval);
        morefactors();
    }
    else if (lookahead == DIV)
    {
        match(DIV);
        factor();
        emit(DIV, tokenval);
        morefactors();
    }
    else if (lookahead == MOD)
    {
        match(MOD);
        factor();
        emit(MOD, tokenval);
        morefactors();
    }
    else
    {
        /* Empty */
    }
}

void factor()
{
    if (lookahead == '(')
    {
        match('(');
        Expr();
        match(')');
    }
    else if (lookahead == ID)
    {
        int id_lexeme = tokenval;
        match(ID);
        emit(ID, id_lexeme);
    }
    else if (lookahead == NUM)
    {
        int num_value = tokenval;
        match(NUM);
        emit(NUM, num_value);
    }
    else
        error("syntax error in factor");
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