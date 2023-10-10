#include "evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/utils.h"
#include "builtin.h"

void put_variable(struct Environment* env, char* name, struct Value* value) {
    ALLOC(set, struct EnvironmentSet);
    set->name = name;
    set->value = value;

    set->prev = env->current;
    env->current = set;
}

void pop_variable(struct Environment* env) {
    struct EnvironmentSet* trash = env->current;
    env->current = env->current->prev;
    free(trash);
}

struct Value* find_variable(struct Environment* env, char* name) {
    struct EnvironmentSet* current = env->current;
    while (current) {
        if (!strcmp(current->name, name)) {
            return current->value;
        }
        current = current->prev;
    }
    return NULL;
}

struct Value* evaluate(struct Environment* env, struct Expr* expr) {
    switch (expr->type) {
        case E_NUM: {
            ALLOC(val, struct Value);
            val->type = V_NUM;
            val->num = expr->num;
            return val;
        }
        case E_STRING: {
            ALLOC(val, struct Value);
            val->type = V_STRING;
            val->text = expr->text;
            return val;
        }
        case E_VAR: {
            struct Value* val = find_variable(env, expr->var_name);
            if (!val) {
                REPORT_ERR("The variable is not found.");
            }

            return val;
        }
        case E_APP: {
            struct Value* func = evaluate(env, expr->func);
            struct Value** args = (struct Value**)calloc(ARG_LENGTH_MAX, sizeof(struct Value*));
            for (int i = 0; i < ARG_LENGTH_MAX; i++) {
                if (expr->args[i]) {
                    // TODO: The environment should be copied.
                    args[i] = evaluate(env, expr->args[i]);
                }
                else break;
            }

            if (func->type != V_FUNCTION) {
                REPORT_ERR("Cannot apply a value to the other value which is not a function.");
            }

            return (*func->evaluate)(args);
        }
        default: {
            return NULL;
        }
    }
}

void setup_builtin(struct Environment* env) {
    put_variable(env, "+", get_builtin_add());
}

void print_value(struct Value* value) {
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
    }
}
