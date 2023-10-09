#include "parser.h"

#include "tokenizer.h"
#include "utils/utils.h"

inline struct Token* look_token(struct ExtArr* arr) {
    return (struct Token*)ext_arr_get_r_ptr(arr);
}

struct Expr* parse_expr(struct ExtArr* arr) {
    if (ext_arr_is_empty(arr)) {
        exit(1);
    }
    struct Token* token = look_token(arr);
    switch (token->type) {
        case T_NUM: {
            ALLOC(expr, struct Expr);
            expr->type = E_NUM;
            expr->num = token->num;

            ext_arr_consume(arr);
            return expr;
        }
        case T_STRING: {
            ALLOC(expr, struct Expr);
            expr->type = E_STRING;
            expr->text = token->text;

            ext_arr_consume(arr);
            return expr;
        }
        case T_LEFT_PAREN: {
            ext_arr_consume(arr);

            ALLOC(expr, struct Expr);
            expr->type = E_APP;
            expr->func = parse_expr(arr);

            int pos = 0;
            while (look_token(arr)->type != T_RIGHT_PAREN) {
                if (pos == ARG_LENGTH_MAX) {
                    exit(1);
                }
                expr->args[pos] = parse_expr(arr);
                pos++;
            }
            ext_arr_consume(arr);
            return expr;
        }
        case T_RIGHT_PAREN: {
            exit(1);
            break;
        }
        case T_IDENT: {
            ALLOC(expr, struct Expr);
            expr->type = E_VAR;
            expr->var_name = token->text;

            ext_arr_consume(arr);
            return expr;
        }
    }
}

void print_expr_internal(struct Expr* expr) {
    switch (expr->type) {
        case E_APP:
            printf("E_APP");
            printf("(");
            print_expr_internal(expr->func);
            int pos = 0;
            while (expr->args[pos]) {
                printf(", ");
                print_expr_internal(expr->args[pos]);
                pos++;
            }
            printf(")");
            break;
        case E_VAR:
            printf("E_VAR(%s)", expr->var_name);
            break;
        case E_NUM:
            printf("E_NUM(%d)", expr->num);
            break;
        case E_STRING:
            printf("E_STRING(%s)", expr->text);
            break;
        default:
            break;
    }
}

void print_expr(struct Expr* expr) {
    print_expr_internal(expr);
    printf("\n");
}
