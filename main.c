#include "tokenizer.h"

int main(void) {
    while (1) {
        printf(">> ");
        struct ExtArr* arr = tokenize(stdin);
        if (arr) {
            while (!ext_arr_is_empty(arr)) {
                struct Token* token = (struct Token*)ext_arr_get_r_ptr(arr);
                switch (token->type) {
                    case T_LEFT_PAREN:
                        printf("T_LEFT_PAREN ");
                        break;
                    case T_RIGHT_PAREN:
                        printf("T_RIGHT_PAREN ");
                        break;
                    case T_LEFT_BRACKET:
                        printf("T_LEFT_BRACKET ");
                        break;
                    case T_RIGHT_BRACKET:
                        printf("T_RIGHT_BRACKET ");
                        break;
                    case T_IDENT:
                        printf("T_IDENT(%s) ", token->ident);
                        break;
                    case T_NUM:
                        printf("T_NUM(%d) ", token->num);
                        break;
                }
                ext_arr_consume(arr);
            }
        }
        printf("\n");
    }
}
