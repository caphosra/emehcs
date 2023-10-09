#include "parser.h"
#include "tokenizer.h"

int main(void) {
    while (1) {
        printf(">> ");
        struct ExtArr* arr = tokenize(stdin);
        if (!arr) continue;
        print_tokens(arr);

        struct Expr* expr = parse_expr(arr);
        print_expr(expr);
    }
}
