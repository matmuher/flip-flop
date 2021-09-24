#ifndef HAMLET_FUNCTIONS_H_INCLUDED
#define HAMLET_FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

struct beg_end_ptr
        {
        char* beg_ptr;
        char* end_ptr;
        };


int is_empty_line (char* str, int LINE_MAX_LEN);
int struct_cmp_beg_ptr (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr);
int struct_cmp_reversed_line (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr);
char* format_line (char* line, beg_end_ptr* line_ptrs, int LINE_MAX_LEN);
void print_s_in_digs (char* str);


#endif // HAMLET_FUNCTIONS_H_INCLUDED
