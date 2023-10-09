#include "utils/extarr.h"

enum ExprType {
    E_DEFINE,
    E_LAMBDA,
    E_APP,
    E_VAR,
    E_NUM,
    E_STRING
};

#define ARG_LENGTH_MAX 0x80 - 1

struct Expr {
    enum ExprType type;
    union {
        struct {
            char* def_name;
            struct Expr* def_body;
        };
        struct {
            char* vars;
            struct Expr* lambda_body;
        };
        struct {
            struct Expr* func;
            struct Expr* args[ARG_LENGTH_MAX + 1];
        };
        char* var_name;
        int num;
        char* text;
    };
};

struct Token* look_token(struct ExtArr* arr);
struct Expr* parse_expr(struct ExtArr* arr);
void print_expr(struct Expr* expr);
