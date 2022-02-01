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
    const int NAME_LENGTH = 8; // 7 (God's number) + 1  for '\0'

    char id[NAME_LENGTH] = {};
    size_t id_begunok = 0;

    if (isalpha(*exp_reader->begunok))
        {
        id[id_begunok++] = *exp_reader->begunok;
        exp_reader->begunok++;

        while (isalnum(*exp_reader->begunok))
            {
            if (id_begunok == NAME_LENGTH)
                {
                syntax_error (*exp_reader->begunok, LONG_ID);

                return too_long_name;
                }

            id[id_begunok++] = *exp_reader->begunok;
            exp_reader->begunok++;
            }

        puts (id);

        if (check_keyword (id))
            {
            return determine_keyword (id);
            }

        return var;
        }
    else
        {
        return non_id; // Bad identifier reading
        }
    }



