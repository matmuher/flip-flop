#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ak.h"


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

char* read_tree_to_str (void)
    {
    FILE* tree_footprint = fopen ("tree_writer.cpp", "r");
    int tree_file_size = get_size (tree_footprint);

    char* tree_line = (char*) elephant_calloc (tree_file_size, sizeof (char));
    fread (tree_line, sizeof (char), tree_file_size, tree_footprint);

    tree_line[tree_file_size - 1] = '\0';

    fclose (tree_footprint);


    return tree_line;
    }


node* read_node_recursive (tree* matree)
    {
    if (*matree->tree_footprint_begunok++ == '{')
        {
        if (*matree->tree_footprint_begunok == '(')
            {
            char* sign_start = matree->tree_footprint_begunok + 1;
            char sign[MAX_SIGN_NAME_LENGTH] = {};

            matree->tree_footprint_begunok = strchr (matree->tree_footprint_begunok, ')');
            *matree->tree_footprint_begunok++ = '\0';

            sscanf (sign_start, "%s", sign);
            node* current_node = create_sign_node (sign);

            current_node->left_child = read_node_recursive (matree);
            current_node->right_child = read_node_recursive (matree);

            matree->tree_footprint_begunok++;


            return current_node;
            }
        else
            {
            char object[MAX_OBJECT_NAME_LENGTH] = {};
            char* object_start = matree->tree_footprint_begunok;

            matree->tree_footprint_begunok = strchr (matree->tree_footprint_begunok, ';');

            // We should skip ';'
            *matree->tree_footprint_begunok++ = '\0';
            // and '}'
            matree->tree_footprint_begunok++;

            sscanf (object_start, " %s; ", object);
            node* current_node = create_object_node (object);


            return current_node;
            }
        }
    }
