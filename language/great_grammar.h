#ifndef GREAT_GRAMMAR_H_INCLUDED
#define GREAT_GRAMMAR_H_INCLUDED


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


typedef double ma_ty;

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

ma_ty get_P (line_reader* exp_reader);

ma_ty get_N (line_reader* exp_reader);


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

ERROR_LIST syntax_error (char current_char, ERROR_LIST error_code);

void cur_read_pos (line_reader* exp_reader);

int require (char requirement, line_reader* exp_reader);

ma_ty execute (IDS id_info, ma_ty arg);


#endif // GREAT_GRAMMAR_H_INCLUDED
