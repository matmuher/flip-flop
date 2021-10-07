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
struct line_buf
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
       elements of line structure) from begin in lexicographical order
*/
int compare_line_straight (const void* first_line_void,const void* second_line_void);


/*!
@brief Compares strings (which addresses are stored in\n
    elements of line structure) from end in lexicographical order
*/
int compare_line_reverse (const void* first_line_void, const void* second_line_void);


/*!
@brief Prepares line for correct sorting
*/
char* format_line (char* line, line_buf* line_ptrs);


/*!
@brief Prints digit values of chars-elements of string
*/
void print_s_in_digs (char* str);

/*!
@brief Silly byte swap
*/
void byte_swap (void* first, void* second, size_t element_size);

/*!
@brief Formats lines from line_buf structure object
       to ignore punctuation in the begin and in the end
*/
line_buf* prepare_data (line_buf* line_ptrs, size_t lines_num);

/*!
@brief Get lines_num lines from file, package it in line_buf structure object
@details get_strings calls elephant_calloc for better memory allocation,\n
         elephant_calloc_extert.h must be included
         and elephant_calloc.cpp must be included to the project.\n\n
         As memory was allocated:\n
         [YOU SHOULD USE MEMORY_FREE FUNCTION WHEN FINISH USING LINES]
@params[in] *lines_num pointer to variable where number of lines will be written
@params[out] lines_num number_of_lines
@return pointer to line_buf structure object with with written lines
*/
line_buf* get_strings (char* file_name, size_t* lines_num);

/*!
@brief Write line_buf to file
*/
void write_line_buf_to_file (const char* file_name, int correct_lines, const line_buf* line_ptrs);

/*!
@brief Copies line_buf structure object
*/
line_buf* copy_line_buf (line_buf* origin, size_t lines_num);

/*!
@brief Replace '\n' with '\0' in buffer
       Adds '\0' in the end
*/
void put_zeros (char* const buffer, size_t file_size);

/*!
@brief Count not empty lines
*/
int count_correct_lines (char* const buffer, size_t file_size);

/*!
@brief Literally bubble sort. My first sorting algorithm!
*/
void bubble_sort (void* arr,  size_t arr_size, size_t elem_size, int (*compar)(const void *, const void*));


#endif // D_HAMLET_FUNCTIONS_H_INCLUDED
