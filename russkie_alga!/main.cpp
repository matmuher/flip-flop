#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"


int main ()
    {
    char* file_name = "wasup_world.cum";
    size_t lines_num = 0;
    line_buf* lines =  get_strings (file_name, &lines_num, true);


    const char* expression = "if(x=3){x:5-1*x-12/y+x^3}$";

    // "(sinus(5^2)+3)*(12-3)$"

    // const char* double_test = "11.$";

    // line_reader double_reader = {double_test, double_test};

    // printf ("Double is %f\n", get_N (&double_reader));

    // printf ("get_G returned %f", get_G (expression));


    token* parsed_line = lexo_parse (lines[0].beg_ptr);

    parsed_line_reader pl_reader = {0, parsed_line};

    #if 0 // Check lexo parse

    print_pl (&pl_reader);

    #endif

    #if 1 // Recursive parse
    node* root = get_G (&pl_reader);

    dot_this_shit (root);

    #endif
    memory_free ();
    }
