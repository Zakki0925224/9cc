#include "9cc.h"

static int depth;

static void push(void)
{
    printf("    push rax\n");
    depth++;
}

static void pop(char *arg)
{
    printf("    pop %s\n", arg);
    depth--;
}

static void gen_expr(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("    mov rax, %d\n", node->val);
        return;

    case ND_NEG:
        gen_expr(node->lhs);
        printf("    neg rax\n");
        return;
    }

    gen_expr(node->rhs);
    push();
    gen_expr(node->lhs);
    pop("rdi");

    switch (node->kind)
    {
    case ND_ADD:
        printf("    add rax, rdi\n");
        return;

    case ND_SUB:
        printf("    sub rax, rdi\n");
        return;

    case ND_MUL:
        printf("    imul rax, rdi\n");
        return;

    case ND_DIV:
        printf("    cqo\n");
        printf("    idiv rdi\n");
        return;

    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
        printf("    cmp rax, rdi\n");

        if (node->kind == ND_EQ)
            printf("    sete al\n");

        else if (node->kind == ND_NE)
            printf("    setne al\n");

        else if (node->kind == ND_LT)
            printf("    setl al\n");

        else if (node->kind == ND_LE)
            printf("    setle al\n");

        printf("    movzb rax, al\n");
        return;
    }

    error("invalid expression");
}

void codegen(Node *node)
{
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen_expr(node);
    printf("    ret\n");

    assert(depth == 0);
}