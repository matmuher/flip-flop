#ifndef D_HAMLET_FUNCTIONS_H_INCLUDED
#define D_HAMLET_FUNCTIONS_H_INCLUDED

#include "string_funk.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>


enum LOGIC
    {
    FALSE = 0,
    TRUE = 1
    };


/*!
@brief struct that should contain pointers to begin and end of line
*/
struct beg_end_ptr
        {
        char* beg_ptr;
        char* end_ptr;
        };


/*!
@brief Read file_name with "r" mode and write it to allocated buffer
@return Pointer to allocated buffer\n
        FREE IT AFTER USING
*/
char* read_to_buffer (const char* file_pointer, size_t* file_size);


/*!
@brief Returns size of file in bytes
@details Uses fseek function to find end of file\n
         and ftell to get bytes size of file\n
         !! FILE MUST BE OPENED !!
*/
int get_size (FILE* file_pointer);


/*!
@brief Determines whether it is end of file or not
@details Compares ftell (file_pointer) and fseek (file_pointer, 0, SEEK_END);
@params[in] file_pointer pointer to opened file stream
@return TRUE if internal file pointer is in the end,\n
        FALSE if it is not
*/
int is_eof (FILE* file_pointer);


/*!
@brief Prints line-delimiter
*/
void say_delimiter();


/*!
@brief Checks if there are letter- or digit-chars in str
@params[in] str C-string
@return FALSE if there is letter- or digit-char\n
        TRUE if there are no chars\n
*/
int is_empty_line (const char* str);


/*!
@brief Compares strings (which addresses are stored in\n
       beg_ptr elements of structure) in lexicographical order from begin
*/
int struct_cmp_beg_ptr (const void* first_ptr_void,const void* second_ptr_void);


/*!
@brief Compares strings (which addresses are stored in\n
       beg_ptr elements of structure) in lexicographical order from end
*/
int struct_cmp_reversed_line (const void* first_ptr_void, const void* second_ptr_void);


/*!
@brief Prepares line for correct sorting
*/
char* format_line (char* line, beg_end_ptr* line_ptrs);


/*!
@brief Prints digit values of chars-elements of string
*/
void print_s_in_digs (char* str);


// One-project functions
struct beg_end_ptr* prepare_data (char* const buffer, size_t file_size, size_t* correct_lines);

void write_sort_to_file (const char* file_name, int correct_lines, const beg_end_ptr* line_ptrs);

void write_buffer_to_file (const char* const file_name, char* const buffer, size_t file_size);

void put_zeros (char* const buffer, size_t file_size);

int count_correct_lines (char* const buffer, size_t file_size);

#endif // D_HAMLET_FUNCTIONS_H_INCLUDED
