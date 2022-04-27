#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"

void to_asm (node* root);

node* build_ast (const char* file_name);

int main (int, char* argv[])
    {
    const char* file_name = argv[1] ? argv[1] : "wasup_world.cum";

    node* root = build_ast (file_name);

    system ("pause");

    dot_this_shit (root);

    // to_asm (root);

    memory_free ();
    }

node* build_ast (const char* file_name)
    {
    // Lexical parsing
    size_t lines_num = 0;
    line_buf* lines =  get_strings ((char*) file_name, &lines_num, true);
    token* parsed_line = lexo_parse (lines[0].beg_ptr);
    parsed_line_reader pl_reader = {0, parsed_line};
    print_pl (&pl_reader);
    system("pause");
    // Syntax parsing
    return get_G (&pl_reader);
    }
