#include <stdio.h>
#include <stdlib.h>
#include "..\hamlet_dynamic_proc\string_funk.h"
#include "..\hamlet_dynamic_proc\d_hamlet_functions.h"

// Allocates memory for each string

int main()
    {
    const size_t LINE_MAX_LEN = 150;

    FILE* file_pointer = fopen ("hamlet.txt", "r");
    size_t file_size = get_size (file_pointer);

    // Number of correct lines
    size_t lines = 0;

    // Structure of pointers to end and beg of each line
    beg_end_ptr* lines_ptrs = NULL;

    // Dynamic buffer
    char* buffer_for_sort = NULL;

    // Pointer for dynamic buffer filling
    char* cur_pos = NULL;
    size_t cur_buf_size = 0;


    while (!is_eof (ftell (file_pointer), file_size))
        {
        char buf_str[LINE_MAX_LEN] = {};

        // Get string
        fget_string (buf_str, LINE_MAX_LEN, file_pointer);

        if (!is_empty_line (buf_str, LINE_MAX_LEN))
            {
            // Temporary ptrs buffer
            beg_end_ptr temp_ptrs = {0, 0};
            lines++;

            // Format string for sorting
            format_line (buf_str, &temp_ptrs, LINE_MAX_LEN);

            // Calculate length considering '\0' char
            size_t str_len = temp_ptrs.end_ptr + 1 - temp_ptrs.beg_ptr ;

            // Expand allocated buffer memory for new string
            size_t need_buf_size = cur_buf_size +  str_len * sizeof (char);
            buffer_for_sort = (char*) realloc (buffer_for_sort, need_buf_size);

            // Copy string from static buffer to dynamic buffer
            cur_pos = buffer_for_sort + cur_buf_size;
            string_copy (cur_pos, temp_ptrs.beg_ptr);

            cur_buf_size = need_buf_size;
            }
        }

    // Allocate memory to contain ptrs to beg and end of each line
    lines_ptrs = (beg_end_ptr*) calloc (lines, sizeof (beg_end_ptr));

    // Put ptrs to begin and end of each line in structure elements
    char* temp_ptr = buffer_for_sort;
    for (int line = 0; line < lines; line++)
        {
        lines_ptrs[line].beg_ptr = temp_ptr;
        while (*temp_ptr++) {;}

        // temp_ptr points to next symbol after '\0' of current line
        lines_ptrs[line].end_ptr = temp_ptr - 1;
        }

    // Print sort straightly
    say ("\nSorted lexicographically\n");

    qsort (lines_ptrs, lines, sizeof (beg_end_ptr),
    (int (*)(const void*,const void*)) struct_cmp_beg_ptr);

    for (size_t line = 0; line < lines; line++)
        {
        printf ("%d %s\n", line, lines_ptrs[line].beg_ptr);
        }

    say_delimiter ();


    // Print sort reversely
    say ("\nSorted poetically\n");

    qsort (lines_ptrs, lines, sizeof (beg_end_ptr),
    (int (*)(const void*,const void*)) struct_cmp_reversed_line);

    for (size_t line = 0; line < lines; line++)
        {
        printf ("%d %s\n", line, lines_ptrs[line].beg_ptr);
        }

    // Print non-sorted
    temp_ptr = buffer_for_sort;
    for (size_t line = 0; line < lines; line++)
        {
        printf ("%d ", line);

        while (*temp_ptr++)
            {
            printf ("%c",*(temp_ptr-1));
            }

        // For new line
        say ("");
        }

    // Free string memory
    free (buffer_for_sort);

    // Free structure memory
    free (lines_ptrs);

    fclose (file_pointer);
    }

