#ifndef TO_ASM_H_INCLUDED
#define TO_ASM_H_INCLUDED

dict collect_vars (dict ma_dict, node* root);

dict try_node (dict ma_dict, node* current_node);

void assembly (node* root, FILE* asm_file, dict ma_dict);



#endif // TO_ASM_H_INCLUDED
