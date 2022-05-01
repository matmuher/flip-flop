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
    BAD_TERMINAL = -1,
    BAD_REQUIREMENT = -2,
    LONG_ID = -3,
    COMPARISON_EXPECTED = -4,
    };

const int NAME_LENGTH = 30;
typedef node* ma_ty;

//=============================================================================\\
//                             GRAMMAR SHIT                                    \\
//=============================================================================\\

ma_ty get_G (parsed_line_reader* pl_reader);

ma_ty get_A (parsed_line_reader* pl_reader);

ma_ty get_B (parsed_line_reader* pl_reader);

ma_ty get_E (parsed_line_reader* pl_reader);

ma_ty get_T (parsed_line_reader* pl_reader);

ma_ty get_S (parsed_line_reader* pl_reader);

ma_ty get_F (parsed_line_reader* pl_reader);

ma_ty get_P (parsed_line_reader* pl_reader);

ma_ty get_N (parsed_line_reader* pl_reader);


//=============================================================================\\
//                             SERVICE SHIT                                    \\
//=============================================================================\\

int are_equal (const char* left, const char* right);

ERROR_LIST syntax_error (parsed_line_reader* pl_reader, ERROR_LIST error_code);

void cur_read_pos (parsed_line_reader* pl_reader);

int require (char requirement, parsed_line_reader* pl_reader);

// Copypaste? After all this time? Always...
token_type try_Id (const char* begunok);

double dig_out_val (const char** begunok);

char* dig_out_id (const char** begunok);

void put_HH (char* const buffer, size_t file_size);

//=============================================================================\\
//                             TREE DSL SHIT                                   \\
//=============================================================================\\


//--------------------------------ATLANTS!--------------------------------------

node* bi_oper (node* left, node* right, const char* oper, node_type NODE_TYPE = OP);

node* mono_oper (node* right, const char* oper, node_type NODE_TYPE = OP);

//-----------------------------------------------------------------------------


node* nsin (node* arg);

node* ncos (node* arg);

node* nlog (node* arg);

node* add (node* left, node* right);

node* sub (node* left, node* right);

node* mlt (node* left, node* right);

node* div (node* left, node* right);

node* npow (node* left, node* right);

node* eq (node* left, node* right);

node* gr (node* left, node* right);

node* ls (node* left, node* right);

node* wh (node* left, node* right);

node* ass (node* left, node* right);

node* nprint (node* arg);

node* create_val (const char* val);

node* st (node* left, node* right);

node* param (node* left, node* right);

node* def (node* left, node* right);

node* funk (node* left, node* right);

node* call (node* left, node* right);

#endif // GREAT_GRAMMAR_H_INCLUDED
