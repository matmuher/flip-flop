#include "differ.h"
#include "tree_funks.h"
#include "diff_dsl.h"


#define LEFT(node) (node)->left_child

#define RIGHT(node) (node)->right_child

#define CONTENT(node) (node)->content

#define D(node) to_diff (node)

#define COPY(node) tree_copy_recurs (node)


node* bi_oper (node* left, node* right, const char* oper)
    {
    node* root = create_node (OP, oper);

    LEFT(root) = left;
    RIGHT(root) = right;

    return root;
    }


node* mono_oper (node* right, const char* oper)
    {
    node* root = create_node (OP, oper);

    RIGHT(root) = right;

    return root;
    }


node* sinus (node* arg)
    {
    return mono_oper (arg, "sin");
    }


node* cosinus (node* arg)
    {
    return mono_oper (arg, "cos");
    }


node* add (node* left, node* right)
    {
    return bi_oper (left, right, "+");
    }


node* sub (node* left, node* right)
    {
    return bi_oper (left, right, "-");
    }


node* mlt (node* left, node* right)
    {
    return bi_oper (left, right, "*");
    }


node* pow (node* left, node* right)
    {
    return bi_oper (left, right, "^");
    }


node* create_val (const char* val)
    {
    return create_node (VAL, val);
    }


/*!
@brief (f_x + g_x)' = f_x' + g_x'
*/
node* diff_sum (node* cur_node)
    {
    if (*CONTENT(cur_node) == '+')
        {
        return add (D(LEFT(cur_node)), D(RIGHT(cur_node)) );
        }
    else if (*CONTENT(cur_node) == '-')
        {
        return sub (D(LEFT(cur_node)), D(RIGHT(cur_node)) );
        }

    return NULL;
    }

/*!
@brief (f_x * g_x)' = f_x' * g_x + g_x' * f_x
*/
node* diff_mlt (node* cur_node)
    {
    node* dfx_mlt_gx = mlt (D(LEFT(cur_node)), RIGHT(cur_node) );
    node* dgx_mlt_fx = mlt (D(RIGHT(cur_node)), LEFT(cur_node) );

    node* temp_dump = add (dfx_mlt_gx, dgx_mlt_fx);

    return temp_dump;
    }

/*!
@brief ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
*/
node* diff_pow (node* cur_node)
    {
    const char* A_st = CONTENT(RIGHT(cur_node));
    node* fx = COPY(LEFT(cur_node));
    node* dfx = D(LEFT(cur_node));
    node* A = create_val (A_st);

    return mlt (A, mlt (pow (fx, sub (create_val (A_st), create_val ("1"))), dfx));
    }

/*!
@brief (x / y) = (x * y ^ (-1))
*/
node* transform_division (node* cur_node)
    {
    return mlt (LEFT(cur_node), pow (RIGHT(cur_node), create_val ("-1")));
    }

/*!
@brief (sin_f_x)' = cos * (f_x)'
*/
node* diff_sin (node* cur_node)
    {
    return mlt (cosinus (COPY(RIGHT(cur_node))), D(RIGHT(cur_node)));
    }

/*!
@brief (cos_f_x)' = (-1) * sin (f_x) * (f_x)'
*/
node* diff_cos (node* cur_node)
    {
    return mlt (mlt (create_val ("-1"), sinus (RIGHT(cur_node))), D(RIGHT(cur_node)));
    }
