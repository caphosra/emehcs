#include "utils/extarr.h"

#include <string.h>

#include "utils/utils.h"

struct ExtArr* ext_arr_init(int unit_size) {
    ALLOC(arr, struct ExtArr);
    ALLOC(page, struct ExtArrPage);
    if (unit_size > EXT_ARR_PAGE_SIZE) {
        return NULL;
    }
    arr->unit_size = unit_size;
    arr->w_page = page;
    arr->r_page = page;
    return arr;
}

int ext_arr_mark_used(struct ExtArr* arr) {
    arr->w_current_pos++;
    if (arr->w_current_pos * arr->unit_size > EXT_ARR_PAGE_SIZE) {
        // The page is all consumed.
        ALLOC(page, struct ExtArrPage);

        if (!page) {
            // Failed to allocate the memory.
            return 1;
        }

        arr->w_current_pos = 0;
        page->prev = arr->w_page;
        arr->w_page->next = page;
        arr->w_page = page;
    }
    return 0;
}

int ext_arr_consume(struct ExtArr* arr) {
    arr->r_current_pos++;
    if (arr->r_current_pos * arr->unit_size > EXT_ARR_PAGE_SIZE) {
        arr->r_current_pos = 0;
        if (!arr->r_page->next) {
            return 1;
        }
        arr->r_page = arr->r_page->next;
    }
    return 0;
}

inline char* ext_arr_get_w_ptr(struct ExtArr* arr) {
    return arr->w_page->data + arr->unit_size * arr->w_current_pos;
}

inline char* ext_arr_get_r_ptr(struct ExtArr* arr) {
    return arr->r_page->data + arr->unit_size * arr->r_current_pos;
}

inline int ext_arr_is_empty(struct ExtArr* arr) {
    return ext_arr_get_w_ptr(arr) == ext_arr_get_r_ptr(arr);
}

void ext_arr_reset_r_ptr(struct ExtArr* arr) {
    arr->r_current_pos = 0;
    while (arr->r_page->prev) {
        arr->r_page = arr->r_page->prev;
    }
}
