#include <stdio.h>
#include "processor.h"
#include "..\strings\string_funk.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\square_solver\beer_equation.h"


int main ()
    {
    size_t lines_num = 0;
    line_buf* code = get_code ("square_code.txt", &lines_num);

    size_t bin_size = 0;
    int* binary = create_binary (code, lines_num, &bin_size);
    write_binary (binary, bin_size);

    stop_inst_console_close ();
    }

