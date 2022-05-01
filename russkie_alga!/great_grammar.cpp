#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "..\differ\tree_funks.h"
#include "great_grammar.h"

//=============================================================================\\
//                             GRAMMAR SHIT                                    \\
//=============================================================================\\

#define VERBOSE_SIGNAL(cmd_name) puts(#cmd_name)

/*
gfs - get from structure
pl - parsed_line
*/
#define gfs(element) pl_reader->element
#define TOKEN_ID gfs(token_id)
#define TOKEN_TYPE gfs(pl[TOKEN_ID].type)
#define TOKEN_TYPE_FLEX(token_id) gfs(pl[token_id].type)
#define REQUIRE(requirement) require (requirement, pl_reader)
#define CONTENT gfs(pl[TOKEN_ID].content)
#define TOKEN_SERVANT CONTENT.servant
#define TOKEN_IDENTIFIER CONTENT.id
#define SFRAME_IDENTIFIER gfs(pl[sframe_token_id].content.id)

/*!
@brief [G]eneral rule: connects high-level(A) constructions
*/
ma_ty get_G (parsed_line_reader* pl_reader)
    {
    // binder connects statements with each other
    ma_ty old_binder = NULL;

    // Statements growing according to tree standard
    while (TOKEN_TYPE != T_END)
        {
        node* new_binder = st (NULL, NULL); // st - statement
        new_binder->left_child = old_binder;

        ma_ty val = get_A (pl_reader);
        require ('#', pl_reader);

        new_binder->right_child = val;

        old_binder = new_binder;
        }

    require ('$', pl_reader);

    return old_binder;
    }


/*!
@brief Parse high-level([A]) constructions:
        1. Standard frames (if, while);
        2. Initialization;
        3. Function define;
        4. Return;
        Descend to expression, boolean etc;
*/
ma_ty get_A (parsed_line_reader* pl_reader)
    {
    if (TOKEN_TYPE == T_SFRAME) // standard frames f.e.: if, while...
        {
        size_t sframe_token_id = TOKEN_ID;
        TOKEN_ID++;

        REQUIRE('(');

        ma_ty val = get_B (pl_reader);

        REQUIRE(')');

        REQUIRE('{');

        node* old_binder = NULL;

        while (TOKEN_TYPE != T_SQUARE_BR)
            {
            node* new_binder = st (NULL, NULL);
            new_binder->left_child = old_binder;

            ma_ty val2 = get_A (pl_reader);
            require (';', pl_reader);

            new_binder->right_child = val2;

            old_binder = new_binder;
            }

        REQUIRE('}');

        val = bi_oper (val, old_binder, SFRAME_IDENTIFIER, SFRAME);

        return val;
        }
    else if (TOKEN_TYPE == T_VAL   || // "55" - value
             TOKEN_TYPE == T_SFUNK || // "printf" - standard function
             TOKEN_TYPE == T_UFUNK)   // "esh_slizney()" - user function
        {

        ma_ty val = get_E (pl_reader);

        return val;
        }
    // "val1:2" - initialization
    else if (TOKEN_TYPE == T_VAR)
        {
        ma_ty val = create_node (VAR, gfs(pl[TOKEN_ID].content.id));
        TOKEN_ID++;

        require (':', pl_reader);

        ma_ty val2 = get_E (pl_reader);

        val = ass (val, val2);

        return val;
        }
    // "define esh_slizney (param1,param2){x:5;}" - function definition
    else if (TOKEN_TYPE == T_DEF)
        {
        TOKEN_ID++;

        REQUIRE(' ');

        node* funk_name = create_node (UFUNK, TOKEN_IDENTIFIER);
        TOKEN_ID++;

        REQUIRE(' ');

        REQUIRE('(');

        // TODO: copypaste!
        node* param_old_binder = NULL;

        // Getting parameters
        while (TOKEN_TYPE == T_VAR)
            {
            node* param_new_binder = param (NULL, NULL);
            param_new_binder->left_child = param_old_binder;

            ma_ty parameter = create_node (VAR, gfs(pl[TOKEN_ID++].content.id));

            param_new_binder->right_child = parameter;

            param_old_binder = param_new_binder;


            if (TOKEN_TYPE != T_ROUND_BR)
                {
                require (',', pl_reader);
                }
            }

        require (')', pl_reader);

        require ('{', pl_reader);

        node* st_old_binder = NULL;

        while (TOKEN_TYPE != T_SQUARE_BR)
            {
            node* st_new_binder = st (NULL, NULL);
            st_new_binder->left_child = st_old_binder;

            ma_ty val2 = get_A (pl_reader);
            require (';', pl_reader);

            st_new_binder->right_child = val2;

            st_old_binder = st_new_binder;
            }

        require ('}', pl_reader);

        return def (funk (funk_name, param_old_binder), st_old_binder);
        }
    else if (TOKEN_TYPE == T_RET)
        {
        TOKEN_ID++;

        require (' ', pl_reader);

        node* ret_val = get_E (pl_reader);

        return mono_oper (ret_val, "return", RET);
        }
    else
        {
        printf ("[UNEXPECTED TOKEN] : %d\n", TOKEN_TYPE);
        }
    }


