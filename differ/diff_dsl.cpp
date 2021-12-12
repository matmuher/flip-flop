#include "differ.h"
#include "tree_funks.h"


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


// (sin_f_x)' = cos * (f_x)'
node* diff_sin (node* cur_node)
    {
    node* diff_node = create_node (OP, "*");

    node* node_l = diff_node->left_child = create_node (OP, "cos");

    node_l->right_child = tree_copy_recurs (cur_node->right_child);

    diff_node->right_child = to_diff (cur_node->right_child);

    return diff_node;
    }


// (cos_f_x)' = (-1) * sin (f_x) * (f_x)'
node* diff_cos (node* cur_node)
    {
    node* diff_node = create_node (OP, "*");

    node* node_l = diff_node->left_child = create_node (VAL, "-1");

    node* node_r = diff_node->right_child = create_node (OP, "*");

    node* node_r_l = node_r->left_child = create_node (OP, "sin");
    node_r_l->right_child = tree_copy_recurs (cur_node->right_child);

    node_r->right_child = to_diff (cur_node->right_child);

    return diff_node;
    }
