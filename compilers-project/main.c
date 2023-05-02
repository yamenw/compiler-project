#include "global.h"

void init() /*  loads keywords into symtable  */
{
    struct entry *p;
    for (p = keywords; p->token; p++)
        insert(p->lexptr, p->token);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Expected 2 arguments, recieved %d.", argc);
        exit(1);
    }

    in_file = freopen(argv[1], "r", stdin);
    out_file = freopen(argv[2], "w", stdout);
    init();
    parse();
    fclose(in_file);
    fclose(out_file);
    exit(0);
}