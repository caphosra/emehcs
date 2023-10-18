#include "evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "utils/error.h"
#include "utils/utils.h"

const Value CONST_NIL_PAIR_OBJ = { .type = V_NIL_PAIR, .num = 0 };
const Value CONST_TRUE_OBJ = { .type = V_BOOL, .num = 1 };
const Value CONST_FALSE_OBJ = { .type = V_BOOL, .num = 0 };
const Value CONST_UNDEFINED_OBJ = { .type = V_UNDEFINED, .num = 0 };
const Value CONST_NULL_OBJ = { .type = V_NULL, .num = 0 };

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
                REPORT_ERR("The variable %.10s is not found.", expr->var_name);
            }

            return val;
        }
        case E_APP: {
            ASSERT(expr->args[0], "A function to be executed is required.");

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

int is_list(Value* value) {
    Value* current = value;
    while (1) {
        if (current->type == V_NIL_PAIR) return 1;
        if (current->type != V_PAIR) return 0;
        current = current->right;
    }
}

void print_value_internal(Value* value) {
    switch (value->type) {
        case V_NUM: {
            printf("%d", value->num);
            break;
        }
        case V_STRING: {
            printf("\"%s\"", value->text);
            break;
        }
        case V_FUNCTION: {
            printf("<func>");
            break;
        }
        case V_BOOL: {
            if (value->num) {
                printf("#t");
            } else {
                printf("#f");
            }
            break;
        }
        case V_PAIR: {
            if (is_list(value)) {
                printf("(");
                int fst = 1;
                Value* current = value;
                while (current->type != V_NIL_PAIR) {
                    if (fst) fst = 0;
                    else printf(" ");

                    print_value(current->left);

                    current = current->right;
                }
                printf(")");
                break;
            }
            else {
                printf("(");
                print_value(value->left);
                printf(" . ");
                print_value(value->right);
                printf(")");
                break;
            }
        }
        case V_NIL_PAIR: {
            printf("()");
            break;
        }
        case V_IDENT: {
            printf("%s", value->ident);
            break;
        }
        case V_UNDEFINED: {
            printf("#undefined");
            break;
        }
        case V_NULL:
            break;
    }
}

void print_value(Value* value) {
    print_value_internal(value);
    if (value->type != V_NULL) printf("\n");
}

int count_arguments(Expr** exprs) {
    for (int i = 0; i <= ARG_LENGTH_MAX; i++) {
        if (!exprs[i]) return i;
    }
    REPORT_ERR("An overflow detected on arguments.");
}
