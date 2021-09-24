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

int is_eof (FILE* file_pointer)
    {
    assert (file_pointer != NULL);

    int cur_pos = ftell (file_pointer);
    int file_size = get_size (file_pointer);

    return cur_pos == file_size;
    }

void print_line (const char symbol, size_t line_len)
    {
    for (size_t sym_id = 0; sym_id < line_len; sym_id++)
        {
        putchar (symbol);
        }
    }

void say_delimiter()
    {
    const size_t DELIMITER_LEN = 100;

    print_line ('=', DELIMITER_LEN);

    puts ("\n\nMeow!\n");

    print_line ('=', DELIMITER_LEN);
    }


int struct_cmp_beg_ptr (const void* first_ptr_void, const void* second_ptr_void)
    {
    assert (first_ptr_void != NULL);
    assert (second_ptr_void != NULL);

    beg_end_ptr* first_ptr = (beg_end_ptr*) first_ptr_void;
    beg_end_ptr* second_ptr = (beg_end_ptr*) second_ptr_void;

    assert (first_ptr->beg_ptr != NULL);
    assert (second_ptr->beg_ptr != NULL);

    char* first = first_ptr->beg_ptr;
    char* second = second_ptr->beg_ptr;

    return string_cmp (first, second);
    }

int struct_cmp_reversed_line (const void* first_ptr_void, const void* second_ptr_void)
    {
    assert (first_ptr_void != NULL);
    assert (second_ptr_void != NULL);

    beg_end_ptr* first_ptr = (beg_end_ptr*) first_ptr_void;
    beg_end_ptr* second_ptr = (beg_end_ptr*) second_ptr_void;

    assert (first_ptr->beg_ptr != NULL);
    assert (first_ptr->end_ptr != NULL);
    assert (second_ptr->beg_ptr != NULL);
    assert (second_ptr->end_ptr != NULL);

    char* first_beg = first_ptr->beg_ptr;
    char* first_end = first_ptr->end_ptr;

    char* second_beg = second_ptr->beg_ptr;
    char* second_end = second_ptr->end_ptr;


    int shift_from_end = 0;

    while (LOOP)
        {
        // Get begin of second string
        if ((second_end - shift_from_end) < second_beg)
            {
            break;
            }
        // Get begin of first string
        else if ((first_end - shift_from_end) < first_beg)
            {
            break;
            }
        // Chars are not equal and can be compared
        else if(*(second_end - shift_from_end) != *(first_end - shift_from_end))
            {
            break;
            }

        shift_from_end++;
        }

    return *(first_end - shift_from_end) - *(second_end - shift_from_end);
    }


char* format_line (char* line, beg_end_ptr* line_ptrs)
    {
    assert (line != NULL);
    assert (line_ptrs != NULL);

    // Skip non-al/num chars
    int sym_id = 0;
    while (!isalnum (line[sym_id])) {sym_id++;}

    // Start with lowercase
    //line[sym_id] = tolower (line[sym_id]);

    // beg_ptr points to the first char in line
    line_ptrs->beg_ptr = &line[sym_id];



    // Stops when find zero or end of line
    char* last_alnum = &line[sym_id];
    while (line[++sym_id])
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

    putchar ('\n');
    }


int is_empty_line (const char* str)
    {
    assert (str != NULL);

    // Line that is above makes code more readable?
    // char* str_max = find_char (str, '\0');

    while (!isalnum (*str) && *str)
        {
        str++;
        }

    // Loop reached end of line without finding
    // al/num char
    return !*str;
    }

char* read_to_buffer (const char* file_name, size_t* file_size)
    {
    FILE* file_pointer = fopen (file_name, "r");

    // Allocate empty buffer
    char* buffer = NULL;
    *file_size = get_size (file_pointer);

    buffer = (char*)calloc (*file_size, sizeof (char));

    // Fill the buffer
    size_t fread_return = fread (buffer, sizeof (char), *file_size, file_pointer);

    fclose (file_pointer);

    // In case of 'r' in fopen function argument on Windows
    // Only?
    if (fread_return != *file_size)
        {
        *file_size = fread_return;
        buffer = (char*) realloc (buffer, *file_size);
        }

    return buffer;
    }

void write_sort_to_file (const char* file_name, int correct_lines, const beg_end_ptr* line_ptrs)
    {
    FILE* file_writer = fopen (file_name, "w");

    for (int cur_line = 0; cur_line < correct_lines; cur_line++)
        {
        fputs ((line_ptrs + cur_line)->beg_ptr, file_writer);
        putc ('\n', file_writer);
        }

    fclose (file_writer);
    }

void write_buffer_to_file (const char* const file_name, char* const buffer, size_t file_size)
    {
    FILE* file_writer = fopen (file_name, "w");

    char* cur_pos = (char*) buffer;
    while (cur_pos < buffer + file_size)
        {
        char* str_end = find_char (cur_pos, '\0');

        fputs (cur_pos, file_writer);
        fputc ('\n', file_writer);

        cur_pos = str_end + 1;
        }
    fclose (file_writer);
    }

void put_zeros (char* const buffer, size_t file_size)
    {
    char* cur_pos = buffer;
    while (cur_pos < buffer + file_size)
        {
        if (*cur_pos == '\n')
            {
            *cur_pos = '\0';
            }
        cur_pos++;
        }
    }

int count_correct_lines (char* const buffer, size_t file_size)
    {
    char* cur_pos = buffer;
    size_t correct_lines = 0;

    while (cur_pos < buffer + file_size)
        {
        char* str_end = find_char (cur_pos, '\0');

        if (!is_empty_line (cur_pos))
            {
            correct_lines++;
            }

        cur_pos = str_end + 1;
        }

    return correct_lines;
    }

struct beg_end_ptr* prepare_data (char* const buffer, size_t file_size, size_t* correct_lines)
    {
    put_zeros (buffer, file_size);

    *correct_lines = count_correct_lines (buffer, file_size);

    struct beg_end_ptr* line_ptrs = (beg_end_ptr*) calloc (*correct_lines, sizeof (beg_end_ptr));

    char* cur_pos = buffer;
    size_t cur_line = 0;

    while (cur_line < *correct_lines)
        {
        char* str_end = find_char (cur_pos, '\0');
        if (!is_empty_line (cur_pos))
            {
            format_line (cur_pos, &line_ptrs[cur_line]);

            cur_line++;
            }

        cur_pos = str_end + 1;
        }

    return line_ptrs;
    }

