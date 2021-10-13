#include "stack.h"

#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <winbase.h>
#include "..\strings\string_funk.h"

#ifdef CANARY_DEFENCE
typedef double canary_type;
canary_type CANARY = 0xBAADF00D;
#endif

#define is(condition) condition ? TRUE : FALSE
#define mark_error(condition, error_storage) error_storage = (condition ? TRUE : FALSE)

void stack_ctor (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (stk, sizeof (stack)));
    assert (!IsBadReadPtr (log, TRY_LOG));

    // Provide stack's innocence
    stk->data = NULL;
    stk->capacity_ptr = NULL;
    stk->grosse = 0;
    stk->capacity = 0;
    stk->stk_debug.log = log;

    #ifdef CANARY_DEFENCE
    stk->left_struct_canary = CANARY;
    stk->right_struct_canary = CANARY;
    #endif

    #ifdef HASH_DEFENCE
    stk->stack_hash = get_stack_hash (stk);
    #endif

    stk->stk_event.ctor = TRUE;
    }


void stack_verify (stack* stk)
    {
    assert (!IsBadReadPtr (stk->stk_debug.log, TRY_LOG));
    assert (stk->stk_event.ctor == TRUE);
    assert (stk->stk_debug.error_id.stack_is_destroyed == FALSE);

    if (IsBadReadPtr (stk, sizeof (stack)))
        {
        stk->stk_debug.error_id.bad_stack_ptr = TRUE;
        }
    else
        {
        mark_error(stk->grosse < 0 || stk->grosse > stk->capacity,
                   stk->stk_debug.error_id.invalid_size);

        mark_error(stk->grosse < 0,
                   stk->stk_debug.error_id.nothing_to_pop);

        mark_error(stk->grosse > stk->capacity,
                      stk->stk_debug.error_id.overflow);

        mark_error(stk->capacity < 0,
                   stk->stk_debug.error_id.invalid_capacity);

        #ifdef CANARY_DEFENCE
        // Structure: canary check
        mark_error(stk->left_struct_canary != CANARY,
                   stk->stk_debug.error_id.bad_struc_left_canary);

        mark_error(stk->right_struct_canary != CANARY,
                   stk->stk_debug.error_id.bad_struc_right_canary);
        #endif

        // Values array check
        size_t values_array_size = stk->capacity * sizeof (int);

        #ifdef CANARY_DEFENCE
        values_array_size = values_array_size + 2 * sizeof (canary_type);
        #endif

        mark_error(IsBadReadPtr (stk->data, stk->capacity * sizeof (int)),
                   stk->stk_debug.error_id.bad_data_ptr);

        mark_error(IsBadReadPtr (stk->capacity_ptr, values_array_size),
                   stk->stk_debug.error_id.bad_capacity_ptr);

        if (!stk->stk_debug.error_id.bad_capacity_ptr && !stk->stk_debug.error_id.bad_data_ptr)
            {
            #ifdef CANARY_DEFENCE
            // Values array: canary check
            mark_error(*((canary_type*) stk->capacity_ptr) != CANARY,
                       stk->stk_debug.error_id.bad_values_left_canary);

            mark_error(*((canary_type*)((char*) stk->data + stk->capacity * sizeof (int))) != CANARY,
                       stk->stk_debug.error_id.bad_values_right_canary);
            #endif

            #ifdef HASH_DEFENCE
            // Check hash
            if (stk->stk_event.push || stk->stk_event.pop || stk->stk_event.dctor)
                {
                int cur_hash = get_stack_hash (stk);

                mark_error(cur_hash != stk->stack_hash,
                           stk->stk_debug.error_id.bad_hash)
                }
            else
                {
                stk->stack_hash = get_stack_hash (stk);
                }
            #endif
            }
        }

    stack_dump (stk);
    }


