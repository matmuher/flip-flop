#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "great_grammar.h"


//=============================================================================\\
//                             GRAMMAR SHIT                                    \\
//=============================================================================\\

#define VERBOSE_SIGNAL(cmd_name) puts (#cmd_name)


// General rule
ma_ty get_G (parsed_line_reader* pl_reader)
    {
    cur_read_pos (pl_reader);
    VERBOSE_SIGNAL(get_g);

    ma_ty val = {};
    val = get_E (pl_reader);
cur_read_pos (pl_reader);
    require ('$', pl_reader);

    return val;
    }


// gfs - get from structure
#define gfs(element) pl_reader->element
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

    if (t_type == T_KW)  // some keyword
        {
        size_t cur_token_id = gfs(token_id);
        gfs(token_id)++;

        require ('(', pl_reader);

        ma_ty arg = get_E (pl_reader);

        dot_this_shit (arg);

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

    if (gfs(pl[gfs(token_id)].type) == T_PARENTH_O)
        {
        gfs(token_id)++;
        ma_ty val = get_E (pl_reader);

        require (')', pl_reader);

        return val;
        }
    else if (gfs(pl[gfs(token_id)].type) == T_VAL) // T_VAR process
        {
        return get_N (pl_reader);
        }
    }


// Dig a number from expression
ma_ty get_N (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_n);

    size_t start_token_id = gfs(token_id);

    ma_ty nval = {};
    double val = 0;

     if (gfs(pl[gfs(token_id)].type) == T_VAL)
        {
        val = gfs(pl[gfs(token_id)++].content.val);
        }
    else
        {
        syntax_error (pl_reader, BAD_DIGIT);

        return NULL;
        }

    const int ACCURACY = 6;
    char str_val[ACCURACY] = {};
    nval = create_val (gcvt (val, ACCURACY, str_val));

    return nval;
    }


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> ", error_code);
    print_token (gfs(pl[gfs(token_id)]));

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

    if (t_type ==  T_OP || t_type ==  T_PARENTH_O || t_type ==  T_PARENTH_C || t_type ==  T_END)
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
    if (gfs(pl[kw_token_id].type) == T_KW)
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
            }
        }

    return NULL;
    }
