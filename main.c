#include <setjmp.h>
#include <stdlib.h>

#include "evaluation.h"
#include "parser.h"
#include "tokenizer.h"
#include "builtin.h"
#include "optimizer.h"
#include "utils/error.h"
#include "utils/utils.h"

int main(void) {
    ALLOC(env, Environment);
    setup_builtin(env);

    while (1) {
        printf(">> ");
        if (setjmp(err_jmp)) {
            printf("Error: %s (%s line: %d)\n", err_msg, err_file, err_pos);
        } else {
            ExtArr* arr = tokenize(stdin);
            print_tokens(arr);

            Expr* expr = parse_expr(arr);
            print_expr(expr);
            Expr* optimized = optimize_tail_recursion(expr);
            print_expr(optimized);

            printf("--------------------\n");
            Value* val = evaluate(env, optimized);
            print_value(val);
            printf("\n");
        }
    }
}
