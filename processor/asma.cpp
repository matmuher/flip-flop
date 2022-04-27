#include <stdio.h>
#include "processor.h"
#include "..\strings\string_funk.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\square_solver\beer_equation.h"


int main (int, char* argv[])
    {
    size_t lines_num = 0;

    char* file_name = argv[1] ? argv[1] : (char*) "fib_code.txt";

    printf ("Assembling file is %s\n", file_name);

    line_buf* code = get_code (file_name, &lines_num);

    size_t bin_size = 0;
    int* binary = create_binary (code, lines_num, &bin_size);
    write_binary (binary, bin_size);

    system ("pause");
    }

