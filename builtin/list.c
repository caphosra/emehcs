#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_list_internal(Environment* env, Expr** args, void* internal) {
    if (!args[0]) {
        return &CONST_NIL_PAIR;
    }
    ALLOC(val, Value);
    val->type = V_PAIR;
    val->left = evaluate(env, args[0]);

    int arg_num = 1;
    Value** parent_right = &val->right;

    while (args[arg_num]) {
        ALLOC(val, Value);
        val->type = V_PAIR;
        val->left = evaluate(env, args[arg_num]);

        *parent_right = val;
        parent_right = &val->right;

        arg_num++;
    }

    *parent_right = &CONST_NIL_PAIR;
    return val;
}

Value* get_builtin_list() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_list_internal;
    return val;
}

Value* builtin_is_list_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("list?", args, 1);

    Value* val = evaluate(env, args[0]);
    if (val->type == V_PAIR) {
        return &CONST_TRUE;
    }
    else {
        return &CONST_FALSE;
    }
}

Value* get_builtin_is_list() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_is_list_internal;
    return val;
}
