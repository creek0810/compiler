#include "9cc.h"
// global


/* help function */
void print(Token *cur) {
    while(cur) {
        switch(cur->kind) {
            case TK_NUM:
                printf("NUM: %d %d %s\n", cur->len, cur->val, cur->str);
                break;
            case TK_EOF:
                printf("EOF: %d %d %s\n", cur->len, cur->val, cur->str);
                break;
            case TK_SYMBOL:
                printf("SYMBOL: %d %d %s\n", cur->len, cur->val, cur->str);
                break;
        }
        cur = cur->next;
    }
}
void print_node(Node *cur) {
    switch(cur->kind) {
        case ND_NUM:
            printf("NUM: %d\n", cur->val);
            break;
        case ND_ADD:
            printf("ADD\n");
            break;
        case ND_SUB:
            printf("SUB\n");
            break;
        case ND_DIV:
            printf("DIV\n");
            break;
        case ND_MUL:
            printf("MUL\n");
            break;
        case ND_EQ:
            printf("EQ\n");
            break;
        case ND_NE:
            printf("NE\n");
            break;
        case ND_LE:
            printf("LE\n");
            break;
        case ND_LT:
            printf("LT\n");
            break;
        default:
            printf("wrong type\n");
    }
    printf("%d %d\n", cur->lhs, cur->rhs);

}
void print_tree(Node *cur) {
    if(cur->kind == ND_NUM) {
        print_node(cur);
        return;
    }
    print_tree(cur->lhs);
    print_tree(cur->rhs);
    print_node(cur);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("參數錯誤\n");
    }

    // token
    cur_token = tokenizer(argv[1]);
    // print(cur_token);

    // parse
    Node *node = parser();
    // printf("start print tree:------------\n");
    // print_tree(node);

    // init asm
    printf(".intel_syntax noprefix\n");
    printf(".global _main\n");
    printf("_main:\n");
    // start gen
    gen(node);
    printf("  pop rax\n");
    printf("  ret\n");
}