#include <stdio.h>

struct _Peekable {
    FILE* file;
    char current;
};
typedef struct _Peekable Peekable;

char peekable_read(Peekable* peekable);
void peekable_consume(Peekable* peekable);
