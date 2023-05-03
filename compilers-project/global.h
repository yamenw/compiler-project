#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>  
#include <ctype.h>  
#include <stdlib.h> 
#include <string.h> 

#define BSIZE 1024 
#define NONE -1
#define EOS '\0'

#define NUM 256
#define DIV 257
#define MOD 258
#define ID 259
#define DONE 260
#define STRMAX 999 
#define SYMMAX 999 
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
#define VAR 289

int lineno = 1, colno = 1;
char lexbuf[BSIZE];
int tokenval = NONE;
int lookahead;

const char *dataTypeID[4] = {"float", "bool", "int", "char"};

typedef enum
{
    REAL_T = REAL,
    BOOLEAN_T = BOOLEAN,
    INTEGER_T = INTEGER,
    CHAT_T = CHAR
} dataType;

typedef enum
{
    true = 1,
    false = 0
} bool;

typedef struct entry
{
    char *lexptr;
    int token;
    dataType type;
    union
    {
        float real;
        bool boolean;
        char character;
        int integer;
    } value;
};

extern struct entry symtable[]; 

extern void init();                  
extern void error(char *m);          
extern int lexan();                  
extern void parse();                 
extern int insert(char *s, int tok); 
extern int lookup(char *s);          
extern void emit(int t, int tval);   

char lexemes[STRMAX];
int lastchar = -1; 
struct entry symtable[SYMMAX];
int lastentry = 0; 
FILE *in_file, *out_file;

int lookup(char *s) 
{
    int p;
    for (p = lastentry; p > 0; p = p - 1)
        if (strcmp(symtable[p].lexptr, s) == 0)
            return p;
    return 0;
}

int insert(char *s, int tok) 
{
    int len;
    len = strlen(s); 
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
    {"inp", INP},
    {"out", OUT},
    {"var", VAR},
    {0, 0}};

void error(char *m) 
{
    fprintf(stderr, "line %d column %d: %s\n", lineno, colno, m);
    exit(EXIT_FAILURE); 
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
    char str[24]; // Make sure the buffer is large enough to hold the string
    sprintf(str, "%d", token);
    return str;
}

char *current_rule[512];

void mismatch_error(int t)
{
    char msg[512];
    sprintf(
        msg,
        "syntax error in match, expected %s (ASCII='%d') but found %s (ASCII='%d') in %s",
        getTokenValue(t),
        t,
        getTokenValue(lookahead),
        lookahead,
        current_rule);
    error(msg);
}

#endif 