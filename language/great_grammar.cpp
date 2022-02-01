#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "great_grammar.h"


//=============================================================================\\
//                             GRAMMAR SHIT                                    \\
//=============================================================================\\

#define VERBOSE_SIGNAL(cmd_name)      \
            puts ("start " #cmd_name);\


// General rule
ma_ty get_G (const char* expression)
    {
    VERBOSE_SIGNAL(get_g);

    line_reader exp_reader = {expression, expression};

    ma_ty val = 0;
    val = get_E (&exp_reader);

    require ('$', &exp_reader);

    return val;
    }


// Expression
ma_ty get_E (line_reader* exp_reader)
    {
     VERBOSE_SIGNAL(get_e);

    ma_ty val = get_T (exp_reader);

    while (*exp_reader->begunok == '+' || *exp_reader->begunok == '-')
        {
        char oper = *exp_reader->begunok++;
        ma_ty val2 = get_T (exp_reader);


        if (oper == '+')
            {
            printf ("val + val2 : %d + %d\n", val, val2);
            val += val2;

            }
        else if (oper == '-')
            {
            printf ("val - val2 : %d - %d\n", val, val2);
            val -= val2;
            }
        }

    return val;
    }


// Term
ma_ty get_T (line_reader* exp_reader)
    {
     VERBOSE_SIGNAL(get_t);

    ma_ty val = get_S (exp_reader);

    while (*exp_reader->begunok == '*' || *exp_reader->begunok == '/')
        {
        char oper = *exp_reader->begunok++;
        int val2 = get_S (exp_reader);

        if (oper == '*')
            {
            val *= val2;
            }
        else if (oper == '/')
            {
            val /= val2;
            }
        }

    return val;
    }


// Stepen'
ma_ty get_S (line_reader* exp_reader)
    {
    VERBOSE_SIGNAL(get_s);

    ma_ty val = get_F (exp_reader);

    while (*exp_reader->begunok == '^')
        {
        exp_reader->begunok++; // Skip '^'
        ma_ty val2 = get_F (exp_reader);

        val = pow (val, val2);
        }

    return val;
    }


// Factor
ma_ty get_F (line_reader* exp_reader)
    {
    VERBOSE_SIGNAL(get_f);

    IDS id_info = get_Id (exp_reader);

    if (id_info > 1)  // some keyword
        {
        require ('(', exp_reader);

        ma_ty arg = get_E (exp_reader);
        ma_ty val = execute (id_info, arg);

        require (')', exp_reader);

        return val;
        }
    else
        {
        return get_P (exp_reader);
        }
    }


// Primary level
ma_ty get_P (line_reader* exp_reader)
    {
    VERBOSE_SIGNAL(get_p);

    if (*exp_reader->begunok == '(')
        {
        exp_reader->begunok++;
        ma_ty val = get_E (exp_reader);

        require (')', exp_reader);

        return val;
        }
    else if (get_Id (exp_reader) == non_id)
        {
        return get_N (exp_reader);
        }
    }


// Dig a number from expression
ma_ty get_N (line_reader* exp_reader)
    {
    VERBOSE_SIGNAL(get_n);

    const char* start_position = exp_reader->begunok;

    ma_ty val = 0;

    // Read integer part
    while (isdigit (*exp_reader->begunok))
        {
        val = val * 10 + (*exp_reader->begunok++ - '0');
        }

    // Read dot part
    if (*exp_reader->begunok == '.')
        {
        exp_reader->begunok++;

        double dot_digits_counter = 0;
        ma_ty dot_digits = 0;

        while (isdigit (*exp_reader->begunok))
            {
            dot_digits = dot_digits * 10 + (*exp_reader->begunok++ - '0');
            dot_digits_counter++;
            }

        val = val + dot_digits * (pow (10, -dot_digits_counter));
        }

    // If no digit was read
    if (start_position == exp_reader->begunok)
        {
        return syntax_error (*exp_reader->begunok, BAD_DIGIT);
        }

    return val;
    }


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

ERROR_LIST syntax_error (char current_char, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> %c\n", error_code, current_char);

    return error_code;
    }


/*!
@brief Points out current position of begunok
*/
void cur_read_pos (line_reader* exp_reader)
    {
    puts (exp_reader->root);

    int pointer = exp_reader->begunok - exp_reader->root + 1;

    printf ("%*c\n", pointer, '^');
    }



int require (char requirement, line_reader* exp_reader)
    {
    VERBOSE_SIGNAL(require);

    if (*exp_reader->begunok == requirement)
        {
        exp_reader->begunok++;

        return true;
        }

    syntax_error (*exp_reader->begunok, BAD_REQUIREMENT);

    return false;
    }


ma_ty execute (IDS id_info, ma_ty arg)
    {
    switch (id_info)
        {
        case sinus:
            return sin (arg);
        case cosus:
            return cos (arg);
        case logus:
            return log (arg);
        }
    }
