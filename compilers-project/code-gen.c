#ifndef CODEGEN_H
#define CODEGEN_H

#include "global.h"

void annotate(int tval, int lexeme, int value)
{
    symtable[tval].value.real = value;
}

void emit(int t, int tval) /*  generates output  */
{
    switch (t)
    {
    case DIV:
    case MOD:
    case PROGRAM:
    case INPUT:
    case OUTPUT:
    case CONST:
    case INTEGER:
    case REAL:
    case CHAR:
    case BOOLEAN:
    case BEGIN:
    case END:
    case IF:
    case THEN:
    case REPEAT:
    case UNTIL:
    case WRITELN:
    case NOT:
    case INP:
    case OUT:
    case VAR:
        printf("%s ", getTokenValue(t));
        break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '>':
    case '<':
        printf("%c ", t);
        break;
    case '=':
    case ':':
        printf("%c", t);
        break;
    case ';':
        printf("%c\n", t);
        break;
    case NUM:
        printf("%d\n", tval);
        break;
    case ID:
        printf("%s %s\n", dataTypeID[symtable[tval].type], symtable[tval].lexptr);
        break;
    default:
        printf("token %d, tokenval %d\n", t, tval);
    }
}
#endif /* CODEGEN_H */