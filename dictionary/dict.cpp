#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct dict_cell
    {
    char* key;
    int value;
    dict_cell* prev;
    };


typedef dict_cell* dict;

#if 1
dict_cell* add_dict_cell (dict ma_dict, char* key, int value)
    {
    // Allocate memory for new dict_cell
    dict_cell* cell_ptr = (dict_cell*) calloc (1, sizeof (dict_cell)); // Free!


    cell_ptr->key = strdup (key); // Free!
    cell_ptr->value = value;
    cell_ptr->prev = ma_dict;

    return cell_ptr;
    }
#endif

void print_dict (dict_cell* cell_ptr)
    {
    if (cell_ptr)
        {
        printf ("%s : %d\n", cell_ptr->key, cell_ptr->value);

        if (cell_ptr->prev)
            {
            print_dict (cell_ptr->prev);
            }
        }
    }


dict_cell* search_in_dict (dict ma_dict, char* word)
    {
    assert (ma_dict && word);

    dict_cell* cur_cell = ma_dict;

    while (cur_cell)
        {
        const int EQUAL = 0; // Is it good idea to define const's here?

        if (strcmp (word, cur_cell->key) == EQUAL)
            {
            return cur_cell;
            }

        cur_cell = cur_cell->prev;
        }

    return NULL;
    }


int dict_get_val (dict ma_dict, char* key)
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


void dict_write_val (dict ma_dict, char* key, int write_val)
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

    puts ("Dictionary was freed!");
    }


#if 0
int main ()
    {
    dict_cell* ma_dict = NULL;

    ma_dict = add_dict_cell (ma_dict, "cow", 4);
    ma_dict = add_dict_cell (ma_dict, "spider", 7);
    ma_dict = add_dict_cell (ma_dict, "spider", 8);

    printf ("cow : %d\n", dict_get_val (ma_dict, "cow"));
    printf ("spider : %d\n", dict_get_val (ma_dict, "spider"));

    dict_write_val (ma_dict, "cow", -17);
    printf ("cow : %d\n", dict_get_val (ma_dict, "cow"));

    free_dict (ma_dict);

    // print_dict (ma_dict);
    }
#endif
