#include "list.h"


#include <stdio.h>
#include <stdlib.h>


static int* fill_array (int* arr, size_t arr_size, int filler)
    {
    for (int cell_id = 0; cell_id < arr_size; cell_id++)
        {
        arr[cell_id] = filler;
        }

    return arr;
    }


void lst_ctor (List* lst, size_t lst_size)
    {
    lst->lst_size = lst_size;


    lst->data = (double*) calloc (lst->lst_size, sizeof (double));

    lst->next = (int*) calloc (lst->lst_size, sizeof (int));

    lst->prev = (int*) calloc (lst->lst_size, sizeof (int));


    fill_array (lst->next, lst->lst_size, FREE_CELL);

    fill_array (lst->prev, lst->lst_size, FREE_CELL);


    lst->next[0] = (int) NULL;

    lst->prev[0] = (int) NULL;

    lst->tail = EMPTY_LST_EDGE;

    lst->head = EMPTY_LST_EDGE;

    owlist_ctor (lst);
    }


void owlist_ctor (List* lst)
    {
    lst->free.lst_size = lst->lst_size;


    lst->free.data = lst->prev;


    lst->free.next = lst->next;


    // One-connection of free cells
    for (size_t cell_id = 1; cell_id < lst->free.lst_size; cell_id++)
        {
        lst->next[cell_id] = cell_id + 1;
        }


    // Provide correct head
    lst->free.head = 1;


    // Provide correct tail
    lst->free.tail = lst->free.lst_size - 1;
    lst->free.next[lst->free.lst_size - 1] = 0;
    };


void lst_dmp (List* lst)
    {
    printf ("cell_id: ");

    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        printf ("[%*d] ", WIDTH, cell_id);
        }

    putchar ('\n');


    printf ("   data: ");

    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        printf ("[%*.*f] ", WIDTH, ACCURACY, lst->data[cell_id], lst->next[cell_id]);
        }

    putchar ('\n');


    printf ("   next: ");

    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        printf ("[%*d] ", WIDTH, lst->next[cell_id]);
        }

    putchar ('\n');


    printf ("   prev: ");

    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        printf ("[%*d] ", WIDTH, lst->prev[cell_id]);
        }

    putchar ('\n');


    printf ("head = %d, tail = %d\n", lst->head, lst->tail);

    putchar ('\n');
    }


int find_free_out_of_date (List* lst)
    {
    size_t cell_id = 0;

    while (cell_id < lst->lst_size)
        {
        if (lst->next[cell_id] == FREE_CELL)
            {
            return cell_id;
            }

        cell_id++;
        }

    // delete from zanachka

    return LST_IS_FULL;
    }


int lst_insert_back (List* lst, double value)
    {
    int val_place = lst_insert_after (lst, value, lst->tail);

    // Move tail
    lst->tail = val_place;

    return val_place;
    }


int lst_insert_after (List* lst, double value, size_t insert_after_this_cell_id)
    {
    int free_cell_id = find_free (lst); // Find free place


    if (free_cell_id == LST_IS_FULL) // Check overflow
        {
        puts ("[BAD INSERT: LIST IF FULL]");

        return LST_IS_FULL;
        }


    // First time here? Set tail and head
    if (lst->tail == EMPTY_LST_EDGE && lst->head == EMPTY_LST_EDGE)
        {
        lst->tail = free_cell_id;

        lst->head = free_cell_id;
        }


    // Put value to "insert"
    lst->data[free_cell_id] = value;


    // Transfer index of next element from "after" to "insert"
    lst->next[free_cell_id] = lst->next[insert_after_this_cell_id];


    // Transfer index of "insert" to 'prev' of "after"
    lst->prev[free_cell_id] = insert_after_this_cell_id;


    if (insert_after_this_cell_id != (int) NULL) // Zero cell is untouchable
        {
        // Connect "after" and "insert" cells
        lst->next[insert_after_this_cell_id] = free_cell_id;

        // Connect "insert" and "next" cells
        lst->prev[lst->next[free_cell_id]] = free_cell_id;
        }


    return free_cell_id;
    }


int find_prev (List* lst, int cell_id)
    {
    int cur_id = lst->head;

    while (lst->next[cur_id] != cell_id && lst->next[cur_id] != (int) NULL)
        {
        cur_id = lst->next[cur_id];
        }

    if (cur_id == (int) NULL)
        {
        puts ("There is no such cell id in list");
        }

    return cur_id;
    }


void lst_delete_cell_out_of_date (List* lst, size_t delete_cell_id)
    {
    int prev_cell = find_prev (lst, delete_cell_id);


    lst->next[prev_cell] = lst->next[delete_cell_id];


    lst->next[delete_cell_id] = FREE_CELL;
    lst->data[delete_cell_id] = (int) NULL;
    }


void lst_delete_cell (List* lst, size_t delete_cell_id)
    {
    int prev_cell = lst->prev[delete_cell_id];


    lst->next[prev_cell] = lst->next[delete_cell_id];
    lst->prev[lst->next[delete_cell_id]] = prev_cell;



    lst->next[delete_cell_id] = FREE_CELL;
    lst->prev[delete_cell_id] = FREE_CELL;
    lst->data[delete_cell_id] = (int) NULL;


    push_free (&lst->free, delete_cell_id);
    }                      // sort с выносом
                         // prev как unsigned
                            // lineriazation
void push_free (OWList* free, int free_cell)
    {
    // Connect previous tail and fresh free_cell
    free->next[free->tail] = free_cell;

    // Update tail information
    free->tail = free_cell;
    free->next[free_cell] = 0;
    }


int pop_free (OWList* free)
    {
    int return_free_cell = free->head;

    // Update head
    free->head = free->next[free->head];

    // Give free_cell
    return return_free_cell;
    }


int find_free (List* lst)
    {
    return pop_free (&lst->free);
    }
