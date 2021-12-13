#include <string.h>
#include <ctype.h>


#include "..\memory_free\elephant_calloc_extern.h"
#include "differ.h"
#include "tree_funks.h"
#include "diff_dsl.h"


/*!
@brief Differentiate tree'ed expression

@details Origin tree isn't changed. New tree for diff' expression is created

@params[in] cur_node pointer to node that should be differentiated

@return Pointer new tree with differentiated expression
*/
node* to_diff (node* cur_node)
    {
    const char* node_content = cur_node->content;

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
        else if (strcmp (node_content, "*") == EQUAL)
            {
            return diff_mlt (cur_node);
            }
        else if (strcmp (node_content, "^") == EQUAL)
            {
            return diff_pow (cur_node);
            }
        else if (strcmp (node_content, "/") == EQUAL)
            {
            // (x / y)' = (x * y ^ (-1))'
            return diff_mlt (transform_division (cur_node));
            }
        else if (strcmp (node_content, "sin") == EQUAL)
            {
            return diff_sin (cur_node);
            }
        else if (strcmp (node_content, "cos") == EQUAL)
            {
            return diff_cos (cur_node);
            }
        }
    }


