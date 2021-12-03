#ifndef AK_H_INCLUDED
#define AK_H_INCLUDED

#include "..\memory_free\elephant_calloc_extern.h"
#include "stdlib.h"


enum VERBOSE_LVLS
    {
    ZERO_LVL = 0,
    FIRST_LVL = 1,
    SECOND_LVL = 2,
    };


// Tree stuff

struct node
    {
    const char* content; //<- C-type content string

    node* left_child;
    node* right_child;

    unsigned int node_type : 1; //<- SIGN or OBJECT
    };

enum node_type
    {
    SIGN = 0,
    OBJECT = 1,
    };

struct tree
    {
    node* root;
    char* tree_footprint_begunok;
    };

node* create_node (const char* content_to_push);

node* create_sign_node (const char* content);

node* create_object_node (const char* content);


// Input process

enum LOGIC
    {
    INCORRECT = -1,
    NO = 0,
    YES = 1,
    };

char* safe_gets (char* write_to_buffer, size_t buffer_size);

int get_answer (void);


// Dot dump

FILE* graph_dump_create (void);

void graph_dump_show (void);

void graph_dump_close (FILE* graph_dump);

void graphviz_dump (node* current_node, FILE* graph_log);


// Akinator

const int MAX_SIGN_NAME_LENGTH = 20;

const int MAX_OBJECT_NAME_LENGTH = 20;

node* start_akinator (void);


//  Tree write

FILE* tree_writer_create (void);

void tree_writer_close (FILE* tree_writer);

void write_node_recursive (node* current_node, FILE* tree_writer);


// Tree read

char* read_tree_to_str (void);

node* read_node_recursive (tree* matree);


// Help functions

int get_size (FILE* file_pointer);


#endif // AK_H_INCLUDED
