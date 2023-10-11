#include <setjmp.h>
#include <stdlib.h>

#include "evaluation.h"
#include "parser.h"
#include "tokenizer.h"
#include "utils/error.h"
#include "utils/utils.h"

int main(void) {
    while (1) {
        printf(">> ");
        if (setjmp(err_jmp)) {
            printf("Error: %s (%s line: %d)\n", err_msg, err_file, err_pos);
        } else {
            ExtArr* arr = tokenize(stdin);
            print_tokens(arr);

            Expr* expr = parse_expr(arr);
            print_expr(expr);

            ALLOC(env, Environment);
            setup_builtin(env);
            Value* val = evaluate(env, expr);
            print_value(val);
        }
    }
}
