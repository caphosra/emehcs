#include "evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "utils/error.h"
#include "utils/utils.h"

void put_variable(Environment* env, char* name, Value* value) {
    ALLOC(set, EnvironmentSet);
    set->name = name;
    set->value = value;

    set->prev = env->current;
    env->current = set;
}

void pop_variable(Environment* env) {
    EnvironmentSet* trash = env->current;
    env->current = env->current->prev;
    free(trash);
}

Value* find_variable(Environment* env, char* name) {
    EnvironmentSet* current = env->current;
    while (current) {
        if (!strcmp(current->name, name)) {
            return current->value;
        }
        current = current->prev;
    }
    return NULL;
}

Value* evaluate(Environment* env, struct Expr* expr) {
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
            Value* func = evaluate(env, expr->func);
            Value** args = (Value**)calloc(ARG_LENGTH_MAX, sizeof(Value*));
            for (int i = 0; i < ARG_LENGTH_MAX; i++) {
                if (expr->args[i]) {
                    // TODO: The environment should be copied.
                    args[i] = evaluate(env, expr->args[i]);
                } else
                    break;
            }

            if (func->type != V_FUNCTION) {
                REPORT_ERR("Cannot apply a value to the other value which is not a function.");
            }

            return (*func->evaluate)(args);
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
