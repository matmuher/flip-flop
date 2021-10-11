#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <winbase.h>
#include "..\strings\string_funk.h"
#include "stack.h"

typedef double canary_type;

canary_type CANARY = 0xBAADF00D;


void stack_ctor (stack* stk)
    {
    assert (!IsBadReadPtr (stk, sizeof (stack)));
    // provide stack's innocence
    stk->data = NULL;
    stk->capacity_ptr = NULL;
    stk->grosse = 0;
    stk->capacity = 0;
    stk->left_struct_canary = CANARY;
    stk->right_struct_canary = CANARY;
    stk->stack_hash = get_stack_hash (stk);

    event.ctor = TRUE;
    }


void stack_verify (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));
    assert (event.ctor == TRUE);
    assert (ERROR_ID.stack_is_destroyed == FALSE);

    if (IsBadReadPtr (stk, sizeof (stack)))
        {
        ERROR_ID.bad_stack_ptr = TRUE;
        }
    else
        {
        if (stk->grosse < 0 || stk->grosse > stk->capacity)
            {
            ERROR_ID.invalid_size = TRUE;
            }
        if (stk->grosse < 0)
            {
            ERROR_ID.nothing_to_pop = TRUE;
            }
        if (stk->grosse > stk->capacity)
            {
            ERROR_ID.overflow = TRUE;
            }
        if (stk->capacity < 0)
            {
            ERROR_ID.invalid_capacity = TRUE;
            }

        // Structure: canary check
        if (stk->left_struct_canary != CANARY)
            {
            ERROR_ID.bad_struc_left_canary = TRUE;
            }
        if (stk->right_struct_canary != CANARY)
            {
            ERROR_ID.bad_struc_right_canary = TRUE;
            }

        // Values array check
        if (IsBadReadPtr (stk->capacity_ptr,
                         stk->capacity * sizeof (int) + 2 * sizeof (canary_type)))
            {
            ERROR_ID.bad_capacity_ptr = TRUE;
            }
        else
            {
            ERROR_ID.bad_capacity_ptr = FALSE;

            // Values array: canary check
            if (*((canary_type*) stk->capacity_ptr) != CANARY)
                {
                ERROR_ID.bad_values_left_canary = TRUE;
                }
            if (*((canary_type*)((char*) stk->data + stk->capacity * sizeof (int))) != CANARY)
                {
                ERROR_ID.bad_values_right_canary = TRUE;
                }

            // Check hash
            if (event.push || event.pop || event.dctor)
                {
                int cur_hash = get_stack_hash (stk);

                if (cur_hash != stk->stack_hash)
                    {
                    ERROR_ID.bad_hash = TRUE;
                    }
                }
            else
                {
                stk->stack_hash = get_stack_hash (stk);
                }
            }
        }

    stack_dump (stk, log);
    }


void stack_dump (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));

    fprint_line (log, '-', DELIMITER_LEN);
    fprintf (log, "stack <int> [%p]\n", stk);

    // Is stack readable?
    if (ERROR_ID.bad_stack_ptr == FALSE)
        {
        // Type info about stk structure elements
        fprintf (log, " size: %d, capacity [%p]: %d, data [%p]:\n",
                 stk->grosse, stk->capacity_ptr, stk->capacity, stk->data);

        // Type values array content
        if (ERROR_ID.bad_capacity_ptr == FALSE)
            {
            fputs ("\t{\n", log);
            for (int element_id = 0; element_id < stk->capacity; element_id++)
                {
                if (stk->data[element_id] == POISON)
                    {
                    fprintf (log, "\tdata[%d] = POISONED;\n", element_id);
                    }
                else
                    {
                    fprintf (log, "\tdata[%d] = %d;\n", element_id, stk->data[element_id]);
                    }
                }
            fputs ("\t}\n", log);
            }
        // What function was called?:
        if (event.push == HAPPENED)
            {
            fputs ("*Start push*\n", log);
            event.push = PASSED;
            }
        else if (event.pop == HAPPENED)
            {
            fputs ("*Start pop*\n", log);
            event.pop = PASSED;
            }
        else if (event.dctor == HAPPENED)
            {
            fputs ("*Destructor happened*\n", log);
            event.dctor = PASSED;
            }

        // Was memory reallocated?:
        if (event.is_capacity_change == HAPPENED)
            {
            fprintf (log, " // Capacity changed to %d //\n", stk->capacity);
            event.is_capacity_change = 0;
            }

        // Canaries are ok?
        if (ERROR_ID.bad_struc_left_canary == TRUE)
            {
            fputs ("@ Structure: LEFT ATTACK! (ptichku jalko) @\n", log);
            }
        if (ERROR_ID.bad_struc_right_canary == TRUE)
            {
            fputs ("@ Structure: RIGHT ATTACK! (ptichku jalko) @\n", log);
            }

        if (ERROR_ID.bad_values_left_canary == TRUE)
            {
            fputs ("@@ Values array: LEFT ATTACK! (ptichku jalko) @@\n", log);
            }
        if (ERROR_ID.bad_values_right_canary == TRUE)
            {
            fputs ("@@ Values array: RIGHT ATTACK! (ptichku jalko) @@\n", log);
            }

        // Hash is ok?
        if (ERROR_ID.bad_hash == TRUE)
            {
            fputs ("# Hash is broken #\n", log);
            }
        }

    fputs ("\tErros:\n", log);

    if (ERROR_ID.bad_stack_ptr == TRUE)
        {
        fputs ("[ERROR] STACK POINTER IS LOCKED\n", log);
        assert (!IsBadReadPtr (stk, sizeof (stack)));
        }
    else
        {
        if (ERROR_ID.invalid_size == TRUE)
            {
            fputs ("[ERROR] INVALID STACK SIZE\n", log);
            }
        if (ERROR_ID.invalid_capacity == TRUE)
            {
            fputs ("[ERROR] INVALID STACK CAPACITY\n", log);
            }
        if (ERROR_ID.bad_capacity_ptr == TRUE)
            {
            fputs ("[WARNING] CAPACITY POINTER IS LOCKED\n", log);
            assert (!IsBadReadPtr (stk->data, stk->capacity * sizeof (int)));
            }
        if (ERROR_ID.overflow == TRUE)
            {
            fputs ("[ERROR] STACK OVERFLOW\n", log);
            }
        if (ERROR_ID.nothing_to_pop == TRUE)
            {
            fputs ("[ERROR] NOTHING TO POP\n", log);
            }
        }

    fprint_line (log, '-', DELIMITER_LEN);
    fputc ('\n', log);

    fflush (log);
    }


