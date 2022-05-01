#include "to_asm.h"

const size_t REVERS = 1,  NON_REVERS = 0,
             IN_EXPR = 1, OUT_EXPR = 0,
             IS_MAIN = 1, NON_MAIN = 0;


// Carry out recursive assembly of statements branch
void st_assembly (node* root, FILE* asm_file, dict ma_dict, int is_main)
    {
    if (root->left_child) st_assembly (root->left_child, asm_file, ma_dict, is_main);

    node* st_body = root->right_child;

    switch (st_body->ntype)
        {
        case INIT:   init_assembly (st_body, asm_file, ma_dict); break;

        case CALL:   call_assembly (st_body, asm_file, ma_dict); break;

        case RET:    ret_assembly (st_body, asm_file, ma_dict, is_main); break;

        case DEF:    def_assembly (st_body, asm_file, ma_dict); break;

        case SFRAME: sframe_assembly (st_body, asm_file, ma_dict, is_main); break;

        default:     printf ("[ERROR]: st_assembly can't process [%s]\n", root->content);
        }
    }


// Put variables from given subtree to dictionary
dict collect_vars (dict ma_dict, node* root)
    {
    ma_dict = try_node (ma_dict, root);

    if (root->left_child)  ma_dict = collect_vars (ma_dict, root->left_child);

    if (root->right_child) ma_dict = collect_vars (ma_dict, root->right_child);

    return ma_dict;
    }


// Put variable from given node to dictionary
dict try_node (dict ma_dict, node* current_node)
    {
    // Shift relatively bx in current scope
    static size_t ram_shift = 0;

    // Dictionary initialization
    if (ma_dict == NULL) {ram_shift = 0;}

    // Add to dictionary
    if (current_node->ntype == VAR && !search_in_dict (ma_dict, current_node->content))

            ma_dict = add_dict_cell (ma_dict, current_node->content, ram_shift++);

    return ma_dict;
    }


// Carry out assembly of function call
// Provides so-called "RAM-frame" (it's kind of stack frame, but with ram)
void call_assembly (node* call_node, FILE* asm_file, dict ma_dict, int in_expression)
    {
    assert (call_node->ntype == CALL);

    if (call_node->left_child->ntype == UFUNK)
        {
        // Save base register
        fprintf (asm_file, "push bx\n\n");

        // Push parameters
        size_t params_num = 0;
        params_num = push_params (call_node->right_child, asm_file, ma_dict);

        // Prepare bx for addressing in new scope
        fprintf (asm_file, "push bx\n"
                           "push %d\n"
                           "add\n"
                           "pop bx\n", ma_dict->value + 1);

        // Pop parameters in RAM to called function has access to them
        pop_params (params_num, asm_file);

        fprintf (asm_file, "call :%s\n", call_node->left_child->content);

        // Recover base register
        fprintf (asm_file, "pop bx\n");

        // push ret value if in expression
        if (in_expression) fprintf (asm_file, "push ax\n");
        }
    else if (call_node->left_child->ntype == SFUNK)
        {
        // Push parameters
        size_t params_num = 0;
        params_num = push_params (call_node->right_child, asm_file, ma_dict);

        switch (call_node->left_child->content[0])
            {
            case 'p':
                {
                for (size_t param_id = 0; param_id < params_num; param_id++)
                    {
                    fprintf (asm_file, "out\n"
                                       "pop 0\n");
                    }
                break;
                }

            case 's':
                {
                assert (params_num == 1);
                fprintf (asm_file, "sqrt\n");
                break;
                }

            case 'i':
                {
                assert (params_num == 0);
                fprintf (asm_file, "in\n");
                break;
                }
            }
        }
    else printf ("[ERROR]: Can't process a function [%s]\n", call_node->left_child->content);
    }


void init_assembly (node* init_node, FILE* asm_file, dict ma_dict)
    {
    expression_assembly (init_node->right_child->right_child, asm_file, ma_dict);

    store_value_assembly (init_node->left_child, asm_file, ma_dict);
    }


void expression_assembly (node* root, FILE* asm_file, dict ma_dict)
    {
    node* right = root->right_child;
    node* left = root->left_child;

    if (right &&
    (right->ntype == VAL ||  right->ntype == VAR || right->ntype == CALL || right->ntype == OP))

        expression_assembly (right, asm_file, ma_dict);

    if (left &&
    (left->ntype == VAL  ||  left->ntype == VAR  || left->ntype == CALL || left->ntype == OP))

        expression_assembly (left, asm_file, ma_dict);

    expression_node_assembly (root, asm_file, ma_dict);
    }


void expression_node_assembly (node* cur_node, FILE* asm_file, dict ma_dict)
    {
    switch (cur_node->ntype)
        {
        case OP:   op_assembly (cur_node, asm_file, ma_dict); break;

        case VAR:  var_assembly (cur_node, asm_file, ma_dict, "push"); break;

        case VAL:  push_val_assembly (cur_node, asm_file, ma_dict); break;

        case CALL: call_assembly (cur_node, asm_file, ma_dict, IN_EXPR); break;

        default:   printf ("Unexpected member of expression: %s\n", cur_node->content);
        }
    }


