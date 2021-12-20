#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "great_grammar.h"


// To add a function you should add it to enum IDS, char* keywords[], determine_keyword (),
// and describe its work in execute ()
const char* sfunk[] = {"sinus", "cosus", "logus", "print"};
const char* sframe[] = {"while", "if"};
size_t sfunk_num = sizeof (sfunk) / sizeof (sfunk[0]);
size_t sframe_num = sizeof (sframe) / sizeof (sframe[0]);



token_type check_keyword (char* suspect)
    {
    const int EQUAL = 0;

    // !Copypaste: add function of search in array, smth like isin ()
    for (size_t sfunk_id = 0; sfunk_id < sfunk_num; sfunk_id++)
            {
            if (strcmp (sfunk[sfunk_id], suspect) == EQUAL)
                {
                return T_SFUNK;
                }
            }

    for (size_t sframe_id = 0; sframe_id < sframe_num; sframe_id++)
        {
        if (strcmp (sframe[sframe_id], suspect) == EQUAL)
            {
            return T_SFRAME;
            }
        }

    return T_VAR;
    }


int are_equal (const char* left, const char* right)
    {
    const int EQUAL = 0; // !Better to put in header or smth

    return strcmp (left, right) == EQUAL;
    }


#define IS_THIS(unknown_kw, kw) \
    if (are_equal (unknown_kw, #kw)) \
        {                            \
        return kw;                   \
        }
kws determine_kw (char* unknown_kw)
    {
    IS_THIS(unknown_kw, sinus)
    IS_THIS(unknown_kw, cosus)
    IS_THIS(unknown_kw, logus)
    IS_THIS(unknown_kw, print)

    return undef_kw;
    }
#undef IS_THIS



int isoper (char begunok)
    {

    switch (begunok)
        {
        case '*':
        case '/':
        case '^':
        case '+':
        case '-':
        case ':': // !double equality symbol (also in comparison)

        return true;

        default:

        return false;
        }
    }


int iscomp (char begunok)
    {
    switch (begunok)
        {
        case '=':
        case '>':
        case '<':

        return true;

        default:

        return false;
        }
    }


int isparenth (char begunok)
    {
    switch (begunok)
        {
        case ')':
        case '(':
        case '}':
        case '{':

        return true;

        default:

        return false;
        }
    }


token_type try_Id (const char* begunok)
    {
    // 7 (God's number) + 1  for '\0'

    char id[NAME_LENGTH] = {};
    size_t id_begunok = 0;
    const char* start_Id = begunok;

    // Starts with letter? -> kinda identifier
    if (isalpha(*begunok))
        {
        id[id_begunok++] = *begunok;
        begunok++;

        // Identifier can consist of both letters and numbers
        // For instance: ma7vey2003, slava1305, iosif1878
        // But not: %var1, 34orange, !cucumber_num
        while (isalnum(*begunok))
            {
            // "Kratkost' - sestra talanta"
            if (id_begunok == NAME_LENGTH)
                {

                printf ("Too long identifier name: %s\n", start_Id);
                // syntax_error (pl_reader, LONG_ID);

                return T_OO_LONG_NAME;
                }

            id[id_begunok++] = *begunok;
            begunok++;
            }

        // Identifier can match with:
        // SFUNK - standard functions (sinus, cosus)
        // SFRAME - standard constructions (while, if)
        // None of then -> it is var
        return check_keyword (id);
        }
    // If no - non-identifier, lol :)
    else
        {
        if (isoper (*begunok))
            {
            return T_OP;
            }
        if (iscomp (*begunok))
            {
            return T_COMP;
            }
        if (isdigit (*begunok))
            {
            return T_VAL;
            }
        if (isparenth (*begunok))
            {
            return T_PARENTH;
            }
        if (*begunok == ';')
            {
            return T_DELIM;
            }
        if (*begunok == '#')
            {
            return T_LINE;
            }
        if (*begunok == '$')
            {
            return T_END;
            }
        }
    }


double dig_out_val (const char** begunok)
    {
    double val = 0;

    // Read integer part
    while (isdigit (*(*begunok)))
        {
        val = val * 10 + (*(*begunok)++ - '0');
        }

    // Read dot part
    if (*(*begunok) == '.')
        {
        (*begunok)++;

        double dot_digits_counter = 0;
        double dot_digits = 0;

        while (isdigit (*(*begunok)))
            {
            dot_digits = dot_digits * 10 + (*(*begunok)++ - '0');
            dot_digits_counter++;
            }

        val = val + dot_digits * (pow (10, -dot_digits_counter));
        }

    return val;
    }


char* dig_out_id (const char** begunok)
    {
    char id[NAME_LENGTH] = {};
    size_t id_begunok = 0;


    while (isalnum(*(*begunok)))
        {
        id[id_begunok++] = *(*begunok);
        (*begunok)++;
        }

    size_t id_len = strlen (id);

    char* return_id = (char*) elephant_calloc (id_len + 1, sizeof (char));
    strcpy (return_id, id);

    return return_id;
    }



