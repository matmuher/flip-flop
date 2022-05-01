#ifndef DICT_H_INCLUDED
#define DICT_H_INCLUDED

#include <stdio.h>

struct dict_cell
    {
    char* key;
    int value;
    dict_cell* prev;
    };

typedef dict_cell* dict;

// Two-way list of dictionaries
struct dict_shelf_element
    {
    dict cur_dic;
    dict_shelf_element* prev;
    };

typedef dict_shelf_element* dict_shelf;

extern const int DICT_INIT_VALUE;

dict add_dict_cell (dict ma_dict, const char* key, int value);

void print_dict (dict_cell* cell_ptr);

dict search_in_dict (dict ma_dict,const char* word);

int dict_get_val (dict ma_dict,const char* key);

void dict_write_val (dict ma_dict,const char* key, int write_val);

void free_dict (dict doomed_dict);


dict_shelf put_in_shelf (dict_shelf ma_shelf, dict ma_dict);

dict_cell* search_in_shelf (dict_shelf ma_shelf, const char* word);

void print_shelf (dict_shelf ma_shelf);

dict_shelf delete_dict_from_shelf (dict_shelf ma_shelf);

void free_shelf (dict_shelf ma_shelf);

#endif
