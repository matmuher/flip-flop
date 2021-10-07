#include <stdio.h>
#include <stdlib.h>
#include "..\hamlet_dynamic_proc\string_funk.h"
#include "..\hamlet_dynamic_proc\d_hamlet_functions.h"
#include "..\..\..\memory_free\elephant_calloc_extern.h"

int main()
    {
    struct line_buf* line_ptrs = NULL;
    size_t lines_num = 0;
    char* file_name = "hamlet.txt";

    line_ptrs = get_strings (file_name, &lines_num);

    prepare_data (line_ptrs, lines_num);

    line_buf* origin_lines = copy_line_buf (line_ptrs, lines_num);

    // Sort reversely and write
    qsort (line_ptrs, lines_num, sizeof (line_buf), &compare_line_reverse);

    write_line_buf_to_file ("pit/reversed_sorted_hamlet.txt", lines_num, line_ptrs);

    // Sort "zato svoe"
    bubble_sort (line_ptrs,  lines_num, sizeof (line_buf), &compare_line_straight);

    write_line_buf_to_file ("pit/rukami.txt", lines_num, line_ptrs);

    // Sort straightly and write
    qsort (line_ptrs, lines_num, sizeof (line_buf), &compare_line_straight);

    write_line_buf_to_file ("pit/straight_sorted_hamlet.txt", lines_num, line_ptrs);

    // Origin text write
    write_line_buf_to_file ("pit/origin.txt", lines_num, origin_lines);

    // Free memory
    memory_free ();

    return 0;
    }
