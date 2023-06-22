#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node Node;

// tokenize.c
typedef enum
{
    TK_IDENT,
    TK_PUNCT,
    TK_NUM,
    TK_KEYWORD,
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token
{
    TokenKind kind;
    Token *next;
    int val;
    char *loc;
    int len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
Token *tokenize(char *input);

// parse.c
typedef struct Obj Obj;
struct Obj
{
    Obj *next;
    char *name;
    int offset;
};

typedef struct Function Function;
struct Function
{
    Node *body;
    Obj *locals;
    int stack_size;
};

typedef enum
{
    ND_ADD,       // +
    ND_SUB,       // -
    ND_MUL,       // *
    ND_DIV,       // /
    ND_NEG,       // unary -
    ND_EQ,        // ==
    ND_NE,        // !=
    ND_LT,        // <
    ND_LE,        // <=
    ND_ASSIGN,    // =
    ND_EXPR_STMT, // Expression statement
    ND_VAR,       // Variable
    ND_NUM,       // Integer
    ND_RETURN,    // "return"
    ND_BLOCK,     // { ... }
} NodeKind;

struct Node
{
    NodeKind kind;
    Node *next;
    Node *lhs;
    Node *rhs;
    Node *body; // block
    Obj *var;
    int val;
};

Function *parse(Token *tok);

// codegen.c
void codegen(Function *prog);