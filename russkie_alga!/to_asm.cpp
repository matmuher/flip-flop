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
    if (current_node->ntype == VAR)
        {
        if (!search_in_dict (ma_dict, current_node->content))
            {
            ma_dict = add_dict_cell (ma_dict, current_node->content, DICT_INIT_VALUE);
            }
        }

    return ma_dict;
    }





