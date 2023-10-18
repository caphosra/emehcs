#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_tail_rec_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("__tail_rec", args, 2);

    Expr* func_args = args[0];
    Expr* body = args[1];

    ASSERT(func_args->type == E_APP, "The tail recursion is invalid.");
    ASSERT(func_args->args[0] && func_args->args[0]->type == E_VAR, "The tail recursion is invalid.");

    char* func_name = func_args->args[0]->var_name;
    int arg_num = 0;
    char** func_arg_names = calloc(ARG_LENGTH_MAX, sizeof(char*));
    while (func_args->args[arg_num + 1]) {
        Expr* arg = func_args->args[arg_num + 1];

        ASSERT(arg && arg->type == E_VAR, "The tail recursion is invalid.");

        func_arg_names[arg_num] = arg->var_name;
        arg_num++;
    }

    Expr* current = body;
    while (1) {
        if (current->type != E_APP) return evaluate(env, current);

        Expr* f = current->args[0];
        if (!f || f->type != E_VAR) return evaluate(env, current);

        if (!strcmp(f->var_name, "if")) {
            VALIDATE_ARGS_NUM("if", current->args + 1, 3);

            Value* cond = evaluate(env, current->args[1]);

            ASSERT(cond->type == V_BOOL, "A \"if\" must returns a boolean.");

            if (cond->num) current = current->args[2];
            else current = current->args[3];
        }
        else if (!strcmp(f->var_name, func_name)) {
            VALIDATE_ARGS_NUM(func_name, current->args + 1, arg_num);

            ALLOC(old_env, Environment);
            copy_env(old_env, env);

            for (int i = 0; i < arg_num; i++)
                pop_variable(env);

            for (int i = 0; i < arg_num; i++) {
                Value* new_arg = evaluate(old_env, current->args[i + 1]);
                put_variable(env, func_arg_names[i], new_arg);
            }
            current = body;
        }
        else {
            return evaluate(env, current);
        }
    }
}

Value* get_builtin_tail_rec() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_tail_rec_internal;
    return val;
}
