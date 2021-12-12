#include <string.h>
#include <stdio.h>
#include <ctype.h>


#include "differ.h"
#include "tree_funks.h"


node* read_expression_recurs (tree_reader* t_reader)
    {
    cur_read_pos (t_reader);

    if (*t_reader->begunok++ == '(')
        {
        if (*t_reader->begunok == '(')
            {
            node* node_oper = create_node (OP);

            // INORDER TREE READING
            //t_reader->begunok++;

            printf ("Create left oper child\n");
            puts (t_reader->begunok);
            node_oper->left_child = read_expression_recurs (t_reader);

            // Operator node content process
            const size_t OPER_LENGTH = 10;
            char oper_content[OPER_LENGTH] = {};
            sscanf (t_reader->begunok, "%[^()]s", oper_content);
            node_oper->content = transform_to_node_content (oper_content);

            // Shift begunok for next recursive reading
            t_reader->begunok += strlen (oper_content);

            printf ("Create right oper child\n");
            puts (t_reader->begunok);
            node_oper->right_child = read_expression_recurs (t_reader);

            t_reader->begunok++;

            return node_oper;
            }
        else
            {
            puts ("Finite reading");
            puts (t_reader->begunok);

            // Node content process
            const size_t VERTEX_LENGTH = 10;
            char vertex_content[VERTEX_LENGTH] = {};
            int scan_back = sscanf (t_reader->begunok, "%[^()]s", vertex_content);

            if (scan_back) // Not empty node
                {
                node* node_vertex = create_node (VERTEX);

                // Vertex is childfree :)
                node_vertex->left_child = NULL;
                node_vertex->right_child = NULL;

                if (isalpha (*t_reader->begunok))
                    {
                    node_vertex->ntype = VAR;
                    }
                else
                    {
                    node_vertex->ntype = VAL;
                    }

                node_vertex->content = transform_to_node_content (vertex_content);

                // Shift begunok for next recursive reading
                t_reader->begunok += strlen (vertex_content) + 1;

                return node_vertex;
                }

            t_reader->begunok += strlen (vertex_content) + 1;
            return NULL;
            }
        }
    }