int find_stock (int cur_size)
    {
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


void stack_push (stack* stk, int value, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));

    event.push = HAPPENED;
    stack_verify (stk, log);

    stk->grosse++;
    if (stk->grosse > stk->capacity)
        {
        stk->capacity = find_stock (stk->grosse);


        stk->capacity_ptr = (int*) realloc (stk->capacity_ptr,
                                            stk->capacity * sizeof (int) + 2 * sizeof (canary_type));

        stk->data = (int*) ((char*) stk->capacity_ptr + sizeof (canary_type));

        event.is_capacity_change = HAPPENED;

        // Set first canary
        *((canary_type*) stk->capacity_ptr) = CANARY;
        // Set second canary
        *((canary_type*) (stk->data + stk->capacity)) = CANARY;

        // Clear tales (chistim hvosty (poison trash-memory))
        for (size_t element_id = stk->grosse; element_id < stk->capacity; element_id++)
            {
            stk->data[element_id] = POISON;
            }
        }

    // Push value
    stk->data[stk->grosse - 1] = value;

    stack_verify (stk, log);
    }

int stack_pop (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));

    event.pop = HAPPENED;
    stack_verify (stk, log);

    int pop_elem = stk->data[--stk->grosse];

    // Decrease stack
    if (stk->grosse <= stk->capacity / 4) // shifted decrease
        {
        stk->capacity = stk->capacity / 2;
        stk->capacity_ptr = (int*) realloc (stk->capacity_ptr,
                                            stk->capacity * sizeof (int) + 2 * sizeof (canary_type));

        stk->data = (int*) ((char*) stk->capacity_ptr + sizeof (canary_type));

        event.is_capacity_change = 1;

        // Set first canary
        *((canary_type*) stk->capacity_ptr) = CANARY;
        // Set second canary
        *((canary_type*) ((char*) stk->data + stk->capacity * sizeof (int))) = CANARY;
        }

    stack_verify (stk, log);

    return pop_elem;
    }


void stack_dtor (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));

    event.dctor = HAPPENED;
    stack_verify (stk, log);

    // Poison stack
    for (size_t element_id = 0; element_id < stk->capacity; element_id++)
        {
        stk->data[element_id] = POISON;
        }

    free (stk->capacity_ptr);

    stk->grosse = 0;
    stk->data = (int*) POISON;
    stk->capacity_ptr = (int*) POISON;
    stk = (stack*) POISON;

    ERROR_ID.stack_is_destroyed = TRUE;
    // Kak obnulit ERROR_IDS structure?
    }

int get_stack_hash (stack* stk)
    {
    int stack_hash = 0;
    int seed = 1799;

    for (int element_id = 0; element_id < stk->grosse; element_id ++)
        {
        stack_hash = stk->data[element_id] * seed + stack_hash + element_id + stk->grosse + stk->capacity;
        }

    return stack_hash;
    }
