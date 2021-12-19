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
            TOK_PROC (T_SFUNK, ID_ASS)
            TOK_PROC (T_VAL, VAL_ASS)
            TOK_PROC (T_OP, CHAR_ASS)
            TOK_PROC (T_PARENTH, CHAR_ASS)
            TOK_PROC (T_END, CHAR_ASS)
            TOK_PROC (T_COMP, CHAR_ASS)
            TOK_PROC (T_SFRAME, ID_ASS)

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

    while (pl_reader->pl[token_id-1].type != T_END)
        {
        print_token (pl_reader->pl[token_id++], 0);
        }

    putchar ('\n');
    }


void print_token (token token_to_print, int mode) // 1 - with '\n', 0 - without
    {
    const int NEW_LINE_MODE = 1;
    const int SPACE_MODE = 0;

    switch (token_to_print.type)
        {
        case T_VAR:
        case T_SFUNK:
        case T_SFRAME:
            printf ("%s[%d] ", token_to_print.content.id, token_to_print.type);
            if (mode == NEW_LINE_MODE)
                {
                putchar ('\n');
                }
            break;
        case T_OP:
        case T_PARENTH:
        case T_COMP:
        case T_END:
            printf ("%c[%d] ", token_to_print.content.servant, token_to_print.type);
            if (mode == NEW_LINE_MODE)
                {
                putchar ('\n');
                }
            break;
        case T_VAL:
            printf ("%lf[%d] ", token_to_print.content.val, token_to_print.type);
            if (mode == NEW_LINE_MODE)
                {
                putchar ('\n');
                }
            break;
        }
    }
