#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "elephant_calloc_extern.h"

death_note std_note;

void* elephant_calloc (size_t elements_num, size_t element_size, death_note* note)
    {
    assert (note != NULL);

    // Calloc ptr
    void* ptr = calloc (elements_num, element_size);
    // Memorize ptr
    note->ptrs_list[note->grosse] = ptr;
    note->grosse++;

    return ptr;
    }

void* elephant_realloc (void* user_ptr, size_t elements_num,
                        size_t element_size, death_note* note)
    {
    assert (note != NULL);

    // Calloc ptr
    void* ptr = realloc (user_ptr, elements_num * element_size);
    // Memorize ptr
    note->ptrs_list[note->grosse] = ptr;
    note->grosse++;

    return ptr;
    }

void memory_free (death_note* note)
    {
    assert (note != NULL);

    // printf ("Ptrs used: [%d]\n", note->grosse);

    while (note->grosse-- > 0)
        {
        free (note->ptrs_list[note->grosse]);
        note->ptrs_list[note->grosse] = NULL;
        }

    // puts ("Memory was successfully freed!");
    }
