#include "builtin.h"

#include "evaluation.h"
#include "utils/utils.h"
#include "utils/error.h"

struct _LambdaInternalData {
    int var_num;
    char* vars[LAMBDA_ARG_LENGTH_MAX + 1];
    struct Expr* body;
};

Value* builtin_lambda_body_internal(Environment* env, struct Expr** args, void* internal) {
    struct _LambdaInternalData* data = (struct _LambdaInternalData*)internal;

    VALIDATE_ARGS_NUM("<lambda>", args, data->var_num);

    for (int i = 0; i < data->var_num; i++) {
        // TODO: The environemnt should be copied.
        Value* val = evaluate(env, args[i]);
        put_variable(env, data->vars[i], val);
    }

    Value* ret = evaluate(env, data->body);

    for (int i = 0; i < data->var_num; i++) {
        pop_variable(env);
    }

    return ret;
}

Value* builtin_lambda_internal(Environment* env, struct Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("lambda", args, 2);

    ALLOC(internal_data, struct _LambdaInternalData);

    struct Expr* arguments = args[0];
    internal_data->body = args[1];

    ASSERT(arguments->type == E_APP, "The arguments of lambda should be enclosed by parentheses.");

    for (int i = -1; i < LAMBDA_ARG_LENGTH_MAX - 1; i++) {
        struct Expr* arg;
        if (i == -1) {
            arg = arguments->func;
        }
        else {
            arg = arguments->args[i];
        }
        if (!arg) {
            internal_data->var_num = i + 1;
            break;
        }
        if (arg->type != E_VAR) {
            REPORT_ERR("The arguments of lambda should be named.");
        }
        internal_data->vars[i + 1] = arg->var_name;
    }

    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_lambda_body_internal;
    val->internal = internal_data;
    return val;
}

Value* get_builtin_lambda() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_lambda_internal;
    return val;
}