/*!
@brief Parse boolean;
*/
ma_ty get_B (parsed_line_reader* pl_reader)
    {
    ma_ty val = get_E (pl_reader);

    if (TOKEN_TYPE == T_COMP)
        {
        char comp_oper = TOKEN_SERVANT;
        size_t comp_token_id = TOKEN_ID++;

        ma_ty val2 = get_E (pl_reader);

        switch (comp_oper)
            {
            case '=': val = eq (val, val2); break;
            case '>': val = gr (val, val2); break;
            case '<': val = ls (val, val2); break;
            default:
                printf ("%c: ", TOKEN_SERVANT);
                puts ("This comparison operator can't be processed");
            }

        return val;
        }
    else
        {
        syntax_error (pl_reader, COMPARISON_EXPECTED);

        return NULL;
        }
    }


/*!
@brief Parse sum and subtraction;
       Descend to multiply operation etc;
*/
ma_ty get_E (parsed_line_reader* pl_reader)
    {
    ma_ty val = get_T (pl_reader);

    if (TOKEN_TYPE == T_OP)
        {
        while (gfs(pl[TOKEN_ID].content.servant) == '+' ||
               gfs(pl[TOKEN_ID].content.servant) == '-')
            {
            char oper = gfs(pl[TOKEN_ID++].content.servant);
            ma_ty val2 = get_T (pl_reader);

            if (oper == '+')      val = add (val, val2);
            else if (oper == '-') val = sub (val, val2);
            }
        }

    return val;
    }

/*!
@brief Parse multiply and division;
       Descend to power etc;
*/
ma_ty get_T (parsed_line_reader* pl_reader)
    {
    ma_ty val = get_S (pl_reader);

    if (TOKEN_TYPE == T_OP)
        {
        while (gfs(pl[TOKEN_ID].content.servant) == '*' ||
               gfs(pl[TOKEN_ID].content.servant) == '/')
            {
            char oper = gfs(pl[TOKEN_ID++].content.servant);
            ma_ty val2 = get_S (pl_reader);

            if (oper == '*')      val = mlt (val, val2);
            else if (oper == '/') val = div (val, val2);
            }
        }

    return val;
    }


/*!
@brief Parse powers;
       Descend to function calls etc;
*/
ma_ty get_S (parsed_line_reader* pl_reader)
    {
    ma_ty val = get_F (pl_reader);

    if (gfs(pl[TOKEN_ID]).type == T_OP)
        {
        while (gfs(pl[TOKEN_ID].content.servant) == '^')
            {
            TOKEN_ID++; // Skip '^'
            ma_ty val2 = get_F (pl_reader);

            val = npow (val, val2);
            }
        }

    return val;
    }

