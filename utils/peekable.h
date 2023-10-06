#include <stdio.h>

struct Peekable {
    FILE* file;
    char current;
};

char peekable_read(struct Peekable* peekable);
void peekable_consume(struct Peekable* peekable);