void stack_dump (stack* stk)
    {
    assert (!IsBadReadPtr (stk->stk_debug.log, TRY_LOG));

    fprint_line (stk->stk_debug.log, '-', DELIMITER_LEN);
    fprintf (stk->stk_debug.log, "stack <int> [%p]\n", stk);

    // Is stack readable?
    if (stk->stk_debug.error_id.bad_stack_ptr == FALSE)
        {
        // Type info about stk structure elements
        fprintf (stk->stk_debug.log, " size: %d, capacity [%p]: %d, data [%p]:\n",
                 stk->grosse, stk->capacity_ptr, stk->capacity, stk->data);

        // Type values array content
        if (!stk->stk_debug.error_id.bad_data_ptr && !stk->stk_debug.error_id.bad_capacity_ptr)
            {
            fputs ("\t{\n", stk->stk_debug.log);
            for (int element_id = 0; element_id < stk->capacity; element_id++)
                {
                if (stk->data[element_id] == POISON)
                    {
                    fprintf (stk->stk_debug.log, "\tdata[%d] = POISONED;\n", element_id);
                    }
                else
                    {
                    fprintf (stk->stk_debug.log, "\tdata[%d] = %d;\n", element_id, stk->data[element_id]);
                    }
                }
            fputs ("\t}\n", stk->stk_debug.log);
            }
        // What function was called?:
        if (stk->stk_event.push == HAPPENED)
            {
            fputs ("*Start push*\n", stk->stk_debug.log);
            stk->stk_event.push = PASSED;
            }
        else if (stk->stk_event.pop == HAPPENED)
            {
            fputs ("*Start pop*\n", stk->stk_debug.log);
            stk->stk_event.pop = PASSED;
            }
        else if (stk->stk_event.dctor == HAPPENED)
            {
            fputs ("*Destructor happened*\n", stk->stk_debug.log);
            stk->stk_event.dctor = PASSED;
            }

        // Was memory reallocated?:
        if (stk->stk_event.is_capacity_change == HAPPENED)
            {
            fprintf (stk->stk_debug.log, " // Capacity changed to %d //\n", stk->capacity);
            stk->stk_event.is_capacity_change = 0;
            }

        #ifdef CANARY_DEFENCE
        // Canaries are ok?
        if (stk->stk_debug.error_id.bad_struc_left_canary == TRUE)
            {
            fputs ("@ Structure: LEFT ATTACK! (ptichku jalko) @\n", stk->stk_debug.log);
            }
        if (stk->stk_debug.error_id.bad_struc_right_canary == TRUE)
            {
            fputs ("@ Structure: RIGHT ATTACK! (ptichku jalko) @\n", stk->stk_debug.log);
            }

        if (stk->stk_debug.error_id.bad_values_left_canary == TRUE)
            {
            fputs ("@@ Values array: LEFT ATTACK! (ptichku jalko) @@\n", stk->stk_debug.log);
            }
        if (stk->stk_debug.error_id.bad_values_right_canary == TRUE)
            {
            fputs ("@@ Values array: RIGHT ATTACK! (ptichku jalko) @@\n", stk->stk_debug.log);
            }
        #endif

        #ifdef HASH_DEFENCE
        // Hash is ok?
        if (stk->stk_debug.error_id.bad_hash == TRUE)
            {
            fputs ("# Hash is broken #\n", stk->stk_debug.log);
            }
        #endif
        }

    fputs ("[Erros-Bip-Bop]\n", stk->stk_debug.log);

    if (stk->stk_debug.error_id.bad_stack_ptr == TRUE)
        {
        fputs ("[ERROR] STACK POINTER IS LOCKED\n", stk->stk_debug.log);
        assert (!IsBadReadPtr (stk, sizeof (stack)));
        }
    else
        {
        if (stk->stk_debug.error_id.invalid_size == TRUE)
            {
            fputs ("[ERROR] INVALID STACK SIZE\n", stk->stk_debug.log);
            }
        if (stk->stk_debug.error_id.invalid_capacity == TRUE)
            {
            fputs ("[ERROR] INVALID STACK CAPACITY\n", stk->stk_debug.log);
            }
        if (stk->stk_debug.error_id.bad_capacity_ptr == TRUE)
            {
            fputs ("[WARNING] CAPACITY POINTER IS LOCKED\n", stk->stk_debug.log);
            assert (!stk->stk_debug.error_id.bad_capacity_ptr);
            }
        if (stk->stk_debug.error_id.bad_data_ptr == TRUE)
            {
            fputs ("[WARNING] DATA POINTER IS LOCKED\n", stk->stk_debug.log);
            assert (!stk->stk_debug.error_id.bad_data_ptr);
            }
        if (stk->stk_debug.error_id.overflow == TRUE)
            {
            fputs ("[ERROR] STACK OVERFLOW\n", stk->stk_debug.log);
            }
        if (stk->stk_debug.error_id.nothing_to_pop == TRUE)
            {
            fputs ("[ERROR] NOTHING TO POP\n", stk->stk_debug.log);
            }
        }

    fprint_line (stk->stk_debug.log, '-', DELIMITER_LEN);
    fputc ('\n', stk->stk_debug.log);

    fflush (stk->stk_debug.log);
    }


