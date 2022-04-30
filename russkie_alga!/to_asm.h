#ifndef TO_ASM_H_INCLUDED
#define TO_ASM_H_INCLUDED

dict collect_vars (dict ma_dict, node* root);

dict try_node (dict ma_dict, node* current_node);

void st_assembly (node* root, FILE* asm_file, dict ma_dict, int is_main = 0);




#endif // TO_ASM_H_INCLUDED
