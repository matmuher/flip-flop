#include <stdio.h>
#include "great_grammar.h"


int main ()
    {
    const char* expression = "1/0$";
    const char* double_test = "11.$";

    line_reader double_reader = {double_test, double_test};

    // printf ("Double is %f\n", get_N (&double_reader));

    printf ("get_G returned %f", get_G (expression));
    }
