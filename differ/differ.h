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

    node_type ntype;
    };


struct tree_reader // !Add ctor
    {
    char* root;
    char* begunok;
    };


// Read expression to tree

node* read_expression_recurs (tree_reader* t_reader);


// Differentiate expression [CREATES NEW TREE]

node* to_diff (node* root);


// Dot dump

FILE* dot_dump_create (void); // !Add setting file_name

void dot_dump_show (void);

void dot_dump_close (FILE* graph_dump);

void tree_dot_dump (node* current_node, FILE* graph_log);


// Tex dump

FILE* create_tex (const char* name);

void tex_dump_recurs (FILE* tex_file, node* root);

void close_tex (FILE* tex_file);


// Console dump

void cur_read_pos (tree_reader* t_reader);


// Optimizer [CHANGES CURRENT TREE !!!]

node* optimize_node_recurs (node* root); // !Add optimize until there are changes in the tree
                                         // Could be everlasting changes?


// For comparison in differ.cpp and optimizer.cpp

enum comparison
    {
    EQUAL = 0,
    };

#endif // DIFFER_H_INCLUDED
