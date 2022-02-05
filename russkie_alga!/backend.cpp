#include "..\differ\tree_funks.h"
#include "..\differ\differ.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "backend.h"


// Operator node process


void alga_to_asm (node* op_node, FILE* alga_file)
    {
    #define asm_correspond(lang_oper, asm_oper)                    \
                else if (are_equal (op_node->content, #lang_oper)) \
                    {                                              \
                    fputs (#asm_oper " ", alga_file);                  \
                    }

    if (2+2==5)
        {
        fputs ("War is peace\n", alga_file);
        fputs ("And it never changes\n", alga_file);
        }
    asm_correspond(print, out)
    asm_correspond(-,sub)
    asm_correspond(+,add)
    asm_correspond(*,mlt)
    asm_correspond(>,ja)
    asm_correspond(<,jb)
    asm_correspond(==,je)

    #undef asm_correspond
    }


void asm_print_node (node* cur_node)
    {
    // !Cringe: better to pass FILE* as an argument
    FILE* alga_file = fopen ("alga.txt", "a");

    switch (cur_node->ntype)
        {
        case OP:
            {
            alga_to_asm (cur_node, alga_file);
            putc ('\n', alga_file);
            break;
            }
        case VAL:
            {
            double val = strtod (cur_node->content, NULL);
            int value = round (val);
            fprintf (alga_file, "push %d\n", value);
            break;
            }
        case VAR:
            {
            // Let's agree that variables should be named as registers so far
            puts ("I ve entered VAR branch!");
            puts (cur_node->content);
            fprintf (alga_file, "push %s\n", cur_node->content);
            break;
            }
        }

    fclose (alga_file);
    }


int fprint_expression (node* expression_root)
    {
    tree_visitor (expression_root, asm_print_node, POST, RIGHT);
    }


void make_mark (size_t mark_id, FILE* alga_file)
    {
    fprintf (alga_file, ":%d\n", mark_id);

    fflush (alga_file);
    }


void make_cond_jump (node* conditional_node, size_t mark_id, FILE* alga_file)
    {
    // Push left and right expressions
    fprint_expression (conditional_node->right_child);
    fprint_expression (conditional_node->left_child);

    alga_to_asm (conditional_node, alga_file);
    make_mark (mark_id, alga_file);
    }



#define fpush(arg) fprintf (alga_file, "push %s\n", arg)
#define fassign(var) fprintf (alga_file, "pop %s\n", var)
#define fprint(str) fputs (str, alga_file)
void backend_it (node* root)
    {
    FILE* alga_file = fopen ("alga.txt", "a");

    size_t mark_id = 0;

    switch (root->ntype)
        {
        case SFRAME:
            {
            if (are_equal (root->content, "="))
                {
                assert (root->left_child->ntype == VAR && "there should be var before assignment (=)\n");

                // !In fact we are making traversal twice - may be it is more logical
                // to do main traversal via INORDER?
                fprint_expression (root->right_child);
                fassign(root->left_child->content);
                }
            else if (are_equal (root->content, "while"))
                {
                puts ("Processing while!");
                size_t cur_mark = mark_id++;
                make_mark (cur_mark, alga_file);

                tree_visitor (root->right_child, backend_it, POST);

                // !!KOSTYL!! to avoid double use of backend_it in this "if" and
                // according to to_asm () traversal
                root->right_child = NULL;

                make_cond_jump (root->left_child, cur_mark, alga_file);

                root->left_child = NULL;
                }
            break;
            }
        case SFUNK:
            {
            fprint_expression (root->right_child);
            alga_to_asm (root, alga_file);
            putc ('\n', alga_file);
            break;
            }
        }

    fclose (alga_file);
    }


void to_asm (node* root)
    {
    // Clear alga.txt file
    FILE* alga_file = fopen ("alga.txt", "w");
    fclose (alga_file);

    tree_visitor (root, backend_it, IN);

    system ("start wasup_world.cum");
    system ("start alga.txt");
    }



