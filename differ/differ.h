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


struct tree_reader // !Add ctor
    {
    char* root;
    char* begunok;
    };


// Read expression to tree

node* read_expression_recurs (tree_reader* t_reader);


// Differentiate expression

node* to_diff (node* root);


// Dot dump

FILE* dot_dump_create (void);

void dot_dump_show (void);

void dot_dump_close (FILE* graph_dump);

void tree_dot_dump (node* current_node, FILE* graph_log);


// Console dump

void cur_read_pos (tree_reader* t_reader);

#endif // DIFFER_H_INCLUDED
