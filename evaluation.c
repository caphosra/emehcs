#include "evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* evaluate(Environment* env, Expr* expr) {
    switch (expr->type) {
        case E_NUM: {
            ALLOC(val, Value);
            val->type = V_NUM;
            val->num = expr->num;
            return val;
        }
        case E_STRING: {
            ALLOC(val, Value);
            val->type = V_STRING;
            val->text = expr->text;
            return val;
        }
        case E_VAR: {
            Value* val = find_variable(env, expr->var_name);
            if (!val) {
                REPORT_ERR("The variable is not found.");
            }

            return val;
        }
        case E_APP: {
            Value* func = evaluate(env, expr->args[0]);

            if (func->type != V_FUNCTION) {
                REPORT_ERR("Cannot apply a value to the other value which is not a function.");
            }

            return (*func->evaluate_func)(env, expr->args + 1, func->internal);
        }
        case E_BOOL: {
            ALLOC(val, Value);
            val->type = V_BOOL;
            val->num = expr->num;
            return val;
        }
    }
}

void setup_builtin(Environment* env) {
    put_variable(env, "+", get_builtin_add());
    put_variable(env, "define", get_builtin_define());
    put_variable(env, "lambda", get_builtin_lambda());
}

void print_value(Value* value) {
    switch (value->type) {
        case V_NUM: {
            printf("%d\n", value->num);
            break;
        }
        case V_STRING: {
            printf("\"%s\"\n", value->text);
            break;
        }
        case V_FUNCTION: {
            printf("<func>\n");
            break;
        }
        case V_BOOL: {
            if (value->num) {
                printf("#t\n");
            } else {
                printf("#f\n");
            }
            break;
        }
    }
}

int count_arguments(Expr** exprs) {
    for (int i = 0; i <= ARG_LENGTH_MAX; i++) {
        if (!exprs[i]) return i;
    }
    REPORT_ERR("An overflow detected on arguments.");
}
