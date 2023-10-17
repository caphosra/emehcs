#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_branch_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("if", args, 3);

    Value* cond = evaluate(env, args[0]);
    ASSERT(cond->type == V_BOOL, "The condition must be #t or #f.");
    if (cond->num) {
        return evaluate(env, args[1]);
    }
    else {
        return evaluate(env, args[2]);
    }
}

Value* get_builtin_branch() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_branch_internal;
    return val;
}
