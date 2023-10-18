#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* quote(Environment* env, Expr* expr) {
    switch (expr->type) {
        case E_APP: {
            if (!expr->args[0]) {
                return CONST_NIL_PAIR;
            }
            ALLOC(val, Value);
            val->type = V_PAIR;
            val->left = quote(env, expr->args[0]);

            int arg_num = 1;
            Value** parent_right = &val->right;

            while (expr->args[arg_num]) {
                ALLOC(val, Value);
                val->type = V_PAIR;
                val->left = quote(env, expr->args[arg_num]);

                *parent_right = val;
                parent_right = &val->right;

                arg_num++;
            }

            *parent_right = CONST_NIL_PAIR;
            return val;
        }
        case E_VAR: {
            ALLOC(val, Value);
            val->type = V_IDENT;
            val->ident = expr->var_name;
            return val;
        }
        default: {
            Value* val = evaluate(env, expr);
            return val;
        }
    }
}

Value* builtin_quote_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("quote", args, 1);

    return quote(env, args[0]);
}

Value* get_builtin_quote() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_quote_internal;
    return val;
}
