#include <stdio.h>
#include <ctype.h>
#include "grammar_lab.h"

const int MAX_STR_LENGTH = 100;
char str[MAX_STR_LENGTH] = "2+7*3-11*(3*3-9)$";
char* begunok = str;


static int syntax_error (char current_char, ERROR_LIST error_code)
    {
    printf ("[SYNTAX ERROR: %d] -> %c\n", error_code, current_char);

    return error_code;
    }

int get_n (void)
    {
    puts ("start n");
    printf ("%c\n", *begunok);

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
    printf ("Ive required and now ama here: %c\n", *begunok);
    printf ("%p\n", begunok);
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
    puts ("start t");
    printf ("%c\n", *begunok);

    int val = get_p ();

    while (*begunok == '*' || *begunok == '/')
        {
        char oper = *begunok++;
        int val2 = get_p ();

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



int get_e (void)
    {
    puts ("start e");
    printf ("%c\n", *begunok);

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
    puts ("start p");
    printf ("%c\n", *begunok);

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
    puts ("start g");
    printf ("%c\n", *begunok);

    int val = get_e ();

    require ('$');

    return val;
    }
