#pragma once

#include <setjmp.h>
#include <string.h>

#define ERROR_MESSAGE_MAX_LENGTH 0x80 - 1
#define REPORT_ERR(text) { \
    strncpy(err_msg, text, ERROR_MESSAGE_MAX_LENGTH); \
    err_file = __FILE__; \
    err_pos = __LINE__; \
    longjmp(err_jmp, 1); \
}

jmp_buf err_jmp;
char err_msg[ERROR_MESSAGE_MAX_LENGTH + 1];
char* err_file;
int err_pos;
