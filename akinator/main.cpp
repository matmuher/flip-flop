#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"

FILE* tree_writer_create (void)
    {
    FILE* tree_writer = fopen ("tree_writer.cpp", "w");

    return tree_writer;
    }


void tree_writer_close (FILE* tree_writer)
    {
    fclose (tree_writer);
    }


// Write root to file
void write_node_recursive (node* current_node, FILE* tree_writer)
    {
    assert (current_node);

    if (current_node->node_type == OBJECT)
        {
        fprintf (tree_writer, "{ %s; } ", current_node->content);
        }
    else if (current_node->node_type == SIGN)
        {
        fprintf (tree_writer, "{ (%s) ", current_node->content);

        write_node_recursive (current_node->left_child, tree_writer);
        write_node_recursive (current_node->right_child, tree_writer);

        fputs ("} ", tree_writer);
        }

    putc ('\n', tree_writer);
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


node* read_node_recursive (char* tree_footprint_begunok)
    {
    if (*tree_footprint_begunok++ == '{')
        {
        if (*tree_footprint_begunok == '(')
            {
            char* sign_start = tree_footprint_begunok + 1;
            char sign[MAX_SIGN_NAME_LENGTH] = {};

            tree_footprint_begunok = strchr (tree_footprint_begunok, ')');
            *tree_footprint_begunok++ = '\0';

            sscanf (sign_start, "%s", sign);
            printf ("Now processing sign: %s\n", sign);

            node* current_node = create_sign_node (sign);

            current_node->left_child = read_node_recursive (tree_footprint_begunok);
            current_node->right_child = read_node_recursive (tree_footprint_begunok);

            return current_node;
            }
        else
            {
            char object[MAX_OBJECT_NAME_LENGTH] = {};
            char* object_start = tree_footprint_begunok;

            tree_footprint_begunok = strchr (tree_footprint_begunok, ';');
            *tree_footprint_begunok++ = '\0';

            sscanf (object_start, " %s; ", object);
            printf ("Now processing object: %s\n", object);

            node* current_node = create_object_node (object);


            return current_node;
            }
        }
    else if (*tree_footprint_begunok == '\0')
        {
        puts ("[SYSTEM] Digitalizing of tree was successfully ended!");
        }
    }


int get_size (FILE* file_pointer)
    {
    assert (file_pointer != NULL);

    size_t cur_pos = ftell (file_pointer);

    // Get size
    fseek (file_pointer, 0, SEEK_END);
    int file_size = ftell (file_pointer);

    // Return internal pointer
    fseek (file_pointer, cur_pos, SEEK_SET);

    return file_size;
    }


int main ()
    {
    #if 0
    node* root = create_sign_node ("kotyata bivaut");


    root->left_child = create_sign_node ("iz francii");

    root->right_child = create_object_node ("rijii kotenok");

    root->left_child->left_child = create_object_node ("francuz");
    root->left_child->right_child = create_object_node ("belarus");


    FILE* tree_writer = tree_writer_create ();

    write_node_recursive (root, tree_writer);

    tree_writer_close (tree_writer);
    #endif

    FILE* tree_footprint = fopen ("tree_writer.cpp", "r");
    int tree_file_size = get_size (tree_footprint);

    char* tree_line = (char*) calloc (tree_file_size, sizeof (char));
    fread (tree_line, sizeof (char), tree_file_size, tree_footprint);
    tree_line[tree_file_size - 1] = '\0';

    puts (tree_line);

    fclose (tree_footprint);
    #if 1
    node* root = read_node_recursive (tree_line);

    FILE* graph_dump = graph_dump_create ();

    graphviz_dump (root, graph_dump);

    graph_dump_close (graph_dump);
    #endif
    }
