#include "builtin.h"
#include "evaluation.h"
#include "utils/error.h"
#include "utils/utils.h"

Value* builtin_display_internal(Environment* env, Expr** args, void* internal) {
    VALIDATE_ARGS_NUM("display", args, 1);

    Value* val = evaluate(env, args[0]);

    if (val->type == V_STRING) {
        int i = 0;
        while (val->text[i]) {
            if (val->text[i] == '\\') {
                i++;
                if (val->text[i] == '\\') {
                    printf("\\");
                }
                else if (val->text[i] == 'n') {
                    printf("\n");
                }
                else {
                    REPORT_ERR("An unsupported escape sequence is detected.");
                }
            }
            else printf("%c", val->text[i]);
            i++;
        }
    }
    else print_value(val);

    return CONST_UNDEFINED;
}

Value* get_builtin_display() {
    ALLOC(val, Value);
    val->type = V_FUNCTION;
    val->evaluate_func = *builtin_display_internal;
    return val;
}
