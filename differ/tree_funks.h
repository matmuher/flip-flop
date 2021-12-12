#ifndef TREE_FUNKS_H_INCLUDED
#define TREE_FUNKS_H_INCLUDED

#include "differ.h"


node* create_node (node_type ntype = UNDEF, const char* content_to_push = NULL);

const char* transform_to_node_content (const char* content_to_push);

node* copy_node (node* dest_node, node* orig_node);

node* tree_copy_recurs (node* root);


#endif // TREE_FUNKS_H_INCLUDED
