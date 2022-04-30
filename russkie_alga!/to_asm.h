#ifndef TO_ASM_H_INCLUDED
#define TO_ASM_H_INCLUDED


#include <assert.h>
#include <string.h>
#include "..\differ\tree_funks.h"
#include "..\dictionary\dict.h"
#include "..\differ\differ.h"


extern const size_t REVERS,  NON_REVERS,
                    IN_EXPR, OUT_EXPR,
                    IS_MAIN, NON_MAIN;

// TODO: make a structure that contains all this constantly repeating parameters
//                                |--------------\/
void st_assembly (node* root, FILE* asm_file, dict ma_dict, int is_main = NON_MAIN);

void expression_assembly (node* cur_node, FILE* asm_file, dict ma_dict);

void expression_node_assembly (node* cur_node, FILE* asm_file, dict ma_dict);

size_t push_params (node* cur_node, FILE* asm_file, dict ma_dict);

void pop_params (size_t params_num, FILE* asm_file);

void def_assembly (node* def_node, FILE* asm_file, dict ma_dict);

void call_assembly (node* call_node, FILE* asm_file, dict ma_dict, int in_expression = OUT_EXPR);

void init_assembly (node* init_node, FILE* asm_file, dict ma_dict);

void store_value_assembly (node* cur_node, FILE* asm_file, dict ma_dict);

void var_assembly (node* var_node, FILE* asm_file, dict ma_dict, const char* cmd);

void push_val_assembly (node* val_node, FILE* asm_file, dict ma_dict);

void op_assembly (node* op_node, FILE* asm_file, dict ma_dict);

void ret_assembly (node* ret_node, FILE* asm_file, dict ma_dict, int is_main = NON_MAIN);

void cmp_assembly (node* cmp_node, FILE* asm_file, dict ma_dict,
                   const char* mark_name, size_t mark_id, int revers = NON_REVERS);

void sframe_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, int is_main = NON_MAIN);

void if_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, int is_main = NON_MAIN);

void while_assembly (node* sframe_node, FILE* asm_file, dict ma_dict, size_t cond_id, int is_main = NON_MAIN);

dict try_node (dict ma_dict, node* current_node);

#endif // TO_ASM_H_INCLUDED
