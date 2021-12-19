#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"



int main ()
    {
    const char* expression = "(sinus(5^2)+3)*(12-3)$";

    // "(sinus(5^2)+3)*(12-3)$"

    // const char* double_test = "11.$";

    // line_reader double_reader = {double_test, double_test};

    // printf ("Double is %f\n", get_N (&double_reader));

    // printf ("get_G returned %f", get_G (expression));

    token* parsed_line = lexo_parse (expression);

    parsed_line_reader pl_reader = {0, parsed_line};

    print_pl (&pl_reader);

    #if 1
    node* root = get_G (&pl_reader);

    dot_this_shit (root);

    #endif
    memory_free ();
    }
