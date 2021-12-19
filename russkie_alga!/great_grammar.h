#ifndef GREAT_GRAMMAR_H_INCLUDED
#define GREAT_GRAMMAR_H_INCLUDED

#include "..\differ\differ.h"
#include "lexo_parser.h"


struct line_reader
    {
    const char* root;
    const char* begunok;
    };


enum ERROR_LIST
    {
    BAD_DIGIT = -1,
    BAD_REQUIREMENT = -2,
    LONG_ID = -3,
    };

const int NAME_LENGTH = 8;
typedef node* ma_ty;

//=============================================================================\\
//                             GRAMMAR SHIT                                    \\
//=============================================================================\\

ma_ty get_G (parsed_line_reader* pl_reader);

ma_ty get_E (parsed_line_reader* pl_reader);

ma_ty get_T (parsed_line_reader* pl_reader);

ma_ty get_S (parsed_line_reader* pl_reader);

ma_ty get_F (parsed_line_reader* pl_reader);

ma_ty get_P (parsed_line_reader* pl_reader);

ma_ty get_N (parsed_line_reader* pl_reader);


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code);

void cur_read_pos (parsed_line_reader* pl_reader);

int require (char requirement, parsed_line_reader* pl_reader);

ma_ty execute (parsed_line_reader* pl_reader, size_t kw_token_id, ma_ty arg);


enum kws
    {
    undef_kw = -1,
    sinus = 1,
    cosus = 2,
    logus = 3,
    };

kws determine_kw (char* unknown_kw);

// Copypaste? After all this time? Always...
token_type try_Id (const char* begunok);

double dig_out_val (const char** begunok);

char* dig_out_id (const char** begunok);


//=============================================================================\\
//                             TREE DSL SHIT                                   \\
//=============================================================================\\

node* nsin (node* arg);

node* ncos (node* arg);

node* nlog (node* arg);

node* add (node* left, node* right);

node* sub (node* left, node* right);

node* mlt (node* left, node* right);

node* div (node* left, node* right);

node* npow (node* left, node* right);

node* create_val (const char* val);

#endif // GREAT_GRAMMAR_H_INCLUDED
