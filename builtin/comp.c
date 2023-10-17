#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_lt_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("<", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_BOOL;
    val->num = left->num < right->num;
    return val;
}

Value* get_builtin_lt() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_lt_internal;
    return val;
}

Value* builtin_gt_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM(">", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_BOOL;
    val->num = left->num > right->num;
    return val;
}

Value* get_builtin_gt() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_gt_internal;
    return val;
}

Value* builtin_lte_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("<=", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_BOOL;
    val->num = left->num <= right->num;
    return val;
}

Value* get_builtin_lte() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_lte_internal;
    return val;
}

Value* builtin_gte_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM(">=", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_BOOL;
    val->num = left->num >= right->num;
    return val;
}

Value* get_builtin_gte() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_gte_internal;
    return val;
}

Value* builtin_eq_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("=", args, 2);

    Value* left = evaluate(env, args[0]);
    Value* right = evaluate(env, args[1]);

    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }

    ALLOC(val, Value);
    val->type = V_BOOL;
    val->num = left->num == right->num;
    return val;
}

Value* get_builtin_eq() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_eq_internal;
    return val;
}
