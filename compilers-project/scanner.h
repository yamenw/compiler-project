#include "global.h"
#ifndef SCANNER_H
#define SCANNER_H

int lexan() /*  lexical analyzer  */
{

    int t;
    while (1)
    {
        t = getchar();
        colno++;
        if (t == ' ' || t == '\t')
            colno++; /*  strip out white space  */
        else if (t == '\n')
            lineno = lineno + 1, colno = 1;
        else if (isdigit(t))
        { /*  t is a digit  */
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            return NUM;
        }
        else if (isalpha(t))
        { /*  t is a letter */
            int p, b = 0;
            t = tolower(t);
            while (isalnum(t))
            { /* t is alphanumeric  */
                lexbuf[b] = isalpha(t) ? tolower(t) : t;
                t = getchar();
                colno++;
                b = b + 1;
                if (b >= BSIZE)
                    error("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF)
                ungetc(t, stdin);
            p = lookup(lexbuf);
            if (p == 0)
                p = insert(lexbuf, ID);
            tokenval = p;
            return symtable[p].token;
        }
        else if (t == EOF)
        {
            return DONE;
        }
        else
        {
            tokenval = NONE;
            return t;
        }
    }
}

#endif /* SCANNER_H */
