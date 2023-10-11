#include "utils/peekable.h"

char peekable_read(Peekable* peekable) {
    if (!peekable->current) {
        if (fread(&peekable->current, sizeof(char), 1, peekable->file)) {
            return peekable_read(peekable);
        } else {
            return 0;
        }
    }
    return peekable->current;
}

void peekable_consume(Peekable* peekable) {
    peekable->current = 0;
}
