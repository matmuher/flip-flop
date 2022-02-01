#include <stdio.h>
#include <stdlib.h>
#include "ak.h"



int main ()
    {
    #if 1

    node* root = start_akinator ();

    write_this_shit (root);

    dot_this_shit (root);

    #endif


    #if 0

    node* root = create_sign_node ("kotyata bivaut");


    root->left_child = create_sign_node ("iz francii");

    root->right_child = create_object_node ("rijii kotenok");

    root->left_child->left_child = create_object_node ("francuz");
    root->left_child->right_child = create_object_node ("belarus");


    write_this_shit (root);
    #endif


    #if 0

    tree matree = {};
    matree.tree_footprint_begunok = read_tree_to_str ();

    node* root = read_node_recursive (&matree);

    start_akinator (root);

    FILE* graph_dump = graph_dump_create ();

    graphviz_dump (root, graph_dump);

    graph_dump_close (graph_dump);


    #endif

    memory_free ();
    }
