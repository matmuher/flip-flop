#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\dictionary\dict.h"
#include "to_asm.h"


node* build_ast (const char* file_name)
    {
    // Lexical parsing
    size_t lines_num = 0;

    line_buf* lines =  get_strings ((char*) file_name, &lines_num, true);

    token* parsed_line = lexo_parse (lines[0].beg_ptr);
    parsed_line = lexo_parse_second_traversal (parsed_line);
    parsed_line_reader pl_reader = {0, parsed_line};

    // Syntax parsing
    node* root = NULL;
    root = get_G (&pl_reader);

    return root;
    }


void compile (const char* file_name)
    {
    node* syntax_tree = build_ast (file_name);

    FILE* asm_file = fopen (, "w");

    st_assembly (syntax_tree, asm_file, NULL);
    }
