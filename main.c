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

#ifdef EMEHCS_OUTPUT_INTERMEDIATE_OBJ
            printf("-----< tokenizer >-----\n");
            print_tokens(arr);
#endif

            Expr* expr = parse_expr(arr);

#ifdef EMEHCS_OUTPUT_INTERMEDIATE_OBJ
            printf("-----<  parser  >------\n");
            print_expr(expr);
#endif

            if (optimize_tail_recursion(expr)) {
#ifdef EMEHCS_OUTPUT_INTERMEDIATE_OBJ
                printf("-----< optimizer >-----\n");
                print_expr(expr);
#endif
            }

#ifdef EMEHCS_OUTPUT_INTERMEDIATE_OBJ
            printf("-----------------------\n");
#endif

            Value* val = evaluate(env, expr);
            print_value(val);
        }
    }
}
