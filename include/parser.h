#pragma once

#include "tokenizer.h"
#include "utils/extarr.h"

typedef enum _ExprType ExprType;
typedef struct _Expr Expr;

enum _ExprType {
    E_APP,
    E_VAR,
    E_NUM,
    E_STRING,
    E_BOOL
};

#define ARG_LENGTH_MAX 0x80 - 1
#define LAMBDA_ARG_LENGTH_MAX 0x40 - 1

struct _Expr {
    ExprType type;
    union {
        struct {
            Expr* func;
            Expr* args[ARG_LENGTH_MAX + 1];
        };
        char* var_name;
        int num;
        char* text;
    };
};

Token* look_token(ExtArr* arr);
Expr* parse_expr(ExtArr* arr);
void print_expr(Expr* expr);
