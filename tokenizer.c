#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

void take_ident(struct ExtArr* tokens, char* is_num, int* pos, char* buffer) {
    if (*pos) {
        char* str = calloc(*pos + 1, sizeof(char));
        strcpy(str, buffer);

        struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
        if (*is_num) {
            w_ptr->type = T_NUM;
            w_ptr->num = atoi(str);
        } else {
            w_ptr->type = T_IDENT;
            w_ptr->ident = str;
        }
        ext_arr_mark_used(tokens);

        *is_num = 1;
        *pos = 0;
        memset(buffer, 0, IDENT_MAX_LENGTH + 1);
    }
}

struct ExtArr* tokenize(FILE* file) {
    struct ExtArr* tokens = ext_arr_init(sizeof(struct Token));

    char is_num = 1;
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    char tmp;
    while (fread(&tmp, sizeof(char), 1, file)) {
        if (tmp == '\0') continue;
        if (tmp == '\n') break;
        if (tmp == '(') {
            take_ident(tokens, &is_num, &pos, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_LEFT_PAREN;
            ext_arr_mark_used(tokens);
            continue;
        }
        if (tmp == ')') {
            take_ident(tokens, &is_num, &pos, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_RIGHT_PAREN;
            ext_arr_mark_used(tokens);
            continue;
        }
        if (tmp == '[') {
            take_ident(tokens, &is_num, &pos, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_LEFT_BRACKET;
            ext_arr_mark_used(tokens);
            continue;
        }
        if (tmp == ']') {
            take_ident(tokens, &is_num, &pos, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_RIGHT_BRACKET;
            ext_arr_mark_used(tokens);
            continue;
        }
        if ('0' <= tmp && tmp <= '9') {
            if (pos == IDENT_MAX_LENGTH) {
                return NULL;
            }
            buffer[pos] = tmp;
            pos++;
            continue;
        }
        if (('a' <= tmp && tmp <= 'z') || ('A' <= tmp && tmp <= 'Z')) {
            if (pos == IDENT_MAX_LENGTH) {
                return NULL;
            }
            is_num = 0;
            buffer[pos] = tmp;
            pos++;
            continue;
        }
        take_ident(tokens, &is_num, &pos, buffer);
    }

    take_ident(tokens, &is_num, &pos, buffer);

    return tokens;
}
