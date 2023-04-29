#include "global.h"

void init() /*  loads keywords into symtable  */
{
    struct entry *p;
    for (p = keywords; p->token; p++)
        insert(p->lexptr, p->token);
}

int main(void)
{
    init();
    parse();
    exit(0); /*  successful termination  */
}