#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

/* token structure */
typedef enum TokenKind TokenKind;
enum TokenKind {
    TK_NUM,
    TK_SYMBOL,
    TK_IDENT,
    TK_KEYWORD,
    TK_EOF,
};

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token* next;
    int val;
    int len;
    char *str;
};

/* node structure */
typedef enum NodeKind NodeKind;
enum NodeKind {
    ND_NUM,
    ND_LVAR,
    ND_ADD,
    ND_SUB,
    ND_DIV,
    ND_MUL,
    ND_EQ,
    ND_NE,
    ND_LE,
    ND_LT,
    ND_ASSIGN,
    ND_RETURN,
    ND_IF,
    ND_WHILE,
    ND_BLOCK,
};

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs, *rhs;
    Node *condition, *statements, *else_statement;
    Node **stmts;
    int val;  // for number node
    int offset; // for local var node
};

typedef struct Lvar Lvar;
struct Lvar {
    Lvar *next;
    char *str;
    int len;
    int offset;
};


// global
Token *cur_token;
Node *code[100];
Lvar *locals; // symbol table

// parser function
void parser();
Node *expr();
Node *assign();

// code gen function
void gen(Node *cur);

// tokenizer function
void tokenizer(char *p);