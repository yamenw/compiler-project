#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BSIZE 128
#define NONE -1
#define EOS '\0'

#define NUM 256
#define DIV 257
#define MOD 258
#define ID 259
#define DONE 260
#define EXP 261
#define STRT 262
#define STOP 263
#define INF 264
#define PSTF 265

extern int tokenval;
extern int lineno;

struct entry {
    char *lexptr;
    int token;
};

extern struct entry symtable[];

extern void init();
extern void error(char *m);
extern int lexan();
extern void parse();
extern int insert(char *s, int tok);
extern int lookup(char *s);
extern void emit(int t, int tval);
extern void header();
extern void footer();
extern void match_and_emit(int t);

char lexbuf[BSIZE];
int lineno = 1;
int tokenval = NONE;
FILE *in_file, *out_file;

int lexan() {
    int t;
    int is_single_line_comment = 0;
    while (1) {
        t = getchar();
        if (t == ' ' || t == '\t')
            ;
        else if (t == '\n') {
            lineno = lineno + 1;
            is_single_line_comment = 0;
        } else if (t == '$' || is_single_line_comment)  // skip comments
        {
            is_single_line_comment = 1;
        } else if (isdigit(t)) {
            ungetc(t, stdin);
            scanf("%d", &tokenval);
            return NUM;
        } else if (isalpha(t)) {
            int p, b = 0;
            while (isalnum(t)) {
                lexbuf[b] = t;
                t = getchar();
                b = b + 1;
                if (b >= BSIZE) error("compiler error");
            }
            lexbuf[b] = EOS;
            if (t != EOF) ungetc(t, stdin);
            p = lookup(lexbuf);
            if (p == 0) p = insert(lexbuf, ID);
            tokenval = p;
            return symtable[p].token;
        } else if (t == EOF)
            return DONE;
        else {
            tokenval = NONE;
            return t;
        }
    }
}

int lookahead;

void match(int);
void start(), list(), expr(), moreterms(), term(), morefactors(), factor();

void parse() {
    lookahead = lexan();
    start();
}

void start() {
    header();
    list();
    footer();
    match(DONE);
}

void list() {
    if (lookahead == '(' || lookahead == ID || lookahead == NUM) {
        expr();
        match_and_emit(';');
        list();
    }
}

void expr() {
    term();
    moreterms();
}

void moreterms() {
    if (lookahead == '+') {
        match('+');
        term();
        emit('+', tokenval);
        moreterms();
    } else if (lookahead == '-') {
        match('-');
        term();
        emit('-', tokenval);
        moreterms();
    } else if (lookahead == '|') {
        match('|');
        term();
        emit('|', tokenval);
        moreterms();
    } else {
    }
}

void term() {
    factor();
    morefactors();
}

void morefactors() {
    if (lookahead == '*') {
        match('*');
        factor();
        emit('*', tokenval);
        morefactors();
    } else if (lookahead == '&') {
        match('&');
        factor();
        emit('&', tokenval);
        morefactors();
    } else if (lookahead == '/') {
        match('/');
        factor();
        emit('/', tokenval);
        morefactors();
    } else if (lookahead == DIV) {
        match(DIV);
        factor();
        emit(DIV, tokenval);
        morefactors();
    } else if (lookahead == MOD) {
        match(MOD);
        factor();
        emit(MOD, tokenval);
        morefactors();
    }
}

void match_and_emit(int token) {
    int lexeme = tokenval;
    match(token);
    emit(token, lexeme);
}

void factor() {
    if (lookahead == '(') {
        match('(');
        expr();
        match(')');
    } else if (lookahead == ID) {
        match_and_emit(ID);
    } else if (lookahead == NUM) {
        match_and_emit(NUM);
    } else
        error("syntax error in factor");
}

void match(int t) {
    if (lookahead == t)
        lookahead = lexan();
    else
        error("syntax error in match");
}

void header() {
    match_and_emit(EXP);
    match_and_emit(ID);
    match_and_emit('(');
    match_and_emit(INF);
    match_and_emit(',');
    match_and_emit(PSTF);
    match_and_emit(')');
    match_and_emit(STRT);
}

void footer() { match_and_emit(STOP); }

void emit(int t, int tval) {
    switch (t) {
        case '(':
        case ',':
        case ')':
            printf("%c", t);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '&':
        case '|':
            printf("%c ", t);
            break;
        case DIV:
            printf("DIV ");
            break;
        case MOD:
            printf("MOD ");
            break;
        case NUM:
            printf("%d ", tval);
            break;
        case ID:
            printf("%s ", symtable[tval].lexptr);
            break;
        case EXP:
            printf("expressions");
            break;
        case INF:
            printf("inf");
            break;
        case PSTF:
            printf("postf");
            break;
        case STRT:
            printf("\nstart\n");
            break;
        case STOP:
            printf("stop");
            break;
        case ';':
            fseek(out_file, -1,
                  SEEK_CUR);  // delete one trailing whitespace character.
            printf(";\n");
            break;
        default:
            printf("token %d, tokenval %d\n", t, tval);
    }
}

#define STRMAX 999
#define SYMMAX 100

char lexemes[STRMAX];
int lastchar = -1;
struct entry symtable[SYMMAX];
int lastentry = 0;

int lookup(char *s) {
    int p;
    for (p = lastentry; p > 0; p = p - 1)
        if (strcmp(symtable[p].lexptr, s) == 0) return p;
    return 0;
}

int insert(char *s, int tok) {
    int len;
    len = strlen(s);
    if (lastentry + 1 >= SYMMAX) error("symbol table full");
    if (lastchar + len + 1 >= STRMAX) error("lexemes array full");
    lastentry = lastentry + 1;
    symtable[lastentry].token = tok;
    symtable[lastentry].lexptr = &lexemes[lastchar + 1];
    lastchar = lastchar + len + 1;
    strcpy(symtable[lastentry].lexptr, s);
    return lastentry;
}

struct entry keywords[] = {
    {"div", DIV},   {"mod", MOD}, {"expressions", EXP}, {"start", STRT},
    {"stop", STOP}, {"inf", INF}, {"postf", PSTF},      {0, 0}};

void init() {
    struct entry *p;
    for (p = keywords; p->token; p++) insert(p->lexptr, p->token);
}

void error(char *m) {
    fprintf(stderr, "line %d: %s\n", lineno, m);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
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