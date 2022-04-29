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

/*
General rule
ma_ty - node* (or check great_grammar.h)
*/
ma_ty get_G (parsed_line_reader* pl_reader)
    {
    // cur_read_pos (pl_reader);
    VERBOSE_SIGNAL(get_g);

    ma_ty old_binder = NULL;

    while (TOKEN_TYPE != T_END)
        {
        // Statements growing according to tree standard

        node* new_binder = st (NULL, NULL); // st - statement
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


// A - first level of parsing
ma_ty get_A (parsed_line_reader* pl_reader)
    {
    // puts ("MY FIRST STEP");

    if (TOKEN_TYPE == T_SFRAME) // standard frames f.e.: if, while...
        {
        size_t sframe_token_id = TOKEN_ID;
        TOKEN_ID++;

        REQUIRE('('); // Kak napisat' regulyarku chtobi razom zamenit' vse?

        ma_ty val = get_B (pl_reader);

        REQUIRE(')');

        REQUIRE('{');

        node* old_binder = NULL;

        while (TOKEN_TYPE != T_SQUARE_BR)
            {
            node* new_binder = st (NULL, NULL);
            new_binder->left_child = old_binder;

            ma_ty val2 = get_A (pl_reader);
            require (';', pl_reader); // ?Move to get_A

            new_binder->right_child = val2;

            old_binder = new_binder;
            }

        REQUIRE('}');

        val = bi_oper (val, old_binder, SFRAME_IDENTIFIER, SFRAME);

        return val;
        }
    else if (TOKEN_TYPE == T_VAL   || // "55" - value
             TOKEN_TYPE == T_SFUNK || // "print" - standard function
             TOKEN_TYPE == T_UFUNK) // "esh_slizney ()" - user function
        {
        // puts ("User function got here");
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
    // "define esh_slizney (param1, param2){x:5;}" - function definition
    else if (TOKEN_TYPE == T_DEF)
        {
        // puts ("Funktion we are in");

        TOKEN_ID++;

        REQUIRE(' ');

        node* funk_name = create_node (UFUNK, TOKEN_IDENTIFIER);
        TOKEN_ID++;

        REQUIRE(' ');

        REQUIRE('(');

        // Good-old copypaste
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
            require (';', pl_reader); // !Move to get_A

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


// Boolean
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
                printf ("%c: ", TOKEN_SERVANT);
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

    if (TOKEN_TYPE == T_OP)
        {
        while (gfs(pl[TOKEN_ID].content.servant) == '+' ||
               gfs(pl[TOKEN_ID].content.servant) == '-')
            {
            char oper = gfs(pl[TOKEN_ID++].content.servant);
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

    if (TOKEN_TYPE == T_OP)
        {
        while (gfs(pl[TOKEN_ID].content.servant) == '*' ||
               gfs(pl[TOKEN_ID].content.servant) == '/')
            {
            char oper = gfs(pl[TOKEN_ID++].content.servant);
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


// Factor
ma_ty get_F (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_f);

    token_type t_type = TOKEN_TYPE;

    if (t_type == T_SFUNK ||
        t_type == T_UFUNK)  // some keyword
        {
        puts ("USER FUNCTION CALL!!!");
        size_t cur_token_id = TOKEN_ID;

        node_type funk_type = t_type == T_SFUNK? SFUNK : UFUNK;

        node* funk_name = create_node (funk_type, gfs(pl[TOKEN_ID].content.id));

        TOKEN_ID++;

        REQUIRE(' ');

        require ('(', pl_reader);


        // Good-old copypaste
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


// Primary level
ma_ty get_P (parsed_line_reader* pl_reader)
    {
    // puts ("Get_P is in de haus");
    VERBOSE_SIGNAL(get_p);

    if (TOKEN_TYPE == T_ROUND_BR)
        {
        TOKEN_ID++;
        ma_ty val = get_E (pl_reader);

        require (')', pl_reader);

        return val;
        }
    else if (TOKEN_TYPE == T_VAL ||
             TOKEN_TYPE == T_VAR) // T_VAR process
        {
        puts ("get_N is in de haus");
        return get_N (pl_reader);
        }
    }


// Dig a number from expression
ma_ty get_N (parsed_line_reader* pl_reader)
    {
    VERBOSE_SIGNAL(get_n);

    print_token (gfs(pl[TOKEN_ID]), 1);

    if (TOKEN_TYPE == T_VAL)
        {
        double val = 0;

        val = gfs(pl[TOKEN_ID++].content.val);

        char str_val[NAME_LENGTH] = {};

        const size_t DECIMAL_RADIX = 10;

        itoa (val, str_val, DECIMAL_RADIX);
        puts (str_val);

        node* temp_one = create_node (VAL, str_val);

        return temp_one;
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
//                             SERVICE SHIT                                    \\
//=============================================================================\\

// !Add error_code process via switch and define print
ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> ", error_code);
    print_token (gfs(pl[TOKEN_ID]), 1);

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

    token_type t_type = TOKEN_TYPE;

    if (t_type ==  T_OP || t_type ==  T_ROUND_BR || t_type ==  T_END || t_type == T_LINE ||
        t_type ==  T_COMP || t_type == T_DELIM || t_type == T_SQUARE_BR)
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
