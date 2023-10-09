#pragma once

#include <stdlib.h>

#define ALLOC(name, type) type* name = (type*)calloc(1, sizeof(type))
