#include <assert.h>
#include "..\differ\tree_funks.h"
#include "..\dictionary\dict.h"
#include "to_asm.h"


dict collect_vars (dict ma_dict, node* root)
    {
    ma_dict = try_node (ma_dict, root);

    if (root->left_child)
        {
        ma_dict = collect_vars (ma_dict, root->left_child);
        }

    if (root->right_child)
        {
        ma_dict = collect_vars (ma_dict, root->right_child);
        }

    return ma_dict;
    }


dict try_node (dict ma_dict, node* current_node)
    {
    // Shift relatively bx in current scope
    static size_t ram_shift = 0;

    if (current_node->ntype == VAR)
        {
        if (!search_in_dict (ma_dict, current_node->content))
            {
            ma_dict = add_dict_cell (ma_dict, current_node->content, ram_shift++);
            }
        }

    return ma_dict;
    }

void write_to_asm (const char* str)
    {
    static FILE* asm_file = fopen ("asm.txt", "w");

    fprintf (asm_file, str);
    }


void op_node_process (node* op_node, FILE* asm_file, dict ma_dict)
    {
    assert (op_node->ntype == OP);

    size_t ram_shift = 0;

    // Left node process
    node* left_node = op_node->left_child;
    if (left_node->ntype == VAR)
        {
        ram_shift = dict_get_val (ma_dict, left_node->content);
        fprintf (asm_file, "push [bx + %d]\n", dict_get_val);
        }
    else if (left_node->ntype == VAL)
        {
        fprintf (asm_file, "push %s\n", left_node->content);
        }

    // Right node process
    node* rigth_node = op_node->right_child;
    if (rigth_node->ntype == VAR)
        {
        ram_shift = dict_get_val (ma_dict, rigth_node->content);
        fprintf (asm_file, "push [bx + %d]\n", dict_get_val);
        }
    else if (rigth_node->ntype == VAL)
        {
        fprintf (asm_file, "push %s\n", rigth_node->content);
        }

    // Op process
    switch (op_node->content[0])
        {
        case '+':
            fprintf (asm_file, "add\n");
            break;
        case '-':
            fprintf (asm_file, "sub\n");
            break;
        case '*':
            fprintf (asm_file, "mlt\n");
            break;
        case '/':
            fprintf (asm_file, "saw\n");
            break;
        }
    }


void assembly (node* root, FILE* asm_file, dict ma_dict)
    {
    puts (root->content);

    if (root->ntype == OP && root->content[0])
        {
        puts ("Came in!");
        op_node_process (root, asm_file, ma_dict);
        }

    if (root->left_child)
        {
        puts ("Left");
        assembly (root->left_child, asm_file, ma_dict);
        }

    if (root->right_child)
        {
        puts ("Right");
        assembly (root->right_child, asm_file, ma_dict);
        }
    }



