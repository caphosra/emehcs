#pragma once

#include <stdio.h>

#include "utils/extarr.h"

#define IDENT_MAX_LENGTH 0x100 - 1

enum TokenType {
    T_LEFT_PAREN,
    T_RIGHT_PAREN,
    T_IDENT,
    T_NUM,
    T_STRING
};

struct Token {
    enum TokenType type;
    union {
        char* ident;
        char* text;
        int num;
    };
};

ExtArr* tokenize(FILE* file);
void print_tokens(ExtArr* tokens);
