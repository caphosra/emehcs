#include "builtin.h"

#include "evaluation.h"
#include "utils/utils.h"
#include "utils/error.h"

Value* builtin_add_internal(Environment* env, struct Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("+", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_NUM;
    val->num = left->num + right->num;
    return val;
}

Value* get_builtin_add() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_add_internal;
    return val;
}