int find_stock (int cur_size)
    {
    assert (cur_size >= 0);

    int stock = 1;

    if (cur_size % 2 == 0)
        {
        return cur_size;
        }

    while (cur_size != 0)
        {
        stock = stock * 2;
        cur_size = cur_size / 2;
        }

    return stock;
    }


void stack_push (stack* stk, int value)
    {
    assert (!IsBadReadPtr (stk->stk_debug.log, TRY_LOG));

    stk->stk_event.push = HAPPENED;
    stack_verify (stk);

    stk->grosse++;

    if (stk->grosse > stk->capacity)
        {
        stk->capacity = find_stock (stk->grosse);

        update_capacity (stk);

        clear_tales (stk, stk->grosse);
        }

    // Push value
    stk->data[stk->grosse - 1] = value;

    stack_verify (stk);
    }

int stack_pop (stack* stk)
    {
    assert (!IsBadReadPtr (stk->stk_debug.log, TRY_LOG));

    stk->stk_event.pop = HAPPENED;

    stack_verify (stk);

    int pop_elem = stk->data[--stk->grosse];

    // Decrease stack
    if (stk->grosse <= stk->capacity / 4) // shifted decrease
        {
        stk->capacity = stk->capacity / 2;

        update_capacity (stk);
        }

    stack_verify (stk);

    return pop_elem;
    }


void stack_dtor (stack* stk)
    {
    assert (!IsBadReadPtr (stk->stk_debug.log, TRY_LOG));

    stk->stk_event.dctor = HAPPENED;
    stack_verify (stk);

    // Poison stack
    clear_tales (stk, 0);

    free (stk->capacity_ptr);

    stk->grosse = 0;
    stk->data = (int*) POISON;
    stk->capacity_ptr = (int*) POISON;
    stk->stk_debug.log = (FILE*) POISON;
    stk->stk_debug.error_id.stack_is_destroyed = TRUE;
    }

int get_stack_hash (stack* stk)
    {
    int stack_hash = 0;
    int seed = 1799;

    for (int element_id = 0; element_id < stk->grosse; element_id++)
        {
        stack_hash = stk->data[element_id] * seed + stack_hash + element_id + stk->grosse + stk->capacity;
        }

    return stack_hash;
    }

#ifdef CANARY_DEFENCE
int stack_realloc_with_canaries (stack* stk)
    {
    stk->capacity_ptr = (int*) realloc (stk->capacity_ptr,
                                            stk->capacity * sizeof (int) + 2 * sizeof (canary_type));

    stk->data = (int*) ((char*) stk->capacity_ptr + sizeof (canary_type));

    // Set first canary
    *((canary_type*) stk->capacity_ptr) = CANARY;
    // Set second canary
    *((canary_type*) (stk->data + stk->capacity)) = CANARY;

    return stk->capacity;
    }
#endif

int stack_realloc_default (stack* stk)
    {
    stk->capacity_ptr = (int*) realloc (stk->capacity_ptr,
                                        stk->capacity * sizeof (int));
    stk->data = stk->capacity_ptr;

    return stk->capacity;
    }

void update_capacity (stack* stk)
    {
    #ifdef CANARY_DEFENCE
    stack_realloc_with_canaries (stk);

    #else
    stack_realloc_default (stk);

    #endif

    stk->stk_event.is_capacity_change = HAPPENED;
    }


void clear_tales (stack* stk, size_t start_element)
    {
    for (;start_element < stk->capacity; start_element++)
        {
        stk->data[start_element] = POISON;
        }
    }


