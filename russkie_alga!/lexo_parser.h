#ifndef LEXO_PARSER_H_INCLUDED
#define LEXO_PARSER_H_INCLUDED

union token_content
    {
    char* id;
    double val;
    char servant;
    };


enum token_type
    {
    T_END = -1,
    T_VAL = 1,
    T_KW = 2,
    T_OP = 3,
    T_PARENTH_O = 4,
    T_PARENTH_C = 5,
    T_VAR = 6,
    T_OO_LONG_NAME
    };


struct token
    {
    token_type type;
    token_content content;
    };


struct parsed_line_reader
    {
    size_t token_id;
    token* pl;
    };


token* lexo_parse (const char* line);

void print_token (token token_to_print);

void print_pl (parsed_line_reader* pl_reader);

#endif // LEXO_PARSER_H_INCLUDED
