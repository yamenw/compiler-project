#include "global.h"
#include "scanner.c"
#include "code-gen.c"

void match(int);
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
}

void Program()
{
    /* Just one production for start, so we don't need to check lookahead */
    Header();
    Declarations();
    Block();
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
    Statements();
    match(END);
}

void VariableDeclarations()
{
    // TODO: implement
    strcpy(current_rule, "const definition");
}

void VariableDeclaration()
{
    // TODO: implement
    strcpy(current_rule, "const definition");
}

void SimpleExpr()
{
    // TODO: implement
}

void Statements() // Not Sure it works
{
    Statement();
    if (lookahead = ';')
    {
        Statement();
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
}

void Declarations()
{
    // mismatch_error(404);
    // lookahead = lexan();
    strcpy(current_rule, "declarations");
    switch (lookahead)
    {
    case BEGIN:
        Block();
        break;

    case CONST:
        ConstantDefinition();
        ConstantDefinitions();
        break;

    case VAR:
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
}

void ConstantDefinitions()
{
    strcpy(current_rule, "const definition");
    ConstantDefinition();
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

void Expr()
{
    /* Just one production for Expr, so we don't need to check lookahead */
    SimpleExpr();
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

void match(int t)
{
    if (lookahead == t)
        lookahead = lexan();
    else
        mismatch_error(t);
}