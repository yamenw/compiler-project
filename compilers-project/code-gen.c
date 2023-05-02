#ifndef CODEGEN_H
#define CODEGEN_H
#include "global.h"


// void annotate(entry node)
// {
// }

void emit(int t, int tval) /*  generates output  */
{
    switch (t)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        printf("%c\n", t);
        break;
    case DIV:
        printf("DIV\n");
        break;
    case MOD:
        printf("MOD\n");
        break;
    case NUM:
        printf("%d\n", tval);
        break;
    case ID:
        printf("%s %s\n", symtable[tval].type, symtable[tval].lexptr);
        break;
    default:
        printf("token %d, tokenval %d\n", t, tval);
    }
}
#endif /* CODEGEN_H */