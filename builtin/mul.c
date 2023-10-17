#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_mul_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("*", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_NUM;
    val->num = left->num * right->num;
    return val;
}

Value* get_builtin_mul() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_mul_internal;
    return val;
}
