#pragma once

//
// Single-linked
//
struct ExtArr {
    int unit_size;

    int w_current_pos;
    struct ExtArrPage* w_page;

    int r_current_pos;
    struct ExtArrPage* r_page;
};

#define EXT_ARR_PAGE_SIZE 0x1000

struct ExtArrPage {
    struct ExtArrPage* prev;
    struct ExtArrPage* next;
    char data[EXT_ARR_PAGE_SIZE];
};

struct ExtArr* ext_arr_init(int unit_size);
int ext_arr_mark_used(struct ExtArr* arr);
int ext_arr_consume(struct ExtArr* arr);
char* ext_arr_get_w_ptr(struct ExtArr* arr);
char* ext_arr_get_r_ptr(struct ExtArr* arr);
int ext_arr_is_empty(struct ExtArr* arr);
void ext_arr_reset_r_ptr(struct ExtArr* arr);
