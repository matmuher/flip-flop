#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "great_grammar.h"


// To add a function you should add it to enum IDS, char* keywords[], determine_keyword (),
// and describe its work in execute ()
const char* keywords[] = {"sinus", "cosus", "logus"};
size_t keywords_num = sizeof (keywords) / sizeof (keywords[0]);



int check_keyword (char* suspect)
    {
    const int EQUAL = 0;

    int match = false;

    for (size_t kw_id = 0; kw_id < keywords_num; kw_id++)
            {
            if (strcmp (keywords[kw_id], suspect) == EQUAL)
                {
                match = true;
                break;
                }
            }

    return match;
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
IDS determine_keyword (char* unknown_kw)
    {
    IS_THIS(unknown_kw, sinus)
    IS_THIS(unknown_kw, cosus)
    IS_THIS(unknown_kw, logus)

    return undef_kw;
    }
#undef IS_THIS


/*!
@brief Determines is this chunk of expression is identifier or not.
If it is, determines type of identifier.
@return Element of IDS enum
*/
IDS get_Id (line_reader* exp_reader)
    {
    // 7 (God's number) + 1  for '\0'

    char id[NAME_LENGTH] = {};
    size_t id_begunok = 0;

    // Starts with letter? -> kinda identifier
    if (isalpha(*exp_reader->begunok))
        {
        id[id_begunok++] = *exp_reader->begunok;
        exp_reader->begunok++;

        // Identifier can consist of both letters and numbers
        // For instance: ma7vey2003, slava1305, iosif1878
        // But not: %var1, 34orange, !cucumber_num
        while (isalnum(*exp_reader->begunok))
            {
            // "Kratkost' - sestra talanta"
            if (id_begunok == NAME_LENGTH)
                {
                syntax_error (*exp_reader->begunok, LONG_ID);

                return too_long_name;
                }

            id[id_begunok++] = *exp_reader->begunok;
            exp_reader->begunok++;
            }

        // If identifier is a key_word
        // function returns id of this keyword
        if (check_keyword (id))
            {
            return determine_keyword (id);
            }

        // else it is ordinary variable
        return var;
        }
    // If no - non-identifier, lol :)
    else
        {
        return non_id;
        }
    }



int isoper (char begunok)
    {
    switch (begunok)
        {
        case '*':
        case '/':
        case '^':
        case '+':
        case '-':

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
                syntax_error (*begunok, LONG_ID);

                return T_OO_LONG_NAME;
                }

            id[id_begunok++] = *begunok;
            begunok++;
            }

        // If identifier is a key_word
        // function returns id of this keyword
        if (check_keyword (id))
            {
            return T_KW;
            }

        // else it is ordinary variable
        return T_VAR;
        }
    // If no - non-identifier, lol :)
    else
        {
        if (isoper (*begunok))
            {
            return T_OP;
            }
        if (isdigit (*begunok))
            {
            return T_VAL;
            }
        if (*begunok == '$')
            {
            return T_END;
            }
        if (*begunok == '(')
            {
            return T_PARENTH_O;
            }
        if (*begunok == ')')
            {
            return T_PARENTH_C;
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



