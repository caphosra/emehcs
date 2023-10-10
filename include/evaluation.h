#pragma once

#include "parser.h"

enum ValueType {
    V_FUNCTION,
    V_NUM,
    V_STRING
};

struct Value {
    enum ValueType type;
    union {
        struct Value* (*evaluate)(struct Value** values);
        int num;
        char* text;
    };
};

struct EnvironmentSet {
    struct EnvironmentSet* prev;
    char* name;
    struct Value* value;
};

struct Environment {
    struct EnvironmentSet* current;
};

void put_variable(struct Environment* env, char* name, struct Value* value);
void pop_variable(struct Environment* env);
struct Value* find_variable(struct Environment* env, char* name);

struct Value* evaluate(struct Environment* env, struct Expr* expr);
void setup_builtin(struct Environment* env);
void print_value(struct Value* value);
