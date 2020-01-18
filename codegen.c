#include "9cc.h"
int IF_CNT = 0;
int WHILE_CNT = 0;

void gen_lvar(Node *node) {
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}


void gen(Node *cur) {
    int cur_idx = IF_CNT;
    int cur_while = WHILE_CNT;
    int idx = 0;
    switch(cur->kind) {
        case ND_BLOCK:
            while((cur->stmts)[idx] != NULL) {
                gen((cur->stmts)[idx]);
                idx ++;
            }
            return;
        case ND_NUM:
            printf("  push %d\n", cur->val);
            return;
        case ND_LVAR:
            gen_lvar(cur);
            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        case ND_ASSIGN:
            gen_lvar(cur->lhs);
            gen(cur->rhs);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], rdi\n");
            printf("  push rdi\n");
            return;
        case ND_RETURN:
            gen(cur->lhs);
            printf("  pop rax\n");
            printf("  mov rsp, rbp\n");
            printf("  pop rbp\n");
            printf("  ret\n");
            return;
        case ND_IF:
            IF_CNT++;
            gen(cur->condition);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            if(cur->else_statement != NULL) {
                printf("  je IF_ELSE_%d\n", cur_idx);
            } else {
                printf("  je IF_END_%d\n", cur_idx);
            }
            gen(cur->statements);
            printf("  jmp IF_END_%d\n", cur_idx);
            if(cur->else_statement != NULL) {
                printf("IF_ELSE_%d:\n", cur_idx);
                gen(cur->else_statement);
            }
            printf("IF_END_%d:\n", cur_idx);
            return;
        case ND_WHILE:
            WHILE_CNT++;
            printf("WHILE_START_%d:\n", cur_while);
            gen(cur->condition);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je WHILE_END_%d\n", cur_while);
            gen(cur->statements);
            printf("  jmp WHILE_START_%d\n", cur_while);
            printf("WHILE_END_%d:\n", cur_while);
            return;
    }
    gen(cur->lhs);
    gen(cur->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    switch(cur->kind) {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_EQ:
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzx rax, al\n");
            break;
        case ND_NE:
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzx rax, al\n");
            break;
        case ND_LE:
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzx rax, al\n");
            break;
        case ND_LT:
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzx rax, al\n");
            break;
    }
    printf("  push rax\n");
}