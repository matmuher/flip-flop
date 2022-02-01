#include <stdio.h>
#include <ctype.h>
#include "grammar_lab.h"
#include <math.h>

const int MAX_STR_LENGTH = 100;
char str[MAX_STR_LENGTH] = "2^2*3$";
char* begunok = str;

#define VERBOSE FIRST_LVL

static int syntax_error (char current_char, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> %c\n", error_code, current_char);

    return error_code;
    }


#define VERBOSE_SIGNAL(cmd_name)      \
            puts ("start " #cmd_name);\
            printf ("%c\n", *begunok);\


int get_n (void)
    {
    VERBOSE_SIGNAL(get_n);

    char* start_position_begunok = begunok;

    int val = 0;

    while (isdigit (*begunok))
        {
        val = val * 10 + (*begunok++ - '0');
        }

    if (start_position_begunok == begunok)
        {
        return syntax_error (*begunok, BAD_DIGIT);
        }

    return val;
    }

    static char* require (char requirement)
    {
    printf ("Ive required %c and i'm processing: %c\n", requirement, *begunok);

    VERBOSE_SIGNAL(require);

    if (*begunok == requirement)
        {
        begunok++;
        // Anomaly
        // printf ("Ive required and now ama here: %c\n", *(*begunok));
        }
    else
        {
        syntax_error (*begunok, BAD_REQUIREMENT);
        }
    }


int get_t (void)
    {
    VERBOSE_SIGNAL(get_t);

    int val = get_d ();

    while (*begunok == '*' || *begunok == '/')
        {
        char oper = *begunok++;
        int val2 = get_d ();

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


int get_d (void)
    {
    VERBOSE_SIGNAL(get_d);

    int val = get_p ();

    if (*begunok == '^')
        {
        begunok++;
        int val2 = get_p ();

        val = pow (val, val2);
        }

    return val;
    }


int get_e (void)
    {
    VERBOSE_SIGNAL(get_e);

    int val = get_t ();

    while (*begunok == '+' || *begunok == '-')
        {
        char oper = *begunok++;
        int val2 = get_t ();

        if (oper == '+')
            {
            val += val2;
            }
        else if (oper == '-')
            {
            val -= val2;
            }
        }

    return val;
    }


int get_p (void)
    {
    VERBOSE_SIGNAL(get_p);

    if (*begunok == '(')
        {
        begunok++;
        int val = get_e ();

        require (')');

        return val;
        }
    else
        {
        return get_n ();
        }
    }


int get_g (void)
    {
    VERBOSE_SIGNAL(get_g);

    int val = get_e ();

    require ('$');

    return val;
    }

