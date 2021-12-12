#ifndef DIFFER_H_INCLUDED
#define DIFFER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "..\memory_free\elephant_calloc_extern.h"

enum node_type
    {
    UNDEF = 0,
    VERTEX = 1,
    OP = 2,
    VAL = 3,
    VAR = 4,
    };


struct node
    {
    const char* content; //<- C-type content string

    node* left_child;
    node* right_child;

    node_type ntype; //<- SIGN or OBJECT
    };


struct tree_reader
    {
    char* root;
    char* begunok;
    };


node* read_expression_recurs (tree_reader* t_reader);

node* create_node (node_type ntype = UNDEF, const char* content_to_push = NULL);

const char* transform_to_node_content (const char* content_to_push);

node* tree_copy_recurs (node* root);

node* to_diff (node* cur_node);

// Dot dump

FILE* dot_dump_create (void);

void dot_dump_show (void);

void dot_dump_close (FILE* graph_dump);

void tree_dot_dump (node* current_node, FILE* graph_log);

#endif // DIFFER_H_INCLUDED
