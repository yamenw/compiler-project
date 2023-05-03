#ifndef CODEGEN_H
#define CODEGEN_H

#include "global.h"
#define ASSIGN 3000
#define HEADER 4000
#define DECL 5000

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
        break;
    case IF:
        printf("if ((", t);
        break;
    case THEN:
        printf("))", t);
        break;
    case REPEAT:
        printf("do {", t);
        break;
    case UNTIL:
        printf("} while (!(", t);
        break;
    case WRITELN:
        printf("printf(\"%%d\", %s", symtable[tval].lexptr);
        break;
    case NOT:
    case INP:
    case OUT:
    case VAR:
        printf("%s ", getTokenValue(t));
        break;
    case ')':
    case '}':
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
    case ASSIGN:
        printf("%s ", symtable[tval].lexptr);
        break;
    case HEADER:
        printf("#include<iostream>\nusing namespace std;\n");
        break;
    case DECL:
        printf("void main(void){");
        break;
    default:;
        // printf("token %d, tokenval %d\n", t, tval);
    }
}
#endif /* CODEGEN_H */