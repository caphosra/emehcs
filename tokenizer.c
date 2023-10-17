#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

#include "utils/error.h"
#include "utils/peekable.h"

#define IS_IDENT_FRAGMENT(x) ((x) != '(' && (x) != ')' && (x) != '"' && ('!' <= (x) && (x) <= '~'))
#define IS_NUMERIC(x) ('0' <= (x) && (x) <= '9')

void tokenize_ident(ExtArr* tokens, Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    while (1) {
        char head = peekable_read(stream);
        if (IS_IDENT_FRAGMENT(head)) {
            if (pos == IDENT_MAX_LENGTH) {
                REPORT_ERR("A name of a variable is too long.");
            }
            buffer[pos] = head;
            pos++;
            peekable_consume(stream);
        } else {
            char* str = calloc(pos + 1, sizeof(char));
            strcpy(str, buffer);

            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_IDENT;
            w_ptr->ident = str;
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
    }
}

void tokenize_num(ExtArr* tokens, Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    while (1) {
        char head = peekable_read(stream);
        if (IS_NUMERIC(head)) {
            if (pos == IDENT_MAX_LENGTH) {
                REPORT_ERR("A number is too big.");
            }
            buffer[pos] = head;
            pos++;
            peekable_consume(stream);
        } else {
            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_NUM;
            w_ptr->num = atoi(buffer);
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
    }
}

void tokenize_string(ExtArr* tokens, Peekable* stream) {
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    if (peekable_read(stream) != '"') {
        REPORT_ERR("There is no string which does not start with \'\"\'.");
    }
    peekable_consume(stream);

    while (1) {
        char head = peekable_read(stream);
        if (!head) {
            REPORT_ERR("A string cannot contains a null byte.");
        }
        if (head == '\n') {
            REPORT_ERR("A string cannot contains a LF.");
        }
        if (head == '"') {
            peekable_consume(stream);

            char* str = calloc(pos + 1, sizeof(char));
            strcpy(str, buffer);

            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_STRING;
            w_ptr->text = str;
            ext_arr_mark_used(tokens);

            free(buffer);
            return;
        }
        if (pos == IDENT_MAX_LENGTH) {
            REPORT_ERR("The string is too long.");
        }
        buffer[pos] = head;
        pos++;
        peekable_consume(stream);
    }
}

ExtArr* tokenize(FILE* file) {
    ExtArr* tokens = ext_arr_init(sizeof(Token));

    char is_num = 1;
    int pos = 0;
    char* buffer = calloc(IDENT_MAX_LENGTH + 1, sizeof(char));

    Peekable* stream = calloc(1, sizeof(Peekable));
    stream->file = file;

    while (peekable_read(stream)) {
        char head = peekable_read(stream);

        if (head == '\n') {
            peekable_consume(stream);
            break;
        }
        if (head == '\'') {
            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_QUOTE;
            ext_arr_mark_used(tokens);

            peekable_consume(stream);
            continue;
        }
        if (head == '(') {
            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
            w_ptr->type = T_LEFT_PAREN;
            ext_arr_mark_used(tokens);

            peekable_consume(stream);
            continue;
        }
        if (head == ')') {
            Token* w_ptr = (Token*)ext_arr_get_w_ptr(tokens);
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

void print_tokens(ExtArr* tokens) {
    char first = 1;
    while (!ext_arr_is_empty(tokens)) {
        if (!first) {
            printf(" ");
        }
        first = 0;

        Token* token = (Token*)ext_arr_get_r_ptr(tokens);
        switch (token->type) {
            case T_QUOTE:
                printf("T_QUOTE");
                break;
            case T_LEFT_PAREN:
                printf("T_LEFT_PAREN");
                break;
            case T_RIGHT_PAREN:
                printf("T_RIGHT_PAREN");
                break;
            case T_IDENT:
                printf("T_IDENT(%s)", token->ident);
                break;
            case T_NUM:
                printf("T_NUM(%d)", token->num);
                break;
            case T_STRING:
                printf("T_STRING(%s)", token->text);
                break;
        }
        ext_arr_consume(tokens);
    }
    printf("\n");
    ext_arr_reset_r_ptr(tokens);
}
