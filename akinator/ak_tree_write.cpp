#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"


FILE* tree_writer_create (void)
    {
    FILE* tree_writer = fopen ("tree_writer.cpp", "w");


    return tree_writer;
    }


void tree_writer_close (FILE* tree_writer)
    {
    fclose (tree_writer);
    }


void write_node_recursive (node* current_node, FILE* tree_writer)
    {
    assert (current_node);

    if (current_node->node_type == OBJECT)
        {
        fprintf (tree_writer, "{%s;}", current_node->content);
        }
    else if (current_node->node_type == SIGN)
        {
        fprintf (tree_writer, "{(%s)", current_node->content);

        write_node_recursive (current_node->left_child, tree_writer);
        write_node_recursive (current_node->right_child, tree_writer);

        fputs ("}", tree_writer);
        }

    putc ('\n', tree_writer);
    }
