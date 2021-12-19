#include "..\differ\tree_funks.h"
#include "..\differ\differ.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void op_node_proc (node* op_node, FILE* alga_file)
    {
    if (*op_node->content == '+')
        {
        fputs ("add\n", alga_file);
        }
    else if (strcmp (op_node->content, "print") == 0)
        {
        fputs ("out\n", alga_file);
        }
    }

void print_node (node* cur_node)
    {
    FILE* alga_file = fopen ("alga.txt", "a");

    switch (cur_node->ntype)
        {
        case OP:
            {
            op_node_proc (cur_node, alga_file);
            break;
            }
        case VAL:
            {
            char* value[10] = {};
            sscanf (cur_node->content, "%[^.]", value);
            fprintf (alga_file, "push %s\n", value);
            break;
            }
        }

    fclose (alga_file);
    }

void to_asma (node* root)
    {
    tree_visitor (root, print_node, POST);
    }



