#include <stdio.h>
#include <stdlib.h>
#include "ak.h"


FILE* graph_dump_create (void)
    {
    FILE* graph_dump = fopen ("ak.dot", "w");

    fputs ("digraph G {\n", graph_dump);


    return graph_dump;
    }


void graph_dump_show (void)
    {
    system ("dot ak.dot -Tpng -o ak.png");
    system ("start ak.png");
    }


void graph_dump_close (FILE* graph_dump)
    {
    fputs ("}\n", graph_dump);

    fclose (graph_dump);

    // Add show off regime
    graph_dump_show ();
    }


static void dot_connect_nuclear_family (node* parent_node, FILE* graph_log)
    {
    // Pointers are used as IDs
    if (parent_node != NULL)
        {
        fprintf (graph_log, "A%p [label = \"%s\"];\n", parent_node, parent_node->content);

        if (parent_node->left_child != NULL)
            {
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->left_child);
            }

        if (parent_node->right_child != NULL)
            {
            fprintf (graph_log, "A%p -> A%p;\n", parent_node, parent_node->right_child);
            }
        }
    }


void graphviz_dump (node* current_node, FILE* graph_log)
    {
    if (current_node != NULL)
        {
        dot_connect_nuclear_family (current_node, graph_log);

        graphviz_dump (current_node->left_child, graph_log);

        graphviz_dump (current_node->right_child, graph_log);
        }
    }


void dot_this_shit (node* root)
    {
    FILE* graph_dump = graph_dump_create ();

    graphviz_dump (root, graph_dump);

    graph_dump_close (graph_dump);
    }
