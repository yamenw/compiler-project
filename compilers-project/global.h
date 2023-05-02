#ifndef GLOBAL_H
#define GLOBAL_H
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
#define PROGRAM 271
#define INPUT 272
#define OUTPUT 273
#define CONST 274
#define INTEGER 275
#define REAL 276
#define CHAR 277
#define BOOLEAN 278
#define BEGIN 279
#define END 280
#define IF 281
#define THEN 282
#define REPEAT 283
#define UNTIL 284
#define WRITELN 285
#define NOT 286
#define INP 287
#define OUT 288
int lineno = 1;
char lexbuf[BSIZE];
int tokenval = NONE;

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
    {"mod", MOD},
    {"mod", MOD},
    {"program", PROGRAM},
    {"input", INPUT},
    {"output", OUTPUT},
    {"const", CONST},
    {"integer", INTEGER},
    {"real", REAL},
    {"char", CHAR},
    {"boolean", BOOLEAN},
    {"begin", BEGIN},
    {"end", END},
    {"if", IF},
    {"then", THEN},
    {"repeat", REPEAT},
    {"until", UNTIL},
    {"writeln", WRITELN},
    {"not", NOT},
    {0, 0}};

void error(char *m) /* generates all error messages  */
{
    fprintf(stderr, "line %d: %s\n", lineno, m);
    exit(EXIT_FAILURE); /*  unsuccessful termination  */
}

#endif /* GLOBAL_H */