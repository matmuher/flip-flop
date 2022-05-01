#include <stdio.h>
#include <stdlib.h>
#include "differ.h"


FILE* dot_dump_create (void)
    {
    FILE* graph_dump = fopen ("parse.dot", "w");

    fputs ("digraph G {\n", graph_dump);

    return graph_dump;
    }


void dot_dump_show (void)
    {
    system ("dot parse.dot -Tpng -o parse.png");
    system ("start parse.png");
    }


void dot_dump_close (FILE* graph_dump)
    {
    fputs ("}\n", graph_dump);

    fclose (graph_dump);

    // Add show off regime
    dot_dump_show ();
    }


static void dot_connect_nuclear_family (node* parent_node, FILE* graph_log)
    {
    puts (parent_node->content);

    // Pointers are used as IDs
    if (parent_node != NULL)
        {
        fprintf (graph_log, "A%p [label = \"%s\"];\n", parent_node, parent_node->content);

        if (parent_node->left_child != NULL)
            {
            puts (parent_node->left_child->content);
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->left_child);
            }

        if (parent_node->right_child != NULL)
            {
            puts (parent_node->right_child->content);
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->right_child);
            }
        }
    }


void tree_dot_dump (node* current_node, FILE* graph_log)
    {

    if (current_node != NULL)
        {
        dot_connect_nuclear_family (current_node, graph_log);

        tree_dot_dump (current_node->left_child, graph_log);

        tree_dot_dump (current_node->right_child, graph_log);
        }
    }


void dot_this_shit (node* root)
    {
    FILE* dot_file = dot_dump_create ();

    tree_dot_dump (root, dot_file);

    dot_dump_close (dot_file);
    }
