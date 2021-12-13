#ifndef TREE_FUNKS_H_INCLUDED
#define TREE_FUNKS_H_INCLUDED

#include "differ.h"


node* create_node (node_type ntype = UNDEF, const char* content_to_push = NULL);

/*!
@brief Equal to strdup, but uses elephant_calloc
*/
const char* transform_to_node_content (const char* content_to_push);

/*!
@brief Copies information from orig_node to dest_node
@details Both should exist
*/
node* copy_node (node* dest_node, node* orig_node);

/*!
@brief Creates new tree and copies origin tree there
*/
node* tree_copy_recurs (node* root);


#endif // TREE_FUNKS_H_INCLUDED
