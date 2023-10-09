#include <setjmp.h>
#include <stdlib.h>

#include "utils/error.h"
#include "parser.h"
#include "tokenizer.h"

int main(void) {
    while (1) {
        printf(">> ");
        if (setjmp(err_jmp)) {
            printf("Error: %s (%s line: %d)\n", err_msg, err_file, err_pos);
        }
        else {
            struct ExtArr* arr = tokenize(stdin);
            print_tokens(arr);

            struct Expr* expr = parse_expr(arr);
            print_expr(expr);
        }
    }
}
