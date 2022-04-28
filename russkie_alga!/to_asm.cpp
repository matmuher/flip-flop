#include <assert.h>
#include "..\differ\tree_funks.h"
#include "..\dictionary\dict.h"
#include "..\differ\differ.h"
#include "to_asm.h"


void expression_assembly (node* cur_node, FILE* asm_file, dict ma_dict);

void expression_node_assembly (node* cur_node, FILE* asm_file, dict ma_dict);


void store_value_assembly (node* cur_node, FILE* asm_file, dict ma_dict);

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
        fprintf (asm_file, "push [bx + %d]\n", ram_shift);
        }
    else if (left_node->ntype == VAL)
        {
        fprintf (asm_file, "push %s\n", left_node->content);
        }

    // Right node process
    node* right_node = op_node->right_child;
    if (right_node->ntype == VAR)
        {
        ram_shift = dict_get_val (ma_dict, right_node->content);
        fprintf (asm_file, "push [bx + %d]\n", ram_shift);
        }
    else if (right_node->ntype == VAL)
        {
        fprintf (asm_file, "push %s\n", right_node->content);
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


void st_assembly (node* root, FILE* asm_file, dict ma_dict)
    {
    switch (root->right_child->ntype)
        {
        case INIT:
            {
            node* init_node = root->right_child;

            expression_assembly (init_node->right_child->right_child, asm_file, ma_dict);

            store_value_assembly (init_node->left_child, asm_file, ma_dict);

            break;
            }

        default:
            {
            printf ("[ERROR]: Error during processing of [%s]\n", root->content);

            break;
            }
        }
    }

void expression_assembly (node* root, FILE* asm_file, dict ma_dict)
    {
    if (root->left_child) expression_assembly (root->left_child, asm_file, ma_dict);

    if (root->right_child) expression_assembly (root->right_child, asm_file, ma_dict);

    expression_node_assembly (root, asm_file, ma_dict);
    }


void expression_node_assembly (node* cur_node, FILE* asm_file, dict ma_dict)
    {
    puts (cur_node->content);

    switch (cur_node->ntype)
        {
        case OP:
            {
            switch (cur_node->content[0])
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
            break;
            }

        case VAR:
            {
            size_t ram_shift = 0;

            ram_shift = dict_get_val (ma_dict, cur_node->content);

            fprintf (asm_file, "push [bx + %d]\n", ram_shift);

            break;
            }

        case VAL:
            {
            fprintf (asm_file, "push %s\n", cur_node->content);

            break;
            }

        default:
            {
            printf ("Unexpected member of expression: %s\n", cur_node->content);
            }
        }
    }


void store_value_assembly (node* cur_node, FILE* asm_file, dict ma_dict)
    {
    if (cur_node->ntype == VAR)
        {
        size_t ram_shift = 0;

        ram_shift = dict_get_val (ma_dict, cur_node->content);

        fprintf (asm_file, "pop [bx + %d]\n", ram_shift);
        }
    else
        {
        puts ("[ERROR]: Unable to initialize non-variable\n");
        }
    }



