#include <string.h>

#include "differ.h"


node* create_node (node_type ntype, const char* content_to_push)
    {
    // Create node
    node* new_node = (node*) elephant_calloc (1, sizeof (node));

    if(!new_node)
        {
        puts ("Node calloc error!");
        }

    // node content
    char* node_content = NULL;

    if (content_to_push != NULL)
        {
        node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));

        if(!node_content)
        {
        puts ("Node content calloc error!");
        }

        strcpy (node_content, content_to_push);
        }

    new_node->content = node_content;


    // node type
    new_node->ntype = ntype;


    // node children
    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }


const char* transform_to_node_content (const char* content_to_push)
    {
    char* node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
    strcpy (node_content, content_to_push);

    return (const char*) node_content;
    }


node* copy_node (node* dest_node, node* orig_node)
    {
    dest_node->content = transform_to_node_content (orig_node->content);

    dest_node->ntype = orig_node->ntype;

    dest_node->left_child = orig_node->left_child;
    dest_node->right_child = orig_node->right_child;

    return dest_node;
    }


node* tree_copy_recurs (node* root)
    {
    node* copy_root = (node*) elephant_calloc (1, sizeof (node));

    copy_node (copy_root, root);

    if (root->left_child)
        {
        copy_root->left_child = tree_copy_recurs (root->left_child);
        }

    if (root->right_child)
        {
        copy_root->right_child = tree_copy_recurs (root->right_child);
        }

    return copy_root;
    }





node* tree_visitor (node* root, void (*func)(node*), visit_mode v_mode, traversal_mode t_mode)
    {
    if (v_mode == PRE)
        {
        func (root);
        }

    if (t_mode == LEFT)
        {
        if (root->left_child)
            {
            tree_visitor (root->left_child, func, v_mode);
            }
        }
     else if (t_mode == RIGHT)
        {
        if (root->right_child)
            {
            tree_visitor (root->right_child, func, v_mode);
            }
        }

    if (v_mode == IN)
        {
        func (root);
        }

    if (t_mode == LEFT)
        {
        if (root->right_child)
            {
            tree_visitor (root->right_child, func, v_mode);
            }
        }
    else if (t_mode == RIGHT)
        {
        if (root->left_child)
            {
            tree_visitor (root->left_child, func, v_mode);
            }
        }

    if (v_mode == POST)
        {
        func (root);
        }
    }
