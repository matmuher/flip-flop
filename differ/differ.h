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
    SFUNK = 5,
    SFRAME = 6,
    DEF = 7,
    CALL = 8,
    UFUNK = 9,
    PARAM = 10,
    ST = 11,
    CMP = 12,
    FUNK = 13,
    INIT = 14,
    RET = 15,
    };


struct node
    {
    const char* content; //<- C-type content string

    node* left_child;
    node* right_child;

    node_type ntype;
    };



//=============================================================================\\
//                               Tree functions                                \\
//=============================================================================\\

enum visit_mode
    {
    PRE = -1, // Root ->left -> right: like in copying root
    IN = 0,   // Left -> root -> right: like in human's algebra
    POST = 1, // Left -> right -> root: like in assembler
    };

enum traversal_mode
    {
    RIGHT = -1, // Right leaf is processed earlier than right one
    LEFT = 1, // Vice versa
    };

node* tree_visitor (node* root, void (*func)(node*), visit_mode mode, traversal_mode = LEFT);


//=============================================================================\\
//                          Read expression to tree                            \\
//=============================================================================\\

struct tree_reader // !Add ctor
    {
    char* root;
    char* begunok; //<- Current position in string, that is being read
    };

/*!
@brief Read math expression from C-string to tree structure
@params[in] t_reader Structure that helps synchronize working with
                     begunok for recursive calls
@return Pointer to root of tree which was expression parsed to
*/
node* read_expression_recurs (tree_reader* t_reader);


//=============================================================================\\
//                Differentiate expression [CREATES NEW TREE]                  \\
//=============================================================================\\

/*!
@brief Differentiate tree'ed expression
@details Origin tree isn't changed. New tree for diff' expression is created
@params[in] cur_node pointer to node that should be differentiated
@return Pointer new tree with differentiated expression
*/
node* to_diff (node* root); // !Add differentiating for different variables


//=============================================================================\\
//                                 Dot dump                                    \\
//=============================================================================\\

void dot_this_shit (node* root);

FILE* dot_dump_create (void); // !Add setting file_name

void dot_dump_show (void); // !Merge create and recurs functions

void dot_dump_close (FILE* graph_dump);

void tree_dot_dump (node* current_node, FILE* graph_log);


//=============================================================================\\
//                                 Tex dump                                    \\
//=============================================================================\\

FILE* create_tex (const char* name);

void tex_dump_recurs (FILE* tex_file, node* root); // !Merge create and recurs functions

void close_tex (FILE* tex_file);


//=============================================================================\\
//                             Console dump                                    \\
//=============================================================================\\

/*!
@brief Points out current position of begunok
*/
void cur_read_pos (tree_reader* t_reader);


//=============================================================================\\
//                   Optimizer [CHANGES CURRENT TREE !!!]                      \\
//=============================================================================\\

/*!
@brief Optimizes tree'ed expression
@details Origin tree is changed
@params[in] oper pointer to node that should be optimized
@return Pointer to optimized tree root
*/
node* optimize_node_recurs (node* root); // !Add optimize until there are changes in the tree
                                         // Could be everlasting changes?



//=============================================================================\\
//                                     Input                                   \\
//=============================================================================\\

/*!
@brief Read [buffer_size] bytes to [write_to_buffer] from [file_stream]
@details Wrap of fget_s + clear of buffer
@params[in] write_to_buffer buffer to write
@params[in] write_to_buffer how many chars to read
@params[in] file_stream read from here
@return Pointer to written buffer
*/
char* safe_gets (char* write_to_buffer, size_t buffer_size, FILE* file_stream);


//=============================================================================\\
//               For comparison in differ.cpp and optimizer.cpp                \\
//=============================================================================\\

enum comparison
    {
    EQUAL = 0,
    };

#endif // DIFFER_H_INCLUDED
