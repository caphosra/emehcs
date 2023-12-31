#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

typedef struct _LambdaInternalData LambdaInternalData;

struct _LambdaInternalData {
    int var_num;
    char* vars[LAMBDA_ARG_LENGTH_MAX + 1];
    Environment captured;
    Expr* body;
};

Value* builtin_lambda_body_internal(Environment* env, Expr** args, void* internal) {
    LambdaInternalData* data = (LambdaInternalData*)internal;

    int captured_var_num = 0;
    EnvironmentSet* current = data->captured.current;
    while (current) {
        put_variable(env, current->name, current->value);
        captured_var_num++;
        current = current->prev;
    }

    VALIDATE_ARGS_NUM("<lambda>", args, data->var_num);

    for (int i = 0; i < data->var_num; i++) {
        // TODO: The environemnt should be copied.
        Value* val = evaluate(env, args[i]);
        put_variable(env, data->vars[i], val);
    }

    Value* ret = evaluate(env, data->body);

    for (int i = 0; i < data->var_num + captured_var_num; i++) {
        pop_variable(env);
    }

    return ret;
}

Value* builtin_lambda_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("lambda", args, 2);

    ALLOC(internal_data, LambdaInternalData);

    Expr* arguments = args[0];
    internal_data->body = args[1];

    ASSERT(arguments->type == E_APP, "The arguments of lambda should be enclosed by parentheses.");

    for (int i = 0; i < LAMBDA_ARG_LENGTH_MAX + 1; i++) {
        Expr* arg = arguments->args[i];
        if (!arg) {
            internal_data->var_num = i;
            break;
        }
        if (arg->type != E_VAR) {
            REPORT_ERR("The arguments of lambda should be named.");
        }
        internal_data->vars[i] = arg->var_name;
    }

    copy_env(&internal_data->captured, env);

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
