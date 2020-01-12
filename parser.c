#include "9cc.h"

bool consume(char *op) {
    if(cur_token->kind == TK_SYMBOL && cur_token->len == strlen(op) && strncmp(cur_token->str, op, cur_token->len) == 0) {
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

Node *expr();

Node *term() {
    if(cur_token->kind == ND_NUM) {
        
        Node *new_node = add_node(ND_NUM, NULL, NULL, cur_token->val);
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
        return term();
    }
    if(consume("-")) {
        Node *zero_node = add_node(ND_NUM, NULL, NULL, 0);
        return add_node(ND_SUB, zero_node, term(), 0);
    }
    return term();
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
Node *expr() {
    return equality();
}
Node *parser() {
    return expr();
}