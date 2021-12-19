#include "..\differ\differ.h"
#include "..\differ\tree_funks.h"


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


node* nsin (node* arg)
    {
    return mono_oper (arg, "sin");
    }


node* ass (node* left, node* right)
    {
    bi_oper (left, right, "=");
    }


node* ncos (node* arg)
    {
    return mono_oper (arg, "cos");
    }


node* nlog (node* arg)
    {
    return mono_oper (arg, "log");
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


node* div (node* left, node* right)
    {
    return bi_oper (left, right, "/");
    }


node* npow (node* left, node* right)
    {
    return bi_oper (left, right, "^");
    }


node* eq (node* left, node* right)
    {
    return bi_oper (left, right, "==");
    }


node* gr (node* left, node* right)
    {
    return bi_oper (left, right, ">");
    }


node* ls (node* left, node* right)
    {
    return bi_oper (left, right, "<");
    }


node* wh (node* left, node* right)
    {
    return bi_oper (left, right, "while");
    }


node* st (node* left, node* right)
    {
    return bi_oper (left, right, "statement");
    }


node* create_val (const char* val)
    {
    return create_node (VAL, val);
    }
