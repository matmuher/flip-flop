#include <stdio.h>
#include "compile.h"


int main (int, char* argv[])
    {
    const char* file_name = argv[1] ? argv[1] : "square_eq_solve.cum";

    assembly (file_name);

    return 0;
    }


