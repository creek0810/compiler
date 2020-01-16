#include "9cc.h"
// help function
bool consume(char *op) {
    if(cur_token->kind == TK_SYMBOL && cur_token->len == strlen(op) && strncmp(cur_token->str, op, cur_token->len) == 0) {
        cur_token = cur_token->next;
        return true;
    }
    return false;
}

bool consume_keyword(char *keyword) {
    if(cur_token->kind == TK_KEYWORD &&
       cur_token->len == strlen(keyword) &&
       strncmp(cur_token->str, keyword, cur_token->len) == 0
    ) {
        cur_token = cur_token->next;
        return true;
    }
    return false;
}

Node *add_node(NodeKind kind, Node *lhs, Node *rhs, int val){
    Node *cur_node = calloc(1, sizeof(Node));
    cur_node->kind = kind;
    cur_node->lhs = lhs;
    cur_node->rhs = rhs;
    cur_node->val = val;
    return cur_node;
}

Lvar *find_lvar(Token *tok) {
    Lvar *cur = locals;
    while(cur) {
        if(tok->len == cur->len &&
            strncmp(tok->str, cur->str, cur->len) == 0
        ) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

Lvar *add_lvar(Token *tok) {
    Lvar *cur_lvar = calloc(1, sizeof(Lvar));
    cur_lvar->len = tok->len;
    cur_lvar->str = tok->str;
    cur_lvar->next = locals;
    // check if locals is not null
    cur_lvar->offset = (locals) ? locals->offset + 8 : 8;
    locals = cur_lvar;
    return cur_lvar;
}

// parse
Node *primary() {
    if(cur_token->kind == TK_NUM) {
        Node *new_node = add_node(ND_NUM, NULL, NULL, cur_token->val);
        cur_token = cur_token->next;
        return new_node;
    }
    if(cur_token->kind == TK_IDENT) {
        Node *new_node = add_node(ND_LVAR, NULL, NULL, 0);
        // calc offset
        Lvar *lvar = find_lvar(cur_token);
        if(lvar) {
            new_node->offset = lvar->offset;
        } else {
            Lvar *new_lvar = add_lvar(cur_token);
            new_node->offset = new_lvar->offset;
        }
        cur_token = cur_token->next;
        return new_node;
    }
    if(consume("(")) {
        Node *new_node = expr();
        consume(")");
        return new_node;
    }
}

Node *unary() {
    if(consume("+")) {
        return primary();
    }
    if(consume("-")) {
        Node *zero_node = add_node(ND_NUM, NULL, NULL, 0);
        return add_node(ND_SUB, zero_node, primary(), 0);
    }
    return primary();
}

Node *mul() {
    Node *node = unary();
    for(;;) {
        if(consume("*")) {
            node = add_node(ND_MUL, node, unary(), 0);
            continue;
        }
        if(consume("/")) {
            node = add_node(ND_DIV, node, unary(), 0);
            continue;
        }
        return node;
    }
}

Node *add() {
    Node *node = mul();
    for(;;){
        if(consume("+")) {
            node = add_node(ND_ADD, node, mul(), 0);
            continue;
        }
        if(consume("-")) {
            node = add_node(ND_SUB, node, mul(), 0);
            continue;
        }
        return node;
    }
}

Node *relational() {
    Node *node = add();
    for(;;) {
        if(consume("<=")) {
            node = add_node(ND_LE, node, add(), 0);
            continue;
        }
        if(consume(">=")) {
            node = add_node(ND_LE, add(), node, 0);
            continue;
        }
        if(consume("<")) {
            node = add_node(ND_LT, node, add(), 0);
            continue;
        }
        if(consume(">")) {
            node = add_node(ND_LT, add(), node, 0);
            continue;
        }
        return node;
    }


}

Node *equality() {
    Node *node = relational();
    for(;;) {
        if(consume("==")) {
            node = add_node(ND_EQ, node, relational(), 0);
            continue;
        }
        if(consume("!=")) {
            node = add_node(ND_NE, node, relational(), 0);
            continue;
        }
        return node;
    }
}

Node *assign() {
    Node *node = equality();
    if(consume("=")) {
        return add_node(ND_ASSIGN, node, assign(), 0);
    }
    return node;
}


Node *expr() {
    return assign();
}

Node *stmt() {
    Node *node;
    if(consume_keyword("return")){
        node = expr();
        node = add_node(ND_RETURN, node, NULL, 0);
        consume(";");
    } else {
        node = expr();
        consume(";");
    }
    // printf("%d %d %d %d\n", node, node->lhs, node->rhs, node->val);
    return node;
}

// need to check
void program() {
    int i = 0;
    while(cur_token->kind != TK_EOF) {
        code[i++] = stmt();
    }
    code[i] = NULL;
}

void parser() {
    return program();
}