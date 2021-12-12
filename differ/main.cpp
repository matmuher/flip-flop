#include <stdio.h>
#include <string.h>
#include "differ.h"


int main ()
    {
    #if 1
    const char* expression = "((x)/((x)^(2)))";

    tree_reader t_reader = {};

    t_reader.root = (char*) expression;
    t_reader.begunok = (char*) expression;


    node* tree_root = read_expression_recurs (&t_reader);

    #if 1

    node* diffed_tree_root = to_diff (tree_root);

    #endif

    #if 1
    FILE* dot_file = dot_dump_create ();

    tree_dot_dump (diffed_tree_root, dot_file);

    dot_dump_close (dot_file);
    #endif

    #endif

    memory_free ();
    }
