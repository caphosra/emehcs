#pragma once

#include <setjmp.h>
#include <stdlib.h>

static jmp_buf error;

#define ALLOC(name, type) type* name = (type*)calloc(1, sizeof(type))
#define ASSERT(a, b) if (a != b) { longjmp(error); }
