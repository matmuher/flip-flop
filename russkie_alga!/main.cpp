#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\dictionary\dict.h"
#include "to_asm.h"

void to_asm (node* root);

node* build_ast (const char* file_name);

int main (int, char* argv[])
    {
    const char* file_name = argv[1] ? argv[1] : "text.cum";

    node* root = build_ast (file_name);

    // dot_this_shit (root);

    dict ma_dict = NULL;
    // ma_dict = collect_vars (ma_dict, root);
    // print_dict (ma_dict);

    system ("pause");

    #if 1
    FILE* asm_file = fopen ("asm.txt", "w");

    ma_dict = NULL;
    st_assembly (root, asm_file, ma_dict);

    system ("pause");

    fclose (asm_file);
    #endif

    memory_free ();
    }

node* build_ast (const char* file_name)
    {
    // Lexical parsing
    size_t lines_num = 0;
    line_buf* lines =  get_strings ((char*) file_name, &lines_num, true);
    token* parsed_line = lexo_parse (lines[0].beg_ptr);
    parsed_line = lexo_parse_second_traversal (parsed_line);
    parsed_line_reader pl_reader = {0, parsed_line};
    print_pl (&pl_reader);
    system("pause");
    // Syntax parsing
    node* root = NULL;
    root = get_G (&pl_reader);
    return root;
    }
