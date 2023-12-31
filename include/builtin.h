#pragma once

#include "evaluation.h"

#define BUILTIN(var_name, ident) \
    { put_variable(env, var_name, (get_builtin_##ident)()); }

void setup_builtin(Environment* env);

Value* get_builtin_add();

Value* get_builtin_branch();

Value* get_builtin_eq();
Value* get_builtin_gt();
Value* get_builtin_gte();
Value* get_builtin_lt();
Value* get_builtin_lte();
Value* get_builtin_is_equal();

Value* get_builtin_define();

Value* get_builtin_display();

Value* get_builtin_tail_rec();

Value* get_builtin_lambda();

Value* get_builtin_let();
Value* get_builtin_let_ex();

Value* get_builtin_is_list();
Value* get_builtin_is_null();
Value* get_builtin_list();

Value* get_builtin_minus();

Value* get_builtin_mul();

Value* get_builtin_cons();
Value* get_builtin_car();
Value* get_builtin_cdr();

Value* get_builtin_quote();
