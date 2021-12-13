#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


#include "differ.h"
#include "tree_funks.h"


// Should be implemented to OP nodes

void optimize_node (node* oper)
    {
    if (oper != NULL)
        {
        if (oper->left_child != NULL && oper->right_child != NULL)
            {
            // anything * 0 = 0
            if ((strcmp (oper->left_child->content, "0") == EQUAL ||
                strcmp (oper->right_child->content, "0") == EQUAL) &&
                *oper->content == '*')
                {
                oper->ntype = VAL;

                oper->content = transform_to_node_content ("0");

                oper->left_child = oper->right_child = NULL;
                }
            // x ^* 1 = x and x +- 0 = x
            else if (
                    (*oper->left_child->content == '1' &&              *oper->content == '*') ||
                    (            *oper->content == '*' && *oper->right_child->content == '1') ||
                    (*oper->left_child->content == '1' &&              *oper->content == '^') ||
                    (            *oper->content == '^' && *oper->right_child->content == '1') ||
                    (*oper->left_child->content == '0' &&              *oper->content == '-') ||
                    (            *oper->content == '-' && *oper->right_child->content == '0') ||
                    (*oper->left_child->content == '0' &&              *oper->content == '+') ||
                    (            *oper->content == '+' && *oper->right_child->content == '0')
                    )
                {
                node* informative_child = NULL;

                if (isdigit (*oper->left_child->content))
                    {
                    informative_child = oper->right_child;
                    }
                else // *oper->right_child->content == '1'
                    {
                    informative_child = oper->left_child;
                    }

                copy_node (oper, informative_child);
                }
            // value [+-*^] value = value
            else if (oper->left_child->ntype == VAL && oper->right_child->ntype == VAL)
                {
                oper->ntype = VAL;

                const double BAD_VALUE = -1.11;

                double value = BAD_VALUE;

                // Transfer strings to doubles
                double left_child_value = BAD_VALUE;
                left_child_value = atof (oper->left_child->content);

                double right_child_value = BAD_VALUE;
                right_child_value = atof (oper->right_child->content);

                const size_t ACCURACY = 4;

                char value_str[ACCURACY + 1] = {}; // 1 for '\0'


                // Compute optimization
                if (*oper->content == '+')
                    {
                    value = left_child_value + right_child_value;
                    }
                else if (*oper->content == '-')
                    {
                    value = left_child_value - right_child_value;
                    }
                else if (*oper->content == '*')
                    {
                    value = left_child_value * right_child_value;
                    }
                else if (*oper->content == '^')
                    {
                    value = pow (left_child_value, right_child_value);
                    }


                // For repeated walks
                // It is better to avoid different zeros like: 0.000 and 0
                if (value != 0 && value != 1)
                    {
                    oper->content = transform_to_node_content (gcvt (value, ACCURACY, value_str));
                    }
                else if (value == 0)
                    {
                    oper->content = transform_to_node_content ("0");
                    }
                else if (value == 1)
                    {
                    oper->content = transform_to_node_content ("1");
                    }


                oper->left_child = oper->right_child = NULL;
                }
            }
        }
    puts ("Optimizations was skipped");
    }


node* optimize_node_recurs (node* root)
    {
    printf ("Now processing %s[%p]\n", root->content, root);
    optimize_node (root);

    if (root->left_child != NULL)
        {
        puts ("LC oprimizarion");
        optimize_node_recurs (root->left_child);
        }

    if (root->right_child != NULL)
        {
        puts ("RC oprimizarion");
        optimize_node_recurs (root->right_child);
        }

    return root;
    }
