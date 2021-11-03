#ifndef STRING_FUNC_H
#define STRING_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int LOOP = 1;
const int ZERO_SHIFT = 0;

/*!
@brief Outputs given C-type string to console
@params[in] str string for output
@return length of argument string considering '\0' character
*/
int say (const char str[]);


/*!
@brief Searches for char chr in C-string str
@details 1. str must include '\0' character
@params[in] str C-string
@params[in] chr character that is needed to be found
@return char pointer to chr or NULL if there is no chr in str
*/
char* find_char (const char str[], char chr);


/*!
@brief Returns length of C-string
@details Counts length using find_char function\n
         i.e. string_func library must be included
@params[in] str C-string which length must be counted
@return length of str
*/
int string_len (const char str[]);


/*!
@brief Copies source to destination
@params[in] destination array where source will be copied in\n
            its length must be greater or equal to source length
@params[in] source string that will be copied in destination
@return pointer to the begging of destination
*/
char* string_copy (char destination[], char source[]);


/*!
@brief Same as strncpy from <string.h>
*/
char* string_nchrs_copy (char destination[], char source[], int n);


/*!
@brief Concatenates destination and source C-strings
@details Uses string_copy function from string_funk library
         to concatenate strings.\n
         Source string will be copied after last non-'\0' character\n
         of destination. I.e. '\0' of destination will be replaced\n
         with first character of source.
@params[in] destination C-string, which end will be added src string to.\n
                        Must be big enough to store src string after\n
                        last non-zero character.
@params[out] source  C-string, that will be added to end of destination
@return pointer to the begging of destination
*/
char* string_stick (char destination[], char source[]);


/*!
@brief Allocate dynamic memory and place there copy of str C-string
@details Calculates size of str by function string_len, allocates memory\n
         using calloc function from stdlib.h,\n
         if everything is alright copies str to allocated  memory.
         if no — returns NULL-pointer\n\n

         After using returned pointer MUST be passed to free to avoid a memory leak.
@params[in] str C-string that is needed to be duplicated in dynamic memory
@params[out] ?
@return pointer to duplicated str in dynamic memory\n
        or NULL-pointer if error occured
*/
char* string_dup (char str[]);


/*!
@brief Read string from file (file_pointer) to string str
@details Writes line or n characters from file to str\n
         and adds '\0' in the end
@params[in] str array for writing a string
@params[in] n number of characters to read from file
@params[in] file_pointer pointer to file (type FILE*)
@params[out] str array with written line
@return pointer to the begging of str, that is C-string
*/
char* fget_string (char str[], int n, FILE* file_pointer);


/*!
@brief Compares strings in lexicographic order
@details Function iterates symbols from left/right_start
         to left/right_end with step = shift, so:\n
         if you want to compare strings straightly:\n
         left_start < left_end, shift = 1,\n
         if reversely:\n
         left_start > left_end, shift = -1\n
@params[in] left/right_start from here comparison will start
@params[in] left/right_end here comparison will stop
@params[in] shift 1 or -1, depends on straight or reverse comparison
@return Negative value if left string appears before right string in lexicographical order\n
        Zero if strings are equal\n
        Positive value if left_string appears after right_string\n
*/
int string_compare (char* const left_start, char* const left_end,
                    char* const right_start, char* const right_end, const int shift);


/*!
@brief Prints line with length equal to line_len
       that consists of symbol symbols
*/
void print_line (const char symbol, size_t line_len);

/*!
@brief Prints line with length equal to line_len
       that consists of symbol symbols in file_ptr
*/
void fprint_line (FILE* file_ptr, const char symbol, size_t line_len);

#endif
