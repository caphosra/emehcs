#pragma once

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#define ERROR_MESSAGE_MAX_LENGTH 0x80 - 1

// Reports the error and jumps to the designated point.
// Do not give an error message longer than ERROR_MESSAGE_MAX_LENGTH.
#define REPORT_ERR(fmt...)                                \
    {                                                     \
        memset(err_msg, 0, ERROR_MESSAGE_MAX_LENGTH + 1); \
        sprintf(err_msg, fmt);                            \
        err_file = __FILE__;                              \
        err_pos = __LINE__;                               \
        longjmp(err_jmp, 1);                              \
    }

#define ASSERT(cond, fmt) \
    if (!(cond)) {        \
        REPORT_ERR(fmt);  \
    }

jmp_buf err_jmp;
char err_msg[ERROR_MESSAGE_MAX_LENGTH + 1];
char* err_file;
int err_pos;
