#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"


/*!
@brief  Second traversal to differ user functions from ordinary variables
*/
token* lexo_parse_second_traversal (token* parsed_line)
    {
    size_t token_id = 0;

    while (parsed_line[token_id].type != T_END)
        {
        int func_def =  parsed_line[token_id + 1].type == T_DELIM        &&
                        parsed_line[token_id + 1].content.servant == ' ' &&
                        parsed_line[token_id + 2].type == T_ROUND_BR;

        int func_call = parsed_line[token_id + 1].type == T_ROUND_BR &&
                        parsed_line[token_id + 1].content.servant == '(';

        if (parsed_line[token_id].type == T_VAR && (func_def || func_call))
            {
            parsed_line[token_id].type = T_UFUNK;
            }

        token_id++;
        }

    return parsed_line;
    }


/*!
@brief  Parse string into tokens:
        for comprehensive list of tokens
        take a look in "lexo_parser.h, enum token_type"
*/
token* lexo_parse (const char* line)
    {
    size_t line_length = strlen (line);

    token* parsed_line = (token*) calloc (line_length, sizeof (token));

    const char* begunok = line;
    size_t token_id = 0;


    /*
    [ATTENTION! gypsy tricks]
    Suspicious define: how to in elegant way
    concatenate "content." and "ass"?
    */
    #define TOK_PROC(TYPE, ass)                   \
        case TYPE:                                \
            {                                     \
            parsed_line[token_id].type = TYPE;    \
            parsed_line[token_id++].content.\
            ass;\
            break;                                \
            }
    #define ID_ASS id = dig_out_id (&begunok)
    #define VAL_ASS val = dig_out_val (&begunok)
    #define CHAR_ASS servant = *begunok++

    // Only tabs and new lines can be used freely to format code
    // Spaces are to be set in places, established by grammar
    while (begunok < line + line_length)
        {
        // Skip '\n' and sequences of spaces
        if (*begunok == '\n' ||
            *begunok == '\t' ||
           ((begunok + 1 != line + line_length) && *begunok == ' ' && *(begunok + 1) == ' '))
             {
             begunok++;
             continue;
             }

        token_type token_t = try_Id (begunok);

        switch (token_t)
            {
            TOK_PROC (T_VAR, ID_ASS)
            TOK_PROC (T_SFUNK, ID_ASS)
            TOK_PROC (T_VAL, VAL_ASS)
            TOK_PROC (T_OP, CHAR_ASS)
            TOK_PROC (T_SQUARE_BR, CHAR_ASS)
            TOK_PROC (T_ROUND_BR, CHAR_ASS)
            TOK_PROC (T_END, CHAR_ASS)
            TOK_PROC (T_COMP, CHAR_ASS)
            TOK_PROC (T_LINE, CHAR_ASS)
            TOK_PROC (T_DELIM, CHAR_ASS)
            TOK_PROC (T_SFRAME, ID_ASS)
            TOK_PROC (T_DEF, ID_ASS)
            TOK_PROC (T_RET, ID_ASS)

            default: printf ("[UNKNOWN TOKEN] : %s\n", begunok);
            }
        }
    #undef TOK_PROC

    // it's token of last processed token, so it shows how many tokens there are in line
    //                                                       \/
    parsed_line = (token*) elephant_realloc (parsed_line, token_id, sizeof (token));

    return parsed_line;
    }


/*!
@brief  Prints parsed line (stands for pl)
*/
void print_pl (parsed_line_reader* pl_reader)
    {
    size_t token_id = 0;

    while (pl_reader->pl[token_id - 1].type != T_END)
        {
        printf("(%d)", token_id);
        print_token (pl_reader->pl[token_id++], 0);
        }

    putchar ('\n');
    }


// print token modes
const size_t NEW_LINE     = 1,
             NON_NEW_LINE = 0;

void print_token (token token_to_print, int delimiter_mode) // 1 - with '\n', 0 - without
    {
    const int NEW_LINE_MODE = 1;
    const int SPACE_MODE = 0;

    switch (token_to_print.type)
        {
        case T_VAR:
        case T_UFUNK:
        case T_SFUNK:
        case T_SFRAME:
        case T_DEF:
        case T_RET:
            {
            printf ("%s[%d] ", token_to_print.content.id, token_to_print.type);

            if (delimiter_mode == NEW_LINE_MODE)
                {
                putchar ('\n');
                }

            break;
            }

        case T_OP:
        case T_ROUND_BR:
        case T_SQUARE_BR:
        case T_COMP:
        case T_DELIM:
        case T_LINE:
        case T_END:
            {
            printf ("%c[%d] ", token_to_print.content.servant, token_to_print.type);

            if (delimiter_mode == NEW_LINE_MODE) putchar ('\n');

            break;
            }

        case T_VAL:
            {
            printf ("%lf[%d] ", token_to_print.content.val, token_to_print.type);

            if (delimiter_mode == NEW_LINE_MODE) putchar ('\n');

            break;
            }
        }
    }
