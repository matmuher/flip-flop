#ifndef LEXO_PARSER_H_INCLUDED
#define LEXO_PARSER_H_INCLUDED

union token_content
    {
    char* id;
    double val;
    int servant;
    };


enum token_type
    {
    T_END = -1,
    T_VAL = 1,
    T_SFUNK = 2,
    T_OP = 3,
    T_PARENTH = 4,
    T_SFRAME = 5,
    T_VAR = 6, // TOVAR or TVAR'?
    T_OO_LONG_NAME = 7,
    T_COMP = 8,
    T_LINE = 9,
    T_DELIM = 10,
    T_DEF = 11,
    T_RET = 12,
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

void print_token (token token_to_print, int mode);

void print_pl (parsed_line_reader* pl_reader);

#endif // LEXO_PARSER_H_INCLUDED
