#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_cons_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("cons", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    ALLOC(pair, Value);
    pair->type = V_PAIR;
    pair->left = left;
    pair->right = right;

    return pair;
}

Value* get_builtin_cons() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_cons_internal;
    return val;
}

Value* builtin_car_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("car", args, 1);

    Value* pair = evaluate(env, args[0]);
    ASSERT(pair->type == V_PAIR, "The function \"car\" accepts only a pair.");

    return pair->left;
}

Value* get_builtin_car() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_car_internal;
    return val;
}

Value* builtin_cdr_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("cdr", args, 1);

    Value* pair = evaluate(env, args[0]);
    ASSERT(pair->type == V_PAIR, "The function \"cdr\" accepts only a pair.");

    return pair->right;
}

Value* get_builtin_cdr() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_cdr_internal;
    return val;
}
