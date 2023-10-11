#pragma once

#include "parser.h"
#include "utils/error.h"

typedef enum _ValueType ValueType;
typedef struct _Value Value;
typedef struct _EnvironmentSet EnvironmentSet;
typedef struct _Environment Environment;

enum _ValueType {
    V_FUNCTION,
    V_NUM,
    V_STRING,
    V_BOOL
};

struct _Value {
    ValueType type;
    union {
        struct {
            Value* (*evaluate_func)(Environment* env, Expr** args, void* internal);
            void* internal;
        };
        int num;
        char* text;
    };
};

struct _EnvironmentSet {
    EnvironmentSet* prev;
    char* name;
    Value* value;
};

struct _Environment {
    EnvironmentSet* current;
};

void put_variable(Environment* env, char* name, Value* value);
void pop_variable(Environment* env);
Value* find_variable(Environment* env, char* name);

Value* evaluate(Environment* env, Expr* expr);
void setup_builtin(Environment* env);
void print_value(Value* value);

int count_arguments(Expr** exprs);

#define VALIDATE_ARGS_NUM(func_name, exprs, num) \
    if (count_arguments(exprs) != num)           \
        REPORT_ERR("The function %.10s needs just %d argument(s).", func_name, num);