/*!
@brief Parse function calls;
       Descend to primary expressions;
*/
ma_ty get_F (parsed_line_reader* pl_reader)
    {
    token_type t_type = TOKEN_TYPE;

    // Function call process
    if (t_type == T_SFUNK ||
        t_type == T_UFUNK)
        {
        size_t cur_token_id = TOKEN_ID;

        node_type funk_type = t_type == T_SFUNK? SFUNK : UFUNK;

        node* funk_name = create_node (funk_type, gfs(pl[TOKEN_ID].content.id));

        TOKEN_ID++;

        require ('(', pl_reader);

        // TODO: copypaste!
        node* param_old_binder = NULL;

        while (TOKEN_TYPE == T_VAR ||
               TOKEN_TYPE == T_VAL)
            {
            node* param_new_binder = param (NULL, NULL);
            param_new_binder->left_child = param_old_binder;

            ma_ty parameter = get_E (pl_reader);

            param_new_binder->right_child = parameter;

            param_old_binder = param_new_binder;


            if (TOKEN_TYPE != T_ROUND_BR)
                {
                require (',', pl_reader);
                }
            }

        require (')', pl_reader);

        return call (funk_name, param_old_binder);
        }
    else
        {
        node* debug_node = get_P (pl_reader);

        return debug_node;
        }
    }

/*!
@brief [P]rimary level: parse parenthesized expressions;
       Descend to values and variables parse;
*/
ma_ty get_P (parsed_line_reader* pl_reader)
    {
    // (expression)
    if (TOKEN_TYPE == T_ROUND_BR)
        {
        TOKEN_ID++;
        ma_ty val = get_E (pl_reader);

        require (')', pl_reader);

        return val;
        }
    else if (TOKEN_TYPE == T_VAL || TOKEN_TYPE == T_VAR)
        {
        return get_N (pl_reader);
        }
    }


/*!
@brief Dig from parsed line a [N]umber or identifier;
*/
ma_ty get_N (parsed_line_reader* pl_reader)
    {
    if (TOKEN_TYPE == T_VAL)
        {
        double val = 0;

        val = gfs(pl[TOKEN_ID++].content.val);

        char str_val[NAME_LENGTH] = {};
        const size_t DECIMAL_RADIX = 10;
        itoa (val, str_val, DECIMAL_RADIX);
        node* val_node = create_node (VAL, str_val);

        return val_node;
        }
    else if (TOKEN_TYPE == T_VAR)
        {
        return create_node (VAR, gfs(pl[TOKEN_ID++].content.id));
        }
    else
        {
        syntax_error (pl_reader, BAD_TERMINAL);

        return NULL;
        }
    }


//=============================================================================\\
//                             SERVICE FUNCTIONS                               \\
//=============================================================================\\

/*!
@brief Prints information about syntax error;
*/
ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code)
    {
    static size_t error_token_id = 0;
    static size_t call_num = 0;

    if (error_token_id != TOKEN_ID) call_num = 0;

    if (call_num == 0)
        {
        printf ("[SYNTAX ERROR: %d] -> ", error_code, TOKEN_ID);
        print_token (gfs(pl[TOKEN_ID]), NEW_LINE);

        // Print context
        for (int token_id = - 2; token_id <= 2; token_id++)
            {
            print_token (gfs(pl[TOKEN_ID - token_id]), NON_NEW_LINE);
            }

        putchar ('\n');

        error_token_id = TOKEN_ID;
        call_num++;
        }

    return error_code;
    }


/*!
@brief Points out current position of begunok;
*/
void cur_read_pos (parsed_line_reader* pl_reader)
    {
    print_pl (pl_reader);
    // TODO: !Add '^'
    printf ("Current token id is %d\n", pl_reader->token_id);
    }


/*!
@brief Check if current token id corresponds requirement char;
*/
int require (char requirement, parsed_line_reader* pl_reader)
    {
    token_type t_type = TOKEN_TYPE;

    if (t_type ==  T_OP   || t_type ==  T_ROUND_BR || t_type ==  T_END || t_type == T_LINE ||
        t_type ==  T_COMP || t_type == T_DELIM     || t_type == T_SQUARE_BR)
        {
        if (gfs(pl[TOKEN_ID].content.servant) == requirement)
            {
            TOKEN_ID++;

            return true;
            }
        }

    syntax_error (pl_reader, BAD_REQUIREMENT);

    return false;
    }

#undef gfs
