#include "differ.h"


FILE* create_tex (const char* name)
    {
    FILE* tex_file = fopen (name, "w");

    fputs ("\\documentclass[a4paper,18pt]{article}\n\n"

           "\\begin {document}\n\n"
           "$ ", tex_file);

    return tex_file;
    }


void close_tex (FILE* tex_file)
    {
    fputs ("$ \n\n \\end {document}\n", tex_file);

    fclose (tex_file);
    }


void tex_dump_recurs (FILE* tex_file, node* root)
    {
    if (root)
        {
        if (root->ntype == OP)
            {
            putc ('(', tex_file);
            }

        if (root->left_child)
            {
            tex_dump_recurs (tex_file, root->left_child);
            }

        if (*root->content == '*')
            {
            fputs (" \\cdot ", tex_file);
            }
        else if (root->ntype == OP)
            {
            fprintf (tex_file, " %s ", root->content);
            }
        else
            {
            fprintf (tex_file, " {%s} ", root->content);
            }

        if (root->right_child)
            {
            tex_dump_recurs (tex_file, root->right_child);
            }

        if (root->ntype == OP)
            {
            putc (')', tex_file);
            }
        }
    }
