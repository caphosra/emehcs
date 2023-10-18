#include "optimizer.h"

#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

int contains_tail_recursion(char* func_name, Expr* func_body) {
    if (func_body->type != E_APP) return 0;
    if (!func_body->args[0] || func_body->args[0]->type != E_VAR) return 0;
    char* name = func_body->args[0]->var_name;
    if (!strcmp(name, "if")) {
        if (!func_body->args[3]) return 0;
        int true_side = contains_tail_recursion(func_name, func_body->args[2]);
        int false_side = contains_tail_recursion(func_name, func_body->args[3]);
        return true_side || false_side;
    }
    else if (!strcmp(name, func_name)) return 1;
    else return 0;
}

Expr* optimize_tail_recursion(Expr* expr) {
    if (expr->type != E_APP || !expr->args[2] || expr->args[3]) return expr;

    Expr* name = expr->args[0];
    Expr* definition = expr->args[1];
    Expr* body = expr->args[2];

    if (name->type != E_VAR || strcmp(name->var_name, "define")) return expr;
    if (definition->type != E_APP || definition->args[0]->type != E_VAR) return expr;

    char* func_name = definition->args[0]->var_name;
    if (contains_tail_recursion(func_name, body)) {
        printf("A tail recursion detected.\n");

        ALLOC(tail_recursion_name, Expr);
        tail_recursion_name->type = E_VAR;
        tail_recursion_name->var_name = "__tail_rec";

        ALLOC(optimized, Expr);
        optimized->type = E_APP;
        optimized->args[0] = tail_recursion_name;
        optimized->args[1] = definition;
        optimized->args[2] = body;

        expr->args[2] = optimized;
        return expr;
    }
    else return expr;
}
