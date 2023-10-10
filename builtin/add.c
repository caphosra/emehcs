#include "builtin.h"

#include "evaluation.h"
#include "utils/utils.h"
#include "utils/error.h"

Value* builtin_add_internal(Value** values) {
    Value* left = values[0];
    Value* right = values[1];
    if (!left || !right) {
        REPORT_ERR("The function + needs more arguments.");
    }
    if (left->type != V_NUM || right->type != V_NUM) {
        REPORT_ERR("The arguments of + should be numerics.");
    }
    if (values[2]) {
        REPORT_ERR("The number of arguments passed to + should be 2.");
    }

    ALLOC(val, Value);
    val->type = V_NUM;
    val->num = left->num + right->num;
    return val;
}

Value* get_builtin_add() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate = *builtin_add_internal;
    return val;
}
