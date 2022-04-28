#ifndef DICT_H_INCLUDED
#define DICT_H_INCLUDED

#include <stdio.h>

struct dict_cell
    {
    char* key;
    int value;
    dict_cell* prev;
    };

extern const int DICT_INIT_VALUE;

typedef dict_cell* dict;

dict add_dict_cell (dict ma_dict,const char* key, int value);

void print_dict (dict_cell* cell_ptr);

dict search_in_dict (dict ma_dict,const char* word);

int dict_get_val (dict ma_dict,const char* key);

void dict_write_val (dict ma_dict,const char* key, int write_val);

void free_dict (dict doomed_dict);

#endif
