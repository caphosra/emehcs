#include "parser.h"

#include "tokenizer.h"
#include "utils/utils.h"
#include "utils/error.h"

inline struct Token* look_token(ExtArr* arr) {
    return (struct Token*)ext_arr_get_r_ptr(arr);
}

struct Expr* parse_expr(ExtArr* arr) {
    if (ext_arr_is_empty(arr)) {
        REPORT_ERR("There is no expression which are constructed with none of tokens.");
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
                    REPORT_ERR("The number of arguments is too large.");
                }
                expr->args[pos] = parse_expr(arr);
                pos++;
            }
            ext_arr_consume(arr);
            return expr;
        }
        case T_RIGHT_PAREN: {
            REPORT_ERR("There is no expression which starts with \")\".");
        }
        case T_IDENT: {
            if (!strcmp(token->text, "#t")) {
                ALLOC(expr, struct Expr);
                expr->type = E_BOOL;
                expr->num = 1;

                ext_arr_consume(arr);
                return expr;
            }
            else if (!strcmp(token->text, "#f")) {
                ALLOC(expr, struct Expr);
                expr->type = E_BOOL;
                expr->num = 0;

                ext_arr_consume(arr);
                return expr;
            }
            else {
                ALLOC(expr, struct Expr);
                expr->type = E_VAR;
                expr->var_name = token->text;

                ext_arr_consume(arr);
                return expr;
            }
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
        case E_BOOL:
            if (expr->num) {
                printf("E_BOOL(#t)");
            }
            else {
                printf("E_BOOL(#f)");
            }
            break;
    }
}

void print_expr(struct Expr* expr) {
    print_expr_internal(expr);
    printf("\n");
}
