#include <stdio.h>
#include "ak.h"


void print_node (node* node_to_print)
    {
    // printf ("node address: %p\n", node_to_print);
    if (node_to_print != NULL)
        {
        printf (" %s \n", node_to_print->content);
        }
    else
        {
        printf (" empty \n");
        }
    }


// Mode idea generalize: node_visitor gets function pointer
void node_printor (node* current_node)
    {
    if (current_node != NULL)
        {
        print_node (current_node);

        node_printor (current_node->left_child);

        node_printor (current_node->right_child);
        }
    else
        {
        print_node (current_node);
        }
    }
