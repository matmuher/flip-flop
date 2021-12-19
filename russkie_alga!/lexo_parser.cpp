#include <stdio.h>
#include <string.h>
#include "great_grammar.h"
#include "lexo_parser.h"

token* lexo_parse (const char* line)
    {
    size_t line_length = strlen (line);

    token* parsed_line = (token*) calloc (line_length, sizeof (token));

    const char* begunok = line;
    size_t token_id = 0;


    // Suspicious define: how to in elegant way
    // concatenate "content." and "id"?
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
    while (begunok < line + line_length)
        {
        token_type token_t = try_Id (begunok);

        switch (token_t)
            {
            TOK_PROC (T_VAR, ID_ASS)
            TOK_PROC (T_KW, ID_ASS)
            TOK_PROC (T_VAL, VAL_ASS)
            TOK_PROC (T_OP, CHAR_ASS)
            TOK_PROC (T_PARENTH_O, CHAR_ASS)
            TOK_PROC (T_PARENTH_C, CHAR_ASS)
            TOK_PROC (T_END, CHAR_ASS)
            default:
                {
                puts ("Nevedomya dich occured! Unknown token:");
                puts (begunok);
                }
            }
        }
    #undef TOK_PROC

    parsed_line = (token*) elephant_realloc (parsed_line, token_id, sizeof (token));

    return parsed_line;
    }


void print_pl (parsed_line_reader* pl_reader)
    {
    size_t token_id = 0;

    while (pl_reader->pl[token_id].type != T_END)
        {
        switch (pl_reader->pl[token_id].type)
            {
            case T_VAR:
            case T_KW:
                printf ("%s ", pl_reader->pl[token_id].content.id);
                token_id++;
                break;
            case T_OP:
            case T_PARENTH_O:
            case T_PARENTH_C:
                printf ("%c ", pl_reader->pl[token_id].content.servant);
                token_id++;
                break;
            case T_VAL:
                printf ("%lf ", pl_reader->pl[token_id].content.val);
                token_id++;
                break;
            }
        }

    putchar ('\n');
    }


void print_token (token token_to_print)
    {
    switch (token_to_print.type)
        {
        case T_VAR:
        case T_KW:
            printf ("%s\n", token_to_print.content.id);
            break;
        case T_OP:
        case T_PARENTH_O:
        case T_PARENTH_C:
            printf ("%c\n", token_to_print.content.servant);
            break;
        case T_VAL:
            printf ("%lf\n", token_to_print.content.val);
            break;
        }
    }
