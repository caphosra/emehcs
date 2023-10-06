#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

#include "utils/peekable.h"

#define IS_IDENT_FRAGMENT(x) ((x) != '(' && (x) != ')' && (x) != '"' && ('!' <= (x) && (x) <= '~'))
#define IS_NUMERIC(x) ('0' <= (x) && (x) <= '9')

void tokenize_ident(struct ExtArr* tokens, struct Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    while (1) {
        char head = peekable_read(stream);
        if (IS_IDENT_FRAGMENT(head)) {
            if (pos == IDENT_MAX_LENGTH) {
                exit(1);
            }
            buffer[pos] = head;
            pos++;
            peekable_consume(stream);
        }
        else {
            char* str = calloc(pos + 1, sizeof(char));
            strcpy(str, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_IDENT;
            w_ptr->ident = str;
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
    }
}

void tokenize_num(struct ExtArr* tokens, struct Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    while (1) {
        char head = peekable_read(stream);
        if (IS_NUMERIC(head)) {
            if (pos == IDENT_MAX_LENGTH) {
                exit(1);
            }
            buffer[pos] = head;
            pos++;
            peekable_consume(stream);
        }
        else {
            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_NUM;
            w_ptr->num = atoi(buffer);
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
    }
}

void tokenize_string(struct ExtArr* tokens, struct Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    if (peekable_read(stream) != '"') {
        exit(1);
    }
    peekable_consume(stream);

    while (1) {
        char head = peekable_read(stream);
        if (!head) {
            exit(1);
        }
        if (head == '\n') {
            exit(1);
        }
        if (head == '"') {
            peekable_consume(stream);

            char* str = calloc(pos + 1, sizeof(char));
            strcpy(str, buffer);

            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_STRING;
            w_ptr->text = str;
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
        if (pos == IDENT_MAX_LENGTH) {
            exit(1);
        }
        buffer[pos] = head;
        pos++;
        peekable_consume(stream);
    }
}

struct ExtArr* tokenize(FILE* file) {
    struct ExtArr* tokens = ext_arr_init(sizeof(struct Token));

    char is_num = 1;
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    struct Peekable* stream = calloc(1, sizeof(struct Peekable));
    stream->file = file;

    while (peekable_read(stream)) {
        char head = peekable_read(stream);

        if (head == '\n') {
            peekable_consume(stream);
            break;
        }
        if (head == '(') {
            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_LEFT_PAREN;
            ext_arr_mark_used(tokens);

            peekable_consume(stream);
            continue;
        }
        if (head == ')') {
            struct Token* w_ptr = (struct Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_RIGHT_PAREN;
            ext_arr_mark_used(tokens);

            peekable_consume(stream);
            continue;
        }
        if (head == '"') {
            tokenize_string(tokens, stream);
            continue;
        }
        if (IS_NUMERIC(head)) {
            tokenize_num(tokens, stream);
            continue;
        }
        if (IS_IDENT_FRAGMENT(head)) {
            tokenize_ident(tokens, stream);
            continue;
        }
        peekable_consume(stream);
    }

    free(buffer);
    free(stream);

    return tokens;
}
