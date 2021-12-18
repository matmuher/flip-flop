#include <stdio.h>
#include <string.h>
#include "great_grammar.h"


union token_content
    {
    char* id;
    double val;
    char servant;
    };





struct token
    {
    token_type type;
    token_content content;
    };


token* lexo_parse (const char* line)
    {
    size_t line_length = strlen (line);

    token* parsed_line = (token*) calloc (line_length, sizeof (token));

    const char* begunok = line;
    size_t token_id = 0;

    while (begunok < line + line_length)
        {
        token_type token_t = try_Id (begunok);

        // !Copypaste : define?
        switch (token_t)
            {
            case T_VAR:
                {
                parsed_line[token_id].type = T_VAR;
                parsed_line[token_id++].content.id = dig_out_id (&begunok);
                break;
                }
            case T_KW:
                {
                parsed_line[token_id].type = T_KW;
                parsed_line[token_id++].content.id = dig_out_id (&begunok);
                break;
                }
            case T_VAL:
                {
                parsed_line[token_id].type = T_VAL;
                parsed_line[token_id++].content.val = dig_out_val (&begunok);
                break;
                }
            case T_OP:
                {
                parsed_line[token_id].type = T_OP;
                parsed_line[token_id++].content.servant = *begunok++;
                break;
                }
            case T_PARENTH_O:
                {
                parsed_line[token_id].type = T_PARENTH_O;
                parsed_line[token_id++].content.servant = *begunok++;
                break;
                }
            case T_PARENTH_C:
                {
                parsed_line[token_id].type = T_PARENTH_C;
                parsed_line[token_id++].content.servant = *begunok++;
                break;
                }
            case T_END:
                {
                parsed_line[token_id].type = T_END;
                parsed_line[token_id++].content.servant = *begunok++;
                break;
                }
            default:
                {
                puts ("Nevedomya dich occured! Unknown token:");
                puts (begunok);
                }
            }
        }

    parsed_line = (token*) elephant_realloc (parsed_line, token_id, sizeof (token));

    return parsed_line;
    }


int main ()
    {
    const char* expression = "(sinus(5^2)+3)*(12-3)$";

    // const char* double_test = "11.$";

    // line_reader double_reader = {double_test, double_test};

    // printf ("Double is %f\n", get_N (&double_reader));

    // printf ("get_G returned %f", get_G (expression));

    token* parsed_line = lexo_parse (expression);

    size_t token_id = 0;

    while (parsed_line[token_id].type != T_END)
        {
        switch (parsed_line[token_id].type)
            {
            case T_VAR:
            case T_KW:
                printf ("%d: %s\n", token_id, parsed_line[token_id].content.id);
                token_id++;
                break;
            case T_OP:
            case T_PARENTH_O:
            case T_PARENTH_C:
                printf ("%d: %c\n", token_id, parsed_line[token_id].content.servant);
                token_id++;
                break;
            case T_VAL:
                printf ("%d: %lf\n", token_id, parsed_line[token_id].content.val);
                token_id++;
                break;
            }
        }

    #if 0
    node* root = get_G (expression);

    dot_this_shit (root);

    #endif
    memory_free ();
    }
