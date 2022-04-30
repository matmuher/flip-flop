#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"
#include "..\hamlet\d_hamlet_functions.h"
#include "..\dictionary\dict.h"
#include "to_asm.h"



int main (int, char* argv[])
    {
    const char* file_name = argv[1] ? argv[1] : "text.cum";

    node* root = build_ast (file_name);

    // dot_this_shit (root);

    system ("pause");

    #if 1
    FILE* asm_file = fopen ("asm.txt", "w");

    ma_dict = NULL;
    fprintf (asm_file, "push 0\n"
                       "pop bx\n"
                       "jump :main\n\n");
    st_assembly (root, asm_file, ma_dict);

    system ("pause");

    fclose (asm_file);
    #endif

    memory_free ();
    }


