#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"


node* create_node (const char* content_to_push)
    {
    char* node_content = (char*) calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
    strcpy (node_content, content_to_push);

    puts (d);
    node* new_node = (node*) calloc (1, sizeof (node));
    new_node->content = node_content;


    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }
