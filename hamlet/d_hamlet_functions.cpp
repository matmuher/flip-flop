#include "d_hamlet_functions.h"

int get_size (FILE* file_pointer)
    {
    assert (file_pointer != NULL);

    int cur_pos = ftell (file_pointer);

    // Get size
    fseek (file_pointer, 0, SEEK_END);
    int file_size = ftell (file_pointer);

    // Return internal pointer
    fseek (file_pointer, cur_pos, SEEK_SET);

    return file_size;
    }

int is_eof (int cur_pos, int file_size)
    {
    assert (cur_pos <= file_size);

    return cur_pos == file_size ? TRUE : FALSE;
    }

void say_delimiter()
    {
    const size_t DELIMITER_LENGTH = 100;

    for (int char_id = 0; char_id < DELIMITER_LENGTH; char_id ++)
        {
        printf ("%c", '=');
        }

    puts ("\n\nMeow!\n");

    for (int char_id = 0; char_id < DELIMITER_LENGTH; char_id ++)
        {
        printf ("%c", '=');
        }
    }


int struct_cmp_beg_ptr (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr)
    {
    char* first = first_ptr->beg_ptr;
    char* second = second_ptr->beg_ptr;

    return string_cmp (first, second);
    }

int struct_cmp_reversed_line (beg_end_ptr* first_ptr, beg_end_ptr* second_ptr)
    {
    assert (first_ptr != NULL);
    assert (second_ptr != NULL);
    assert (first_ptr->beg_ptr != NULL);
    assert (first_ptr->end_ptr != NULL);
    assert (second_ptr->beg_ptr != NULL);
    assert (second_ptr->end_ptr != NULL);

    char* first_beg = first_ptr->beg_ptr;
    char* first_end = first_ptr->end_ptr;

    char* second_beg = second_ptr->beg_ptr;
    char* second_end = second_ptr->end_ptr;

    // Conditions are about:
    // 1. and 2. didn't get begin of any string
    // and  3. chars are equal
    int shift_from_end = 0;
    while ( (second_end - shift_from_end) >= second_beg &&
            (first_end - shift_from_end) >= first_beg &&
           *(second_end - shift_from_end) == *(first_end - shift_from_end) )
        {
        shift_from_end++;
        }

    return *(first_end - shift_from_end) - *(second_end - shift_from_end);
    }


char* format_line (char* line, beg_end_ptr* line_ptrs, int LINE_MAX_LEN)
    {
    assert (line != NULL);
    assert (line_ptrs != NULL);
    assert (LINE_MAX_LEN > 0);

    int sym_id = 0;

    // Skip non-al/num chars
    while (!isalnum (line[sym_id])) {sym_id++;}

    // beg_ptr points to the first char in line
    line_ptrs->beg_ptr = &line[sym_id];

    char* last_alnum = &line[sym_id];

    // Stops when find zero or end of line
    while (line[++sym_id] && sym_id < LINE_MAX_LEN)
        {
        if (isalnum (line[sym_id]))
            {
            last_alnum = &line[sym_id];
            }
        }

    // End of line — straightly after last al/num symbol
    *++last_alnum = '\0';

    line_ptrs->end_ptr = last_alnum;

    return line_ptrs->beg_ptr;
    }


void print_s_in_digs (char* str)
    {
    assert (str != NULL);

    while (*str)
        {
        printf ("%d ", *(str++));
        }

    // New line
    puts ("");
    }


int is_empty_line (char* str, int LINE_MAX_LEN)
    {
    assert (str != NULL);
    assert (LINE_MAX_LEN > 0);

    char* str_max = str + LINE_MAX_LEN - 1;

    while (!isalnum (*str) && str < str_max)
        {
        if (*str == '\0')
            {
            return TRUE;
            }
        str++;
        }

    if (str < str_max)
        {
        return FALSE;
        }
    else
        {
        // Line ends with "...c" i.e. there is no '\n'
        // char in the end.
        // It's incorrect and
        // we disrespect such lines

        return TRUE;
        }
    }
