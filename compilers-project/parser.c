// #include "global.h"
#include "scanner.h"
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
    strcpy(current_rule, "identifier list");
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
    Statements(); // THIS SEGFAULTS
    strcpy(current_rule, "block");
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
    strcpy(current_rule, "simple expr");
    Term();
    if (lookahead == '+')
    {
        match('+');
        emit('+', tokenval);
        Term();
        SimpleExprPrime();
    }
    else if (lookahead == '-')
    {
        match('-');
        emit('-', tokenval);
        Term();
        SimpleExprPrime();
    }
    else
    {
        /* Empty */
    }
}

void SimpleExprPrime()
{
    strcpy(current_rule, "simple expr prime");
    if (lookahead == '+')
    {
        match('+');
        emit('+', tokenval);
        Term();
        SimpleExprPrime();
    }
    else if (lookahead == '-')
    {
        match('-');
        emit('-', tokenval);
        Term();
        SimpleExprPrime();
    }
    else
    {
        /* Empty */
    }
}

void StatementsPrime()
{
    strcpy(current_rule, "statements P");
    if (lookahead == ';')
    {
        match(';');
        Statement();
        StatementsPrime();
    }
    else
    { // the emitter is not done yet
        /*Epsilon is other files working?? so I copy em and format em ffs We do it on weekend u gonna kys wit this just give in and we continue I can assure that 70% at least are not done with it*/
    }
}

void Statements() // Not Sure it works
{
    strcpy(current_rule, "statements");
    Statement();
    StatementsPrime();
}

void Statement()
{
    strcpy(current_rule, "statement");
    switch (lookahead)
    {
    case ID:
        match(ID);
        emit(ID, tokenval);
        match(':');
        match('=');
        emit('=', tokenval);
        Expr();
        break;
    case BEGIN:
        match(BEGIN);
        emit(BEGIN, tokenval);
        Block();
        break;
    case IF:
        match(IF);
        emit(IF, tokenval);
        Expr();
        match(THEN);
        emit(THEN, tokenval);
        Statement();
        break;
    case REPEAT:
        match(REPEAT);
        emit(REPEAT, tokenval);
        Statement();
        strcpy(current_rule, "statement");
        match(UNTIL);
        emit(UNTIL, tokenval);
        Expr();
        break;
    case WRITELN:
        match(WRITELN);
        emit(WRITELN, tokenval);
        match('(');
        emit('(', tokenval);
        SimpleExpr();
        match(')');
        emit(')', tokenval);
        break;
    default:
        // epsilon
        break;
    }
}

void ExprPrime()
{
    strcpy(current_rule, "expr prime");
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
        emit('>', tokenval);
        SimpleExpr();
        break;
    // case '<=':
    //     match('<=');
    //     SimpleExpr();
    //     emit('<=', tokenval);
    //     break;
    // case '>=':
    //     match('>=');
    //     SimpleExpr();
    //     emit('>=', tokenval);
    //     break;
    // case '<>>':
    //     match('<>');
    //     SimpleExpr();
    //     emit('<>', tokenval);
    //     break;
    default:
        // EPSILON
        break;
    }
}

void Expr()
{
    strcpy(current_rule, "expr");
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
        emit('>', tokenval);
        SimpleExpr();
        break;
    // case '<=':
    //     match('<=');
    //     SimpleExpr();
    //     emit('<=', tokenval);
    //     break;
    // case '>=':
    //     match('>=');
    //     SimpleExpr();
    //     emit('>=', tokenval);
    //     break;
    // case '<>>':
    //     match('<>');
    //     SimpleExpr();
    //     emit('<>', tokenval);
    //     break;
    default:
        // mismatch_error(404); // enter the custom error code
        // EPSILON AND NOT AN ERROR DAMN IT.
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
    int lexeme = tokenval;
    match(ID);
    match('=');
    int value = tokenval;
    match(NUM);
    match(';');
    annotate(lexeme, lexeme, value);
    emit(ID, lexeme);
    emit(';', tokenval);
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
    strcpy(current_rule, "list");
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
    strcpy(current_rule, "term");
    /* Just one production for Term, so we don't need to check lookahead */
    Factor();
    MoreFactors();
}

void MoreFactors()
{
    strcpy(current_rule, "more factors");
    if (lookahead == '*')
    {
        match('*');
        emit('*', tokenval);
        Factor();
        MoreFactors();
    }
    else if (lookahead == '/')
    {
        match('/');
        emit('/', tokenval);
        Factor();
        MoreFactors();
    }
    else if (lookahead == DIV)
    {
        match(DIV);
        emit(DIV, tokenval);
        Factor();
        MoreFactors();
    }
    else if (lookahead == MOD)
    {
        match(MOD);
        emit(MOD, tokenval);
        Factor();
        MoreFactors();
    }
    else
    {
        /* Empty */
    }
}

void Factor()
{
    strcpy(current_rule, "factor");
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
        annotate(ID, id_lexeme, REAL_T);
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