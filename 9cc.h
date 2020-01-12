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
    TK_EOF
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
    ND_ADD,
    ND_SUB,
    ND_DIV,
    ND_MUL,
    ND_EQ,
    ND_NE,
    ND_LE,
    ND_LT,
};

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node *lhs, *rhs;
    int val;
};

// global
Token *cur_token;

// parser function
Node *parser();

// code gen function
void gen(Node *cur);

// tokenizer function
Token *tokenizer(char *p);