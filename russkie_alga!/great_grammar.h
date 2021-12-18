#ifndef GREAT_GRAMMAR_H_INCLUDED
#define GREAT_GRAMMAR_H_INCLUDED

#include "..\differ\differ.h"

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

ma_ty get_G (const char* expression);

ma_ty get_E (line_reader* exp_reader);

ma_ty get_T (line_reader* exp_reader);

ma_ty get_S (line_reader* exp_reader);

ma_ty get_F (line_reader* exp_reader);

enum IDS // ?Mojno li eto za extern'it'
    {
    too_long_name = -2,
    undef_kw = -1,
    non_id = 0,
    var = 1,
    sinus = 2,
    cosus = 3,
    logus = 4,
    };

IDS get_Id (line_reader* exp_reader);

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

// Copypaste? After all this time? Always...
token_type try_Id (const char* begunok);

double dig_out_val (const char** begunok);

char* dig_out_id (const char** begunok);



ma_ty get_P (line_reader* exp_reader);

ma_ty get_N (line_reader* exp_reader);


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

ERROR_LIST syntax_error (char current_char, ERROR_LIST error_code);

void cur_read_pos (line_reader* exp_reader);

int require (char requirement, line_reader* exp_reader);

ma_ty execute (IDS id_info, ma_ty arg);


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
