#include <string.h>
#include <ctype.h>
#include "..\memory_free\elephant_calloc_extern.h"
#include "differ.h"

void cur_read_pos (tree_reader* t_reader)
    {
    puts (t_reader->root);

    int pointer = t_reader->begunok - t_reader->root + 1;

    printf ("%*c\n", pointer, '^');
    }

node* create_node (node_type ntype, const char* content_to_push)
    {
    // Create node
    node* new_node = (node*) elephant_calloc (1, sizeof (node));

    // node content
    char* node_content = NULL;

    if (content_to_push != NULL)
        {
        node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
        strcpy (node_content, content_to_push);
        }

    new_node->content = node_content;


    // node type
    new_node->ntype = ntype;


    // node children
    new_node->left_child = NULL;
    new_node->right_child = NULL;


    return new_node;
    }


const char* transform_to_node_content (const char* content_to_push)
    {
    char* node_content = (char*) elephant_calloc (1, (strlen (content_to_push) + 1) * sizeof (char));
    strcpy (node_content, content_to_push);

    return (const char*) node_content;
    }


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
            node* node_vertex = create_node (VERTEX);

            // Vertex is childfree :)
            node_vertex->left_child = NULL;
            node_vertex->right_child = NULL;

            // Content process
            const size_t VERTEX_LENGTH = 10;
            char vertex_content[VERTEX_LENGTH] = {};
            sscanf (t_reader->begunok, "%[^()]s", vertex_content);

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
        }
    }


node* copy_node (node* dest_node, node* orig_node)
    {
    dest_node->content = transform_to_node_content (orig_node->content);

    dest_node->ntype = orig_node->ntype;

    dest_node->left_child = orig_node->left_child;
    dest_node->right_child = orig_node->right_child;

    return dest_node;
    }


node* tree_copy_recurs (node* root)
    {
    node* copy_root = (node*) elephant_calloc (1, sizeof (node));

    copy_node (copy_root, root);

    if (root->left_child)
        {
        copy_root->left_child = tree_copy_recurs (root->left_child);
        }

    if (root->right_child)
        {
        copy_root->right_child = tree_copy_recurs (root->right_child);
        }

    return copy_root;
    }


enum comparison
    {
    EQUAL = 0,
    };


// (f_x + g_x)' = f_x' + g_x'
node* diff_sum (node* cur_node)
    {
    node* diff_node = create_node (OP);

    if (*cur_node->content == '+')
        {
        diff_node->content = transform_to_node_content ("+");
        }
    else if (*cur_node->content == '-')
        {
        diff_node->content = transform_to_node_content ("-");
        }

    diff_node->left_child = to_diff (cur_node->left_child);
    diff_node->right_child = to_diff (cur_node->right_child);

    return diff_node;
    }


// (f_x * g_x)' = f_x' * g_x + g_x' * f_x
node* diff_mlt (node* cur_node)
    {
    node* diff_node = create_node (OP, "+");

    node* node_l = diff_node->left_child = create_node (OP, "*");
    node_l->left_child = to_diff (cur_node->left_child);
    node_l->right_child = tree_copy_recurs (cur_node->right_child);

    node* node_r = diff_node->right_child = create_node (OP, "*");
    node_r->left_child = to_diff (cur_node->right_child);
    node_r->right_child = tree_copy_recurs (cur_node->left_child);

    return diff_node;
    }


// ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
node* diff_pow (node* cur_node)
    {
    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //-----------------^
    node* diff_node = create_node (OP, "*");

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //---------------^
    diff_node->left_child = create_node (VAL, cur_node->right_child->content);

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //-----------------------------------^
    // node_r = ONE right step from start node
    node* node_r = diff_node->right_child = create_node (OP, "*");

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //---------------------------------------^
    node_r->right_child = to_diff (cur_node->left_child);

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //-------------------------^
    // node_r_l = ONE right, ONE left from start node
    node* node_r_l = node_r->left_child = create_node (OP, "^");

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //---------------------^
    node_r_l->left_child = tree_copy_recurs
                                (cur_node->left_child);

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //------------------------------^
    node* node_r_l_r = node_r_l->right_child = create_node (OP, "-");

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //----------------------------^
    node_r_l_r->left_child = create_node (VAL, cur_node->right_child->content);

    // ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
    //--------------------------------^
    node_r_l_r->right_child = create_node (VAL, "1");


    return diff_node;
    }


// (x / y) = (x * y ^ (-1))
node* transform_division (node* cur_node)
    {
    cur_node->content = transform_to_node_content ("*");

    node* denominator = cur_node->right_child;

    node* div_node = create_node (OP, "^");
    div_node->left_child = denominator;
    div_node->right_child = create_node (VAL, "-1");

    cur_node->right_child = div_node;

    return cur_node;
    }


node* to_diff (node* cur_node)
    {
    if (cur_node->ntype == VAL)
        {
        node* diff_node = create_node (VAL, "0");

        return diff_node;
        }
    else if (cur_node->ntype == VAR)
        {
        node* diff_node = create_node (VAL, "1");

        return diff_node;
        }
    else if (cur_node->ntype == OP)
        {
        if (strcmp (cur_node->content, "+") == EQUAL ||
            strcmp (cur_node->content, "-") == EQUAL)
            {
            return diff_sum (cur_node);
            }
        else if (strcmp (cur_node->content, "*") == EQUAL)
            {
            return diff_mlt (cur_node);
            }
        else if (strcmp (cur_node->content, "^") == EQUAL)
            {
            return diff_pow (cur_node);
            }
        else if (strcmp (cur_node->content, "/") == EQUAL)
            {
            // (x / y)' = (x * y ^ (-1))'
            return diff_mlt (transform_division (cur_node));
            }
        }
    }


