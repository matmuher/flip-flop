#include <stdio.h>
#include <stdlib.h>
#include "ak.h"



int main ()
    {
    #if 0
    node* root = create_sign_node ("kotyata bivaut");


    root->left_child = create_sign_node ("iz francii");

    root->right_child = create_object_node ("rijii kotenok");

    root->left_child->left_child = create_object_node ("francuz");
    root->left_child->right_child = create_object_node ("belarus");


    FILE* tree_writer = tree_writer_create ();

    write_node_recursive (root, tree_writer);

    tree_writer_close (tree_writer);
    #endif


    #if 1

    tree matree = {};
    matree.tree_footprint_begunok = read_tree_to_str ();

    node* root = read_node_recursive (&matree);

    FILE* graph_dump = graph_dump_create ();

    graphviz_dump (root, graph_dump);

    graph_dump_close (graph_dump);

    memory_free ();
    #endif
    }
