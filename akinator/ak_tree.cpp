#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"


node* create_node (const char* content_to_push)
    {
    char* node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
    strcpy (node_content, content_to_push);


    node* new_node = (node*) elephant_calloc (1, sizeof (node));
    new_node->content = node_content;


    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }


node* create_sign_node (const char* content)
    {
    node* sign_node = create_node (content);

    sign_node->node_type = SIGN;


    return sign_node;
    }


node* create_object_node (const char* content)
    {
    node* object_node = create_node (content);

    object_node->node_type = OBJECT;


    return object_node;
    }
