#include <stdio.h>
#include <stdlib.h>
#include "..\hamlet_dynamic_proc\string_funk.h"
#include "..\hamlet_dynamic_proc\d_hamlet_functions.h"

int main()
    {
    // Read raw data to buffer
    char* buffer = NULL;
    size_t file_size = -1;
    char file_name[] = "hamlet.txt";

    buffer = read_to_buffer (file_name, &file_size);

    // Fill structure with pointers to formatted data
    size_t correct_lines = -1;
    struct beg_end_ptr* line_ptrs = NULL;

    line_ptrs = prepare_data (buffer, file_size, &correct_lines);


    // Sort straightly and write
    qsort (line_ptrs, correct_lines, sizeof (beg_end_ptr), &struct_cmp_reversed_line);

    write_sort_to_file ("pit/reversed_sorted_hamlet.txt", correct_lines, line_ptrs);


    // Sort reversely and write
    qsort (line_ptrs, correct_lines, sizeof (beg_end_ptr), &struct_cmp_beg_ptr);

    write_sort_to_file ("pit/straight_sorted_hamlet.txt", correct_lines, line_ptrs);


    // Origin text write
    write_buffer_to_file ("pit/true_hamlet.txt", buffer, file_size);


    // Free memory
    free (buffer);
    free (line_ptrs);

    return 0;
    }