void store_value_assembly (node* cur_node, FILE* asm_file, dict ma_dict)
    {
    if (cur_node->ntype == VAR) var_assembly (cur_node, asm_file, ma_dict, "pop");

    else puts ("[ERROR]: Unable to initialize non-variable\n");
    }


size_t push_params (node* param_node, FILE* asm_file, dict ma_dict)
    {
    size_t params_num = 0;

    while (param_node)
        {
        params_num++;

        expression_assembly (param_node->right_child, asm_file, ma_dict);

        param_node = param_node->left_child;
        }

    return params_num;
    }


void var_assembly (node* var_node, FILE* asm_file, dict ma_dict, const char* cmd)
    {
    size_t ram_shift = 0;

    ram_shift = dict_get_val (ma_dict, var_node->content);

    fprintf (asm_file, "%s [bx+%d]\n", cmd, ram_shift);
    }


void push_val_assembly (node* val_node, FILE* asm_file, dict ma_dict)
    {
    fprintf (asm_file, "push %s\n", val_node->content);
    }


void op_assembly (node* op_node, FILE* asm_file, dict ma_dict)
    {
    switch (op_node->content[0])
        {
        case '+': fprintf (asm_file, "add\n");break;

        case '-': fprintf (asm_file, "sub\n");break;

        case '*': fprintf (asm_file, "mlt\n");break;

        case '/': fprintf (asm_file, "saw\n");break;
        }
    }


void pop_params (size_t params_num, FILE* asm_file)
    {
    for (size_t param_id = 0; param_id < params_num; param_id++)

        fprintf (asm_file, "pop [bx+%d]\n", param_id);

    putc ('\n', asm_file);
    }


void ret_assembly (node* ret_node, FILE* asm_file, dict ma_dict, int is_main)
    {
    expression_assembly (ret_node->right_child, asm_file, ma_dict);

    // return value stores in ax
    fprintf (asm_file, "pop ax\n");

    if (is_main) {fprintf (asm_file, "hlt\n\n");}
    else {fprintf (asm_file, "ret\n\n");}
    }


void def_assembly (node* def_node, FILE* asm_file, dict ma_dict)
    {
    if (def_node)
        {
        assert (def_node->ntype == DEF);

        int is_main = 0;
        node* funk_name = def_node->left_child->left_child;
        if (strcmp (funk_name->content, "main") == EQUAL) {is_main = IS_MAIN;}

        fprintf(asm_file, "def %s:\n", funk_name->content);

        dict local_dict = NULL;
        local_dict = collect_vars (local_dict, def_node);

        st_assembly (def_node->right_child, asm_file, local_dict, is_main);

        free_dict (local_dict);
        }
    else puts ("[ERROR]: Empty define node");
    }

// Assembly standard frames: if, while
void sframe_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, int is_main)
    {
    assert (sframe_node->ntype == SFRAME);

    static size_t cond_id = 0;

    switch (sframe_node->content[0])
        {
        // while
        case 'w': while_assembly (sframe_node, asm_file, ma_dict, cond_id++, is_main);break;
        // if
        case 'i': if_assembly (sframe_node, asm_file, ma_dict, is_main);break;
        }
    }


void if_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, int is_main)
    {
    static size_t if_id = 0;

    cmp_assembly (sframe_node->left_child, asm_file, ma_dict, "if", if_id, REVERS);

    size_t cur_if_id = if_id++;

    st_assembly (sframe_node->right_child, asm_file, ma_dict, is_main);

    fprintf (asm_file, "def if%d:\n"
                       "say\n", cur_if_id);
    }


void while_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, size_t cond_id, int is_main)
    {
    static size_t while_id = 0;

    fprintf (asm_file, "jump :cond%d\n"
                       "def while%d:\n",cond_id, while_id);

    st_assembly (sframe_node->right_child, asm_file, ma_dict, is_main);

    fprintf (asm_file, "def cond%d:\n", cond_id);

    cmp_assembly (sframe_node->left_child, asm_file, ma_dict, "while", while_id++);
    }


void cmp_assembly (node* cmp_node, FILE* asm_file, dict ma_dict,
                   const char* mark_name, size_t mark_id, int revers)
    {
    expression_assembly (cmp_node->right_child, asm_file, ma_dict);

    expression_assembly (cmp_node->left_child, asm_file, ma_dict);

    switch (cmp_node->content[0])
        {
        case '>':
            if (revers) fprintf (asm_file, "jbe :%s%d\n", mark_name, mark_id);
            else fprintf (asm_file, "ja :%s%d\n", mark_name, mark_id);
            break;

        case '<':
            if (revers) fprintf (asm_file, "jae :%s%d\n", mark_name, mark_id);
            else fprintf (asm_file, "jb :%s%d\n", mark_name, mark_id);
            break;

        case '=':
            if (revers) fprintf (asm_file, "jne :%s%d\n", mark_name, mark_id);
            else fprintf (asm_file, "je :%s%d\n", mark_name, mark_id);
            break;
        }
    }

