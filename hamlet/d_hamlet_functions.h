#ifndef D_HAMLET_FUNCTIONS_H_INCLUDED
#define D_HAMLET_FUNCTIONS_H_INCLUDED

#include "string_funk.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

/*!
@brief Returns size of file in bytes
@details Uses fseek function to find end of file\n
         and ftell to get bytes size of file\n
         !! FILE MUST BE OPENED !!
*/
int get_size (FILE* file_pointer);

/*!
@brief Determines whether it is end of file or not
@details Compares cur_pos and file_size
@params[in] cur_pos you should place here: ftell (FILE* file_pointer)\n
            where file_pointer points to opened file
@return TRUE if internal file pointer is in the end,\n
        FALSE if it is not
*/
int is_eof (int cur_pos, int file_size);

void say_delimiter();

/*!
@brief struct that should contain pointers to begin and end of line
*/
struct beg_end_ptr
        {
        char* beg_ptr;
        char* end_ptr;
        };

/*!
@brief Checks if there are letter- or digit-chars in the str
@params[in] str array of chars
@return TRUE if there is letter- or digit-char\n
        and it is not last element of array
        FALSE if there are no chars or there is one\n
        but it is the last element
*/
int is_empty_line (char* str, int LINE_MAX_LEN);

/*!
@brief Compares strings (which addresses are stored in\n
       beg_ptr elements of structure) in lexicographical order from begin
*/
int struct_cmp_beg_ptr (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr);

/*!
@brief Compares strings (which addresses are stored in\n
       beg_ptr elements of structure) in lexicographical order from end
*/
int struct_cmp_reversed_line (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr);

/*!
@brief Prepares line for correct sorting
*/
char* format_line (char* line, beg_end_ptr* line_ptrs, int LINE_MAX_LEN);

/*!
@brief Prints digit values of chars-elements of string
*/
void print_s_in_digs (char* str);

#endif // D_HAMLET_FUNCTIONS_H_INCLUDED
