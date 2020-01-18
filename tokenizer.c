#include "9cc.h"

const int keyword_len = 4;
const char *keyword[] = {
    "return", "if", "else", "while"
};

bool startswith(char *str, char *tar) {
    int tar_len = strlen(tar);
    if(strlen(str) >= tar_len) {
        return strncmp(str, tar, tar_len) == 0;
    }
    return false;
}

bool iskeyword(char *str, int len) {
    for(int i=0; i<keyword_len; i++) {
        if(strlen(keyword[i]) == len && strncmp(str, keyword[i], len) == 0) {
            return true;
        }
    }
    return false;
}

Token *add_token(Token *cur, TokenKind kind, char *str, int len) {
    Token *new_token = calloc(1, sizeof(Token));
    new_token->kind = kind;
    new_token->len = len;
    new_token->str = str;
    cur->next = new_token;
    return new_token;
}

char *ident(char *p) {
    char *q = p; // base loc;
    while(isdigit(*p) || *p == '_' || isalpha(*p)) {
        p++;
    }
    return p;
}

void tokenizer(char *p) {
    Token head;
    Token *cur = &head;
    while(*p) {
        // ignore space
        if(isspace(*p)) {
            p++;
            continue;
        }
        // ident or keyword
        if(*p <= 'z' && *p >= 'a') {
            char *q = ident(p);
            if(iskeyword(p, q - p)) {
                cur = add_token(cur, TK_KEYWORD, p, q - p);
            } else{
                cur = add_token(cur, TK_IDENT, p, q - p);
            }
            p = q;
            continue;
        }

        // multi letter op
        if(startswith(p, ">=") || startswith(p, "<=") ||
           startswith(p, "==") || startswith(p, "!=")) {
            cur = add_token(cur, TK_SYMBOL, p, 2);
            p += 2;
            continue;
        }
        // single letter op
        if(strchr("+-*/()<>=;", *p)) {
            cur = add_token(cur, TK_SYMBOL, p, 1);
            p++;
            continue;
        }
        // num
        if(isdigit(*p)) {
            cur = add_token(cur, TK_NUM, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
        }
    }
    add_token(cur, TK_EOF, p, 0);
    cur_token = head.next;
}