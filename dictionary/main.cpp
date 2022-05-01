#include <stdio.h>
#include "dict.h"

int main ()
    {
    const char* words1[] = {"a", "b", "c"} ;
    size_t words1_size = sizeof (words1) / sizeof (words1[0]);

    const char* words2[] = {"1", "2", "3"} ;
    size_t words2_size = sizeof (words2) / sizeof (words2[0]);

    dict ma_dict1 = NULL;
    dict ma_dict2 = NULL;



    int value = 12;

    for (size_t word_id = 0; word_id < words1_size; word_id++)
        {
        ma_dict1 = add_dict_cell (ma_dict1, words1[word_id], value++);
        }

    for (size_t word_id = 0; word_id < words2_size; word_id++)
        {
        ma_dict2 = add_dict_cell (ma_dict2, words2[word_id], value++);
        }

    dict_shelf ma_shelf = NULL;
    ma_shelf = put_in_shelf (ma_shelf, ma_dict1);
    ma_shelf = put_in_shelf (ma_shelf, ma_dict2);

    dict found_cell = search_in_shelf (ma_shelf, "3");

    if (found_cell) printf ("%s %d\n", found_cell->key, found_cell->value);
    else puts ("Cell was not found");

    print_shelf (ma_shelf);

    ma_shelf = delete_dict_from_shelf (ma_shelf);

    print_shelf (ma_shelf);

    free_shelf (ma_shelf);

    return 0;
    }
