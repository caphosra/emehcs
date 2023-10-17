#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_let_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("let", args, 2);

    Expr* arguments = args[0];
    Expr* body = args[1];

    ASSERT(arguments->type == E_APP, "A variables to be defined should be enclosed with parentheses.");

    ALLOC(old_env, Environment);
    copy_env(old_env, env);

    int arg_num = 0;
    while (arguments->args[arg_num]) {
        Expr* expr = arguments->args[arg_num];

        ASSERT(expr->type == E_APP, "A variable should be defined with a name.");
        ASSERT(expr->args[1], "A variable should be defined with a name.");
        ASSERT(!expr->args[2], "A variable cannot have more than one value.");
        ASSERT(expr->args[0]->type == E_VAR, "The name of a variable is invalid.");

        char* name = expr->args[0]->var_name;
        Value* val = evaluate(old_env, expr->args[1]);
        put_variable(env, name, val);

        arg_num++;
    }

    free(old_env);

    Value* result = evaluate(env, body);

    for (int i = 0; i < arg_num; i++)
        pop_variable(env);

    return result;
}

Value* get_builtin_let() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_let_internal;
    return val;
}
