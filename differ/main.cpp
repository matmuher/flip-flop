#include <stdio.h>
#include <string.h>
#include "differ.h"

// ! var search -> e^x, etc
// ! reading from file

int main (int, char* argv[])
    {
    #if 0

    size_t file_size = get_size (file_name);
    FILE* fopen
    char* expression = elephant_calloc (file_size + 1, sizeof (char));
    safe_gets (expression, );

    #endif

    #if 1

        const char* expression = "(((()sin(x))^(2))+((sin((x)^(5)))))";

        tree_reader t_reader = {};

        t_reader.root = (char*) expression;
        t_reader.begunok = (char*) expression;


        node* tree_root = read_expression_recurs (&t_reader);

        // dot_this_shit (tree_root);

        #if 1

            node* diffed_tree_root = to_diff (tree_root);

            // dot_this_shit (diffed_tree_root);

        #endif

        #if 1

            optimize_node_recurs (diffed_tree_root);

             dot_this_shit (diffed_tree_root);

        #endif

    #endif


    #if 1

        FILE* tex_file = create_tex ("diff.tex");

        tex_dump_recurs (tex_file, diffed_tree_root);

        close_tex (tex_file);

    #endif

    memory_free ();
    }
