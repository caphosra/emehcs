#pragma once

#include "parser.h"

typedef enum _ValueType ValueType;
enum _ValueType {
    V_FUNCTION,
    V_NUM,
    V_STRING,
    V_BOOL
};

typedef struct _Value Value;
struct _Value {
    ValueType type;
    union {
        Value* (*evaluate)(Value** values);
        int num;
        char* text;
    };
};

typedef struct _EnvironmentSet EnvironmentSet;
struct _EnvironmentSet {
    EnvironmentSet* prev;
    char* name;
    Value* value;
};

typedef struct _Environment Environment;
struct _Environment {
    EnvironmentSet* current;
};

void put_variable(Environment* env, char* name, Value* value);
void pop_variable(Environment* env);
Value* find_variable(Environment* env, char* name);

Value* evaluate(Environment* env, struct Expr* expr);
void setup_builtin(Environment* env);
void print_value(Value* value);
