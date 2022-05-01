#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dict.h"


const int DICT_INIT_VALUE = 0xBADEDA;


dict_shelf put_in_shelf (dict_shelf ma_shelf, dict ma_dict)
    {
    dict_shelf new_shelf = (dict_shelf) calloc (1, sizeof (dict_shelf_element));

    new_shelf->prev = ma_shelf;
    new_shelf->cur_dic = ma_dict;

    return new_shelf;
    }


dict_cell* search_in_shelf (dict_shelf ma_shelf, const char* word)
    {
    assert (word);

    if (ma_shelf)
        {
        dict_shelf cur_shelf = ma_shelf;

        while (cur_shelf)
            {
            dict_cell* found_word = NULL;
            found_word = search_in_dict (cur_shelf->cur_dic, word);

            if (found_word)
                {
                return found_word;
                }

            cur_shelf = cur_shelf->prev;
            }
        }

    return NULL;
    }


void free_shelf (dict_shelf ma_shelf)
    {
    while (ma_shelf)
        {
        free_dict (ma_shelf->cur_dic);

        dict_shelf doomed_shelf = ma_shelf;

        ma_shelf = ma_shelf->prev;

        free (doomed_shelf);
        }

    puts ("Shelf have been freed!");
    }


void print_shelf (dict_shelf ma_shelf)
    {
    size_t shelf_id = 0;

    while (ma_shelf)
        {
        printf ("Shelf [%d]:\n", shelf_id++);

        print_dict (ma_shelf->cur_dic);

        putchar ('\n');

        ma_shelf = ma_shelf->prev;
        }
    }


dict add_dict_cell (dict ma_dict, const char* key, int value)
    {
    // Allocate memory for new dict_cell
    dict_cell* cell_ptr = (dict_cell*) calloc (1, sizeof (dict_cell)); // Free!


    cell_ptr->key = strdup (key); // Free!
    cell_ptr->value = value;
    cell_ptr->prev = ma_dict;

    return cell_ptr;
    }


void print_dict (dict ma_dict)
    {
    dict cell_ptr = ma_dict;

    while (cell_ptr)
        {
        printf ("%s:%d ", cell_ptr->key, cell_ptr->value);

        cell_ptr = cell_ptr->prev;
        }

    putchar ('\n');
    }


dict search_in_dict (dict ma_dict, const char* word)
    {
    assert (word);

    if (ma_dict)
        {
        dict_cell* cur_cell = ma_dict;

        while (cur_cell)
            {
            const int EQUAL = 0;

            if (strcmp (word, cur_cell->key) == EQUAL)
                {
                return cur_cell;
                }

            cur_cell = cur_cell->prev;
            }
        }

    return NULL;
    }


int dict_get_val (dict ma_dict,const char* key)
    {
    dict_cell* found_cell = search_in_dict (ma_dict, key);

    if (found_cell)
        {
        return found_cell->value;
        }

    printf ("[ERROR] Bad key: %s\n", key);
    const int NO_KEY = -1;
    return NO_KEY;
    }


void dict_write_val (dict ma_dict,const char* key, int write_val)
    {
    dict_cell* found_cell = search_in_dict (ma_dict, key);

    if (found_cell)
        {
        found_cell->value = write_val;
        }
    else
        {
        printf ("[Warning] Key was not found: %s\n", key);
        }
    }


void free_dict (dict doomed_dict)
    {
    dict_cell* cur_cell = doomed_dict;

    while (cur_cell)
        {
        free (cur_cell->key);
        dict_cell* doomed_cell = cur_cell;

        cur_cell = cur_cell->prev;

        free (doomed_cell);
        }

    // puts ("Dictionary was freed!");
    }


dict_shelf delete_dict_from_shelf (dict_shelf ma_shelf)
    {
    if (ma_shelf)
        {
        dict_shelf doomed_shelf_element = ma_shelf;

        ma_shelf = ma_shelf->prev;

        free_dict (doomed_shelf_element->cur_dic);
        free (doomed_shelf_element);
        }

    return ma_shelf;
    }
