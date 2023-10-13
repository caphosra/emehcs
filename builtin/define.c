#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Expr* generate_lambda_term(Expr** args, Expr* body) {
    ALLOC(lambda_ident, Expr);
    lambda_ident->type = E_VAR;
    lambda_ident->var_name = "lambda";

    ALLOC(lambda_args, Expr);
    lambda_args->type = E_APP;
    for (int i = 0; i < ARG_LENGTH_MAX; i++) {
        lambda_args->args[i] = args[i];
    }

    ALLOC(lambda_term, Expr);
    lambda_term->type = E_APP;
    lambda_term->args[0] = lambda_ident;
    lambda_term->args[1] = lambda_args;
    lambda_term->args[2] = body;

    return lambda_term;
}

Value* builtin_define_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("define", args, 2);

    Expr* arguments = args[0];
    Expr* body = args[1];

    if (arguments->type == E_VAR) {
        Value* val = evaluate(env, body);
        put_variable(env, arguments->var_name, val);
    }
    else if (arguments->type == E_APP) {
        Expr* func = arguments->args[0];

        ASSERT(func->type == E_VAR, "The name of the function to be defined is invalid.");

        Expr* lambda_term = generate_lambda_term(arguments->args + 1, body);
        Value* val = evaluate(env, lambda_term);
        put_variable(env, func->var_name, val);
    }
    else REPORT_ERR("The name of the function to be defined is invalid.");

    ALLOC(val, Value);
    val->type = V_NUM;
    val->num = 0;
    return val;
}

Value* get_builtin_define() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_define_internal;
    return val;
}
