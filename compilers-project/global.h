#include <stdio.h>  /* include declarations for i/o routines */
#include <ctype.h>  /* ... and for character test routines */
#include <stdlib.h> /* ... and for some standard routines, such as exit */
#include <string.h> /* ... and for string routines */

#define BSIZE 128 /* buffer size */
#define NONE -1
#define EOS '\0'

#define NUM 256
#define DIV 257
#define MOD 258
#define ID 259
#define DONE 260
#define STRMAX 999 /*  size of lexemes array  */
#define SYMMAX 100 /*  size of symbol table */

extern int tokenval; /*  value of token attribute */
extern int lineno;

struct entry
{ /*  form of symbol table entry  */
    char *lexptr;
    int token;
};

extern struct entry symtable[]; /* symbol table  */

extern void init();                  /*  loads keywords into symtable  */
extern void error(char *m);          /*  generates all error messages  */
extern int lexan();                  /*  lexical analyzer  */
extern void parse();                 /*  parses and translates expression list  */
extern int insert(char *s, int tok); /*  returns position of entry for s */
extern int lookup(char *s);          /*  returns position of entry for s */
extern void emit(int t, int tval);   /*  generates output  */

char lexemes[STRMAX];
int lastchar = -1; /*  last used position in lexemes   */
struct entry symtable[SYMMAX];
int lastentry = 0; /*  last used position in symtable  */
FILE *in_file, *out_file;

int lookup(char *s) /*  returns position of entry for s */
{
    int p;
    for (p = lastentry; p > 0; p = p - 1)
        if (strcmp(symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}

int insert(char *s, int tok) /*  returns position of entry for s */
{
    int len;
    len = strlen(s); /*  strlen computes length of s     */
    if (lastentry + 1 >= SYMMAX)
        error("symbol table full");
    if (lastchar + len + 1 >= STRMAX)
        error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}

struct entry keywords[] = {
    {"div", DIV},
    {
        "mod",
        MOD,
    },
    {0, 0}};

void error(char *m) /* generates all error messages  */
{
    fprintf(stderr, "line %d: %s\n", lineno, m);
    exit(EXIT_FAILURE); /*  unsuccessful termination  */
}