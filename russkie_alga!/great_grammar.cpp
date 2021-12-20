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

#define VERBOSE_SIGNAL(cmd_name) puts (#cmd_name)

// gfs - get from structure
#define gfs(element) pl_reader->element

// General rule
ma_ty get_G (parsed_line_reader* pl_reader)
    {
    // cur_read_pos (pl_reader);
    VERBOSE_SIGNAL(get_g);

    ma_ty old_binder = NULL;

    while (gfs(pl[gfs(token_id)].type) != T_END)
        {
        node* new_binder = st (NULL, NULL);
        new_binder->left_child = old_binder;

        ma_ty val = get_A (pl_reader);
        require ('#', pl_reader); // !Move to get_A

        new_binder->right_child = val;

        old_binder = new_binder;
        }
    // cur_read_pos (pl_reader);
    require ('$', pl_reader);

    return old_binder;
    }


// A - first letter of the alphabet
ma_ty get_A (parsed_line_reader* pl_reader)
    {
    if (gfs(pl[gfs(token_id)].type) == T_SFRAME)
        {
        size_t sframe_token_id = gfs(token_id);
        gfs(token_id)++;

        require ('(', pl_reader);

        ma_ty val = get_B (pl_reader);

        require (')', pl_reader);

        require ('{', pl_reader);

        node* old_binder = NULL;

        while (gfs(pl[gfs(token_id)].type) != T_PARENTH ||
               gfs(pl[gfs(token_id)].content.servant) != '}')
            {
            node* new_binder = st (NULL, NULL);
            new_binder->left_child = old_binder;

            ma_ty val2 = get_A (pl_reader);
            require (';', pl_reader); // !Move to get_A

            new_binder->right_child = val2;

            old_binder = new_binder;
            }

        require ('}', pl_reader);

        val = bi_oper (val, old_binder, gfs(pl[sframe_token_id].content.id));

        return val;
        }
    else if ((gfs(pl[gfs(token_id)].type) == T_VAL) ||
             (gfs(pl[gfs(token_id)].type) == T_SFUNK))
        {
        ma_ty val = get_E (pl_reader);

        return val;
        }
    else if (gfs(pl[gfs(token_id)].type) == T_VAR)
        {
        ma_ty val = create_node (VAR, gfs(pl[gfs(token_id)].content.id));
        gfs(token_id)++;

        require (':', pl_reader);

        ma_ty val2 = get_E (pl_reader);

        val = ass (val, val2);

        return val;
        }
    else
        {
        puts ("Razdel nahoditsya v razrabotke...");
        }
    }


// Boolean
ma_ty get_B (parsed_line_reader* pl_reader)
    {
    ma_ty val = get_E (pl_reader);

    if (gfs(pl[gfs(token_id)].type) == T_COMP)
        {
        size_t comp_token_id = gfs(token_id)++;

        ma_ty val2 = get_E (pl_reader);

        switch (gfs(pl[comp_token_id].content.servant))
            {
            case '=':
                {
                val = eq (val, val2);
                break;
                }
            case '>':
                {
                val = gr (val, val2);
                break;
                }
            case '<':
                {
                val = ls (val, val2);
                break;
                }
            default:
                printf ("%c: ", gfs(pl[gfs(token_id)].content.servant));
                puts ("This comparison operator hasn't been added yet");
            }

        return val;
        }
    else
        {
        syntax_error (pl_reader, COMPARISON_EXPECTED);

        return NULL;
        }
    }


// Expression
ma_ty get_E (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_e);

    ma_ty val = get_T (pl_reader);

    if (gfs(pl[gfs(token_id)].type) == T_OP)
        {
        while (gfs(pl[gfs(token_id)].content.servant) == '+' ||
               gfs(pl[gfs(token_id)].content.servant) == '-')
            {
            char oper = gfs(pl[gfs(token_id)++].content.servant);
            ma_ty val2 = get_T (pl_reader);

            if (oper == '+')
                {
                // printf ("val + val2 : %d + %d\n", val, val2);
                val = add (val, val2);
                }
            else if (oper == '-')
                {
                // printf ("val - val2 : %d - %d\n", val, val2);
                val = sub (val, val2);
                }
            }
        }

    return val;
    }


// Term
ma_ty get_T (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_t);

    ma_ty val = get_S (pl_reader);

    if (gfs(pl[gfs(token_id)].type) == T_OP)
        {
        while (gfs(pl[gfs(token_id)].content.servant) == '*' ||
               gfs(pl[gfs(token_id)].content.servant) == '/')
            {
            char oper = gfs(pl[gfs(token_id)++].content.servant);
            ma_ty val2 = get_S (pl_reader);

            if (oper == '*')
                {
                val = mlt (val, val2);
                }
            else if (oper == '/')
                {
                val = div (val, val2);
                }
            }
        }

    return val;
    }


// Stepen'
ma_ty get_S (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_s);

    ma_ty val = get_F (pl_reader);

    if (gfs(pl[gfs(token_id)]).type == T_OP)
        {
        while (gfs(pl[gfs(token_id)].content.servant) == '^')
            {
            gfs(token_id)++; // Skip '^'
            ma_ty val2 = get_F (pl_reader);

            val = npow (val, val2);
            }
        }

    return val;
    }


// Factor
ma_ty get_F (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_f);

    token_type t_type = gfs(pl[gfs(token_id)].type);

    if (t_type == T_SFUNK)  // some keyword
        {
        size_t cur_token_id = gfs(token_id);
        gfs(token_id)++;

        require ('(', pl_reader);

        ma_ty arg = get_E (pl_reader);

        ma_ty val = execute (pl_reader, cur_token_id, arg);

        require (')', pl_reader);

        return val;
        }
    else
        {
        return get_P (pl_reader);
        }
    }


// Primary level
ma_ty get_P (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_p);

    if (gfs(pl[gfs(token_id)].type) == T_PARENTH &&
        gfs(pl[gfs(token_id)].content.servant) == ')')
        {
        gfs(token_id)++;
        ma_ty val = get_E (pl_reader);

        require (')', pl_reader);

        return val;
        }
    else if (gfs(pl[gfs(token_id)].type) == T_VAL ||
             gfs(pl[gfs(token_id)].type) == T_VAR) // T_VAR process
        {
        return get_N (pl_reader);
        }
    }


// Dig a number from expression
ma_ty get_N (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_n);

    if (gfs(pl[gfs(token_id)].type) == T_VAL)
        {
        double val = 0;

        val = gfs(pl[gfs(token_id)++].content.val);

        const int ACCURACY = 6;
        char str_val[ACCURACY] = {};

        return create_val (gcvt (val, ACCURACY, str_val));
        }
    else if (gfs(pl[gfs(token_id)].type) == T_VAR)
        {
        gfs(token_id)++;
        return create_node (VAR, gfs(pl[gfs(token_id)-1].content.id));
        }
    else
        {
        syntax_error (pl_reader, BAD_TERMINAL);

        return NULL;
        }
    }


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

// !Add error_code process via switch and define print
ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> ", error_code);
    print_token (gfs(pl[gfs(token_id)]), 1);

    return error_code;
    }


/*!
@brief Points out current position of begunok
*/
void cur_read_pos (parsed_line_reader* pl_reader)
    {
    print_pl (pl_reader);
    // !Add '^'
    printf ("Current token id is %d\n", pl_reader->token_id);
    }




// !Add token input, not requirement
int require (char requirement, parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(require);

    printf ("Require %c\n", requirement);

    token_type t_type = gfs(pl[gfs(token_id)].type);

    if (t_type ==  T_OP || t_type ==  T_PARENTH || t_type ==  T_END || t_type == T_LINE ||
        t_type ==  T_COMP || t_type == T_DELIM)
        {
        if (gfs(pl[gfs(token_id)].content.servant) == requirement)
            {
            gfs(token_id)++;

            return true;
            }
        }

    syntax_error (pl_reader, BAD_REQUIREMENT);

    return false;
    }


ma_ty execute (parsed_line_reader* pl_reader, size_t kw_token_id, ma_ty arg)
    {
    if (gfs(pl[kw_token_id].type) == T_SFUNK)
        {
        kws kw_info = determine_kw (gfs(pl[kw_token_id].content.id));

        switch (kw_info)
            {
            case sinus:
                return nsin (arg);
            case cosus:
                return ncos (arg);
            case logus:
                return nlog (arg);
            case print:
                return nprint (arg);
            }
        }

    return NULL;
    }

#undef gfs
