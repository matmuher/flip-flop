#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <winbase.h>
#include "..\encyclopedia of dutch life\Encyclopedia\hamlet_dynamic_proc\string_funk.h"
#include "stack.h"


void stack_ctor (stack* stk)
    {
    assert (!IsBadReadPtr (stk, sizeof (stack)));
    // provide stack's innocence
    stk->data = NULL;
    stk->grosse = 0;
    stk->capacity = 0;
    }


void stack_verify (stack* stk, FILE* log)
    {
    assert (!IsBadReadPtr (log, TRY_LOG));

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

        if (IsBadReadPtr (stk->data, stk->capacity * sizeof (int)))
            {
            ERROR_ID.bad_data_ptr = TRUE;
            }
        else
            {
            ERROR_ID.bad_data_ptr = FALSE;
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
        // Type info about stk elements
        fprintf (log, " size: %d, capacity: %d, data [%p]:\n",
                 stk->grosse, stk->capacity, stk->data);

        // Type data content
        if (ERROR_ID.bad_data_ptr == FALSE)
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
        }

    fputs ("__Erros__:\n", log);

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
        if (ERROR_ID.bad_data_ptr == TRUE)
            {
            fputs ("[WARNING] DATA POINTER IS LOCKED\n", log);
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
        stk->data = (int*) realloc (stk->data, stk->capacity * sizeof (int));
        event.is_capacity_change = HAPPENED;
        
        // Clear tales (chistim hvosty)
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
        stk->data = (int*) realloc (stk->data, (stk->capacity) * sizeof (int));
        event.is_capacity_change = 1;
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
    stk->grosse = 0;
    free (stk->data);
    stk->data = (int*) POISON;
    stk = (stack*) POISON;
    }
