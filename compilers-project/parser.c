#include "global.h"
#include "scanner.c"
#include "code-gen.c"

int lookahead;

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
    ExpressionList(),
    Expression(),
    Term(),
    Factor();

void parse() /*  parses and translates expression list  */
{
    lookahead = lexan();
    start();
}

void start()
{
    /* Just one production for start, so we don't need to check lookahead */
    Program();
    Declarations();
}

void Program()
{
    Header();
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

void Declarations()
{
    match(CONST);
    // WIP
}

void ConstantDefinition()
{
}

void ConstantDefinitions()
{
    ConstantDefinition();
}

void list()
{
    if (lookahead == '(' || lookahead == ID || lookahead == NUM)
    {
        expr();
        match(';');
        list();
    }
    else
    {
        /* Empty */
    }
}

void expr()
{
    /* Just one production for expr, so we don't need to check lookahead */
    term();
    moreterms();
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
        expr();
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

const char *getTokenValue(int token)
{
    struct entry *p;
    for (p = keywords; p->token; p++)
    {
        if (p->token == token)
        {
            return p->lexptr;
        }
    }
    char str[12]; // Make sure the buffer is large enough to hold the string
    sprintf(str, "%d", token);
    return str;
}

void match(int t)
{
    if (lookahead == t)
        lookahead = lexan();
    else
    {
        char msg[100];
        sprintf(
            msg,
            "syntax error in match, expected %s (ASCII='%d') but found %s (ASCII='%d')",
            getTokenValue(t),
            t,
            getTokenValue(lookahead),
            lookahead);
        error(msg);
    }
}

void Block(){

};