#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"


node* create_node (const char* content_to_push = NULL)
    {
    char* node_content = NULL;


    if (content_to_push != NULL)
        {
        node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
        strcpy (node_content, content_to_push);
        }


    node* new_node = (node*) elephant_calloc (1, sizeof (node));
    new_node->content = node_content;


    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }


node* create_typed_node (const char* content, node_type ntype)
    {
    node* sign_node = create_node (content);

    sign_node->node_type = ntype;


    return sign_node;
    }


node* create_sign_node (const char* content)
    {
    return create_typed_node (content, SIGN);
    }


node* create_object_node (const char* content)
    {
    return create_typed_node (content, OBJECT);
    }




