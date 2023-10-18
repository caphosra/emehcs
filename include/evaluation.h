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
    V_BOOL,
    V_PAIR,
    V_NIL_PAIR,
    V_IDENT,
    V_UNDEFINED
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
        char* ident;
        struct {
            Value* left;
            Value* right;
        };
    };
};

extern const Value CONST_NIL_PAIR_OBJ;
extern const Value CONST_TRUE_OBJ;
extern const Value CONST_FALSE_OBJ;
extern const Value CONST_UNDEFINED_OBJ;

#define CONST_NIL_PAIR ((Value*)(&CONST_NIL_PAIR_OBJ))
#define CONST_TRUE ((Value*)(&CONST_TRUE_OBJ))
#define CONST_FALSE ((Value*)(&CONST_FALSE_OBJ))
#define CONST_UNDEFINED ((Value*)(&CONST_UNDEFINED_OBJ))

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
void copy_env(Environment* dist, Environment* src);

Value* evaluate(Environment* env, Expr* expr);
void print_value(Value* value);

int count_arguments(Expr** exprs);

#define VALIDATE_ARGS_NUM(func_name, exprs, num) \
    if (count_arguments(exprs) != num)           \
        REPORT_ERR("The function %.10s needs just %d argument(s).", func_name, num);
