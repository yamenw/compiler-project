// Compilers: Principles, Techniques, and Tools, 1st Edition (1986)
//   by Alfred V. Aho, Ravi. Sethi, Jeffrey D. Ullman
// Section 2.9

// http://basen.oru.se/kurser/koi/2010-2011-p1/asu-86/29/files.html

/* global.h */

#include <stdio.h>  /* include declarations for i/o routines */
#include <ctype.h>  /* ... and for character test routines */
#include <stdlib.h> /* ... and for some standard routines, such as exit */
#include <string.h> /* ... and for string routines */

#define BSIZE  128  /* buffer size */
#define NONE   -1
#define EOS    '\0'

#define NUM    256
#define DIV    257
#define MOD    258
#define ID     259
#define DONE   260

extern int tokenval;   /*  value of token attribute */
extern int lineno;

struct entry {  /*  form of symbol table entry  */
  char *lexptr;
  int  token;
};

extern struct entry symtable[];  /* symbol table  */

extern void init();  /*  loads keywords into symtable  */
extern void error(char* m);  /*  generates all error messages  */
extern int lexan();  /*  lexical analyzer  */
extern void parse();  /*  parses and translates expression list  */
extern int insert(char *s, int tok);  /*  returns position of entry for s */
extern int lookup(char *s);  /*  returns position of entry for s */
extern void emit (int t, int tval);  /*  generates output  */

/* lexer.c */

// #include "global.h"

char lexbuf[BSIZE];
int  lineno = 1;
int  tokenval = NONE;

int lexan ()  /*  lexical analyzer  */
{

  int t;
  while(1) {
    t = getchar ();
    if (t == ' ' || t == '\t')
      ;  /*  strip out white space  */
    else if (t == '\n')
      lineno = lineno + 1;
    else if (isdigit (t)) {  /*  t is a digit  */
      ungetc(t, stdin);
      scanf("%d", &tokenval);
      return NUM;
    }
    else if (isalpha(t)) {  /*  t is a letter */
      int p, b = 0;
      while (isalnum(t)) {  /* t is alphanumeric  */
        lexbuf [b] = t;
        t = getchar ();
        b = b + 1;
        if (b >= BSIZE)
          error("compiler error");
      }
      lexbuf[b] = EOS;
      if (t != EOF)
        ungetc(t, stdin);
      p = lookup (lexbuf);
      if (p == 0)
        p = insert (lexbuf, ID);
      tokenval = p;
      return symtable[p].token;
    }
    else if (t == EOF)
      return DONE;
    else {
      tokenval = NONE;
      return t;
    }
  }
}

/* parser.c -- without the optimizations */

// #include "global.h"

int lookahead;

void match(int);
void start(), list(), Expr(), moreterms(), term(), morefactors(), factor();

void parse()  /*  parses and translates expression list  */
{
  lookahead = lexan();
  start();
}

void start ()
{
  /* Just one production for start, so we don't need to check lookahead */
  list(); match(DONE);
}

void list()
{
  if (lookahead == '(' || lookahead == ID || lookahead == NUM) {
    Expr(); match(';'); list();
  }
  else {
    /* Empty */
  }
}

void Expr ()
{
  /* Just one production for Expr, so we don't need to check lookahead */
  term(); moreterms();
}

void moreterms()
{
  if (lookahead == '+') {
    match('+'); term(); emit('+', tokenval); moreterms();
  }
  else if (lookahead == '-') {
    match('-'); term(); emit('-', tokenval); moreterms();
  }
  else {
    /* Empty */
  }
}

void term ()
{
  /* Just one production for term, so we don't need to check lookahead */
  factor(); morefactors();
}

void morefactors ()
{
  if (lookahead == '*') {
    match('*'); factor(); emit('*', tokenval); morefactors();
  }
  else if (lookahead == '/') {
    match('/'); factor(); emit('/', tokenval); morefactors();
  }
  else if (lookahead == DIV) {
    match(DIV); factor(); emit(DIV, tokenval); morefactors();
  }
  else if (lookahead == MOD) {
    match(MOD); factor(); emit(MOD, tokenval); morefactors();
  }
  else {
    /* Empty */
  }
}

void factor ()
{
  if (lookahead == '(') {
    match('('); Expr(); match(')');
  }
  else if (lookahead == ID) {
    int id_lexeme = tokenval;
    match(ID);
    emit(ID, id_lexeme);
  }
  else if (lookahead == NUM) {
    int num_value = tokenval;
    match(NUM);
    emit(NUM, num_value);
  }
  else
    error("syntax error in factor");
}

void match(int t)
{
  if (lookahead == t)
    lookahead = lexan();
  else
    error ("syntax error in match");
}

/* emitter.c */

// #include "global.h"

void emit (int t, int tval)  /*  generates output  */
{
  switch(t) {
  case '+' : case '-' : case '*' : case '/':
    printf("%c\n", t); break;
  case DIV:
    printf("DIV\n"); break;
  case MOD:
    printf("MOD\n"); break;
  case NUM:
    printf("%d\n", tval); break;
  case ID:
    printf("%s\n", symtable[tval].lexptr); break;
  default:
    printf("token %d, tokenval %d\n", t, tval);
  }
}

/* symbol.c */

// #include "global.h"

#define STRMAX 999  /*  size of lexemes array  */
#define SYMMAX 100  /*  size of symbol table */

char lexemes[STRMAX];
int  lastchar = - 1;  /*  last used position in lexemes   */
struct entry symtable[SYMMAX];
int lastentry = 0;    /*  last used position in symtable  */

int lookup(char *s)         /*  returns position of entry for s */
{
  int p;
  for (p = lastentry; p > 0; p = p - 1)
    if (strcmp(symtable[p].lexptr, s) == 0)
      return p;
  return 0;
}

int insert(char *s, int tok)    /*  returns position of entry for s */
{
  int len;
  len = strlen(s);  /*  strlen computes length of s     */
  if (lastentry + 1 >= SYMMAX)
    error ("symbol table full");
  if (lastchar + len + 1 >= STRMAX)
    error ("lexemes array full");
  lastentry = lastentry + 1;
  symtable[lastentry].token = tok;
  symtable[lastentry].lexptr = &lexemes[lastchar + 1];
  lastchar = lastchar + len + 1;
  strcpy(symtable[lastentry].lexptr, s);
  return lastentry;
}

/* init.c */

// #include "global.h"

struct entry keywords[] = {
  { "div", DIV },
  { "mod", MOD, },
  { 0,     0 }
};

void init()  /*  loads keywords into symtable  */
{
  struct entry *p;
  for (p = keywords; p->token; p++)
    insert(p->lexptr, p->token);
}

/* error.c */

// #include "global.h"

void error(char* m)  /* generates all error messages  */
{
  fprintf(stderr, "line %d: %s\n", lineno, m);
  exit(EXIT_FAILURE);  /*  unsuccessful termination  */
}

/* main.c */

// #include "global.h"

int main(void)
{
  init();
  parse();
  exit(0);    /*  successful termination  */
}