#include "evaluation.h"

#include "utils/utils.h"

void put_variable(Environment* env, char* name, Value* value) {
    ALLOC(set, EnvironmentSet);
    set->name = name;
    set->value = value;

    set->prev = env->current;
    env->current = set;
}

void pop_variable(Environment* env) {
    EnvironmentSet* trash = env->current;
    env->current = env->current->prev;
}

Value* find_variable(Environment* env, char* name) {
    EnvironmentSet* current = env->current;
    while (current) {
        if (!strcmp(current->name, name)) {
            return current->value;
        }
        current = current->prev;
    }
    return NULL;
}

void copy_env(Environment* dist, Environment* src) {
    EnvironmentSet* current = src->current;
    while (current) {
        put_variable(dist, current->name, current->value);
        current = current->prev;
    }
}
