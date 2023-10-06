#pragma once

#include <stdio.h>

#include "utils/extarr.h"

#define IDENT_MAX_LENGTH 0x100 - 1

enum TokenType {
    T_LEFT_PAREN,
    T_RIGHT_PAREN,
    T_LEFT_BRACKET,
    T_RIGHT_BRACKET,
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

struct ExtArr* tokenize(FILE* file);
