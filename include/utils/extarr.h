#pragma once

#define EXT_ARR_PAGE_SIZE 0x1000

typedef struct _ExtArrPage ExtArrPage;
struct _ExtArrPage {
    ExtArrPage* prev;
    ExtArrPage* next;
    char data[EXT_ARR_PAGE_SIZE];
};

//
// Single-linked
//
typedef struct _ExtArr ExtArr;
struct _ExtArr {
    int unit_size;

    int w_current_pos;
    ExtArrPage* w_page;

    int r_current_pos;
    ExtArrPage* r_page;
};

ExtArr* ext_arr_init(int unit_size);
int ext_arr_mark_used(ExtArr* arr);
int ext_arr_consume(ExtArr* arr);
char* ext_arr_get_w_ptr(ExtArr* arr);
char* ext_arr_get_r_ptr(ExtArr* arr);
int ext_arr_is_empty(ExtArr* arr);
void ext_arr_reset_r_ptr(ExtArr* arr);
