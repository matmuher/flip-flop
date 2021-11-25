#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>


#include "list.h"


//============================CONSTANTS=================================

const size_t ACCURACY = 1, WIDTH = 5; //<- for dump format output


int NO_ERRORS = 0x000, BAD_HEAD_MASK = 0x001,
    BAD_TAIL_MASK = 0x002, BAD_CONNECTION_MASK = 0x004; //<- for verification
                                                        // error containing

enum dump_level
    {
    ERRORS_LVL = -1,


    FIRST_LVL = 1,


    SECOND_LVL = 2,
    };


#define VERBOSE SECOND_LVL


#define SHOW_MODE
#undef SHOW_MODE


//============================FUNCTIONS=================================

// __VERIFICATOR__

static int lst_verify (List* lst)
    {
    int cell = lst->head;


    int errors = NO_ERRORS;


    // Go through connections of list

    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        if (cell == (int) NULL)
            {
            break;
            }


        if (lst->prev[lst->next[cell]] == cell)
            {
            #if VERBOSE <= ERRORS_LVL

                printf ("Bad prev connection:\n"
                        "%d -> %d\n"
                        "%d !<- %d\n\n", cell, lst->next[cell],
                                         cell, lst->next[cell]);

            #endif


            break;
            }


        cell = lst->next[cell];
        }


    // If go-round ended not in the zero

    if (cell != (int) NULL)
        {
        #if VERBOSE <= ERRORS_LVL

            puts ("[ERROR]: List bad connection");

        #endif


        errors = errors | BAD_CONNECTION_MASK;
        }


    if (lst->head < (int) NULL)
        {
        #if VERBOSE <= ERRORS_LVL

            printf ("[ERROR]: Incorrect head = %d\n", lst->head);

        #endif


        errors = errors | BAD_HEAD_MASK;
        }


    if (lst->tail < (int) NULL)
        {
        // Kak sdelat' tak:
        // Chtoby mozhno bylo VERBOSE lyubogo levela
        // I dlya lyubovogo levela mozhno bylo vklyuchat'-vyklyuchat' pokaz oshibok
        // Mozhno cherez dopolnitelnii define otvechayushchiy za oshibki
        // No mb mozhno elegantneye?

        #if VERBOSE <= ERRORS_LVL

            printf ("[ERROR]: Incorrect tail = %d\n", lst->tail);

        #endif


        errors = errors | BAD_TAIL_MASK;
        }


    return errors;
    }


// __CONSTRUCTOR__

static void owlist_ctor (List* lst)
    {
    // Pointers inheritance

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


    lst->free.next[lst->free.tail] = 0;
    };


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


    lst->graph_log = fopen ("lst_log.dot", "w");


    lst->text_log = fopen ("lst_log.txt", "w");


    // Allocate data

    lst->data = (double*) calloc (lst->lst_size, sizeof (double));


    lst->sorted_data = (double*) calloc (lst->lst_size, sizeof (double));


    lst->next = (int*) calloc (lst->lst_size, sizeof (int));


    lst->prev = (int*) calloc (lst->lst_size, sizeof (int));


    // Prepare data

    fill_array (lst->next, lst->lst_size, FREE_CELL);


    fill_array (lst->prev, lst->lst_size, FREE_CELL);


    lst->next[0] = (int) NULL;


    lst->prev[0] = (int) NULL;


    lst->tail = EMPTY_LST_EDGE;


    lst->head = EMPTY_LST_EDGE;


    owlist_ctor (lst);


    #if VERBOSE >= FIRST_LVL

        puts ("[CONSTRUCTOR]");

        lst_dmp (lst);

        #ifdef SHOW_MODE

            cool_dmp (lst);

        #endif

    #endif
    }


// __DESTRUCTOR__

void lst_dtor (List* lst)
    {
    #if VERBOSE >= FIRST_LVL


        puts ("[DESTRUCTOR]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    assert (lst);


    free (lst->data);


    free (lst->next);


    free (lst->prev);


    fclose (lst->graph_log);


    fclose (lst->text_log);
    }


// __FREE ELEMENTS LIST FUNCTIONS__

// Push element in free list

static void push_free (OWList* free, int free_cell)
    {
    // Mark cell as free

    free->data[free_cell] = FREE_CELL;


    if (free->head != (int) NULL)
        {
        // Connect previous tail and fresh free_cell

        free->next[free->tail] = free_cell;


        // Free cell becomes tail

        // Transfer tail pointer

        free->tail = free_cell;


        // Tail must point to 0

        free->next[free_cell] = (int) NULL;
        }
    else // First element after resize
        {
        free->head = free_cell;


        free->tail = free_cell;


        free->next[free->tail] = (int) NULL; // Tail must point to 0
        }
    }


// Pop element from free list

static int pop_free (OWList* free)
    {
    assert (free);


    // Catch empty free list

    if (free->head == (int) NULL)
        {
        return LST_IS_FULL; // Doubled_linked list full
        }


    int return_free_cell = free->head;


    // Update head

    free->head = free->next[free->head];


    // Indicate last free element pop'ed

    if (free->head == (int) NULL)
        {
        free->tail = (int) NULL;
        }


    // Give free_cell

    return return_free_cell;
    }


// Find free element for insert

static int find_free (List* lst)
    {
    assert (lst);


    return pop_free (&lst->free);
    }


void lst_props (List* lst, const char* messege)
    {
    printf ("%s:\n\n", messege);


    printf ("List: data[%p], next[%p], prev[%p]\n",
            lst->data, lst->next, lst->prev);


    printf ("Free: data[%p], next[%p]\n",
            lst->free.data, lst->free.next);


    putchar('\n');


    putchar('\n');
    }


// __LIST RESIZE__

static int lst_resize (List* lst)
    {
    #if VERBOSE >= SECOND_LVL


        puts ("[PRE-RESIZE]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    size_t new_elements_num = lst->lst_size * 2;


    printf ("Resize is happening: %d -> %d\n", lst->lst_size, new_elements_num);


    // Expand "data" array

    lst->data = (double*) realloc (lst->data, new_elements_num * sizeof (double));


    lst->sorted_data = (double*) calloc (new_elements_num, sizeof (double));


    // Replace trash with zeros


    size_t shift = new_elements_num - lst->lst_size;


    for (size_t cell_id = 0; cell_id < shift; cell_id++)
            {
            lst->data[lst->lst_size + cell_id] = 0;
            }


    // Expand "prev" array

    lst->prev = (int*) realloc (lst->prev, new_elements_num * sizeof (int));


    lst->free.data = lst->prev;


    for (size_t cell_id = 0; cell_id < shift; cell_id++)
                {
                lst->prev[lst->lst_size + cell_id] = FREE_CELL;
                }


    // Expand "free" array

    lst->next = (int*) realloc (lst->next, new_elements_num * sizeof (int));


    lst->free.next = lst->next;


    for (size_t cell_id = 0; cell_id < shift; cell_id++)
        {
        push_free (&lst->free, lst->lst_size + cell_id);
        }


    lst->lst_size = new_elements_num;


    #if VERBOSE >= FIRST_LVL


        puts ("[POST-RESIZE]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    return find_free (lst);
    }


// __LIST ELEMENT INSERT__

static void bad_insert_print (char* error)
    {
    printf ("[BAD INSERT: %s]\n", error);
    }


int lst_insert_after (List* lst, double value, size_t insert_after_this_cell_id)
    {
    #if VERBOSE >= SECOND_LVL


        puts ("[PRE-INSERT]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif

    // Find free place

    int free_cell_id = find_free (lst);


    printf ("free cell is %d\n", free_cell_id);


    // Resize

    if (free_cell_id == LST_IS_FULL)
        {
        free_cell_id = lst_resize (lst);
        }


    // Errors catch

    if (free_cell_id == LST_IS_FULL)
        {
        bad_insert_print ("LIST IS FULL");


        return LST_IS_FULL;
        }
    else if (free_cell_id == LST_IS_EMPTY)
        {
        bad_insert_print ("LIST IS EMPTY");


        return LST_IS_EMPTY;
        }


    // Put value to "new" cell

    lst->data[free_cell_id] = value;


    // Connect "new" and next["after"] cells via new's "next"

    lst->next[free_cell_id] = lst->next[insert_after_this_cell_id];


    // Connect "new" and "after" cells via new's "prev"

    lst->prev[free_cell_id] = insert_after_this_cell_id;


    if (lst->next[free_cell_id] != (int) NULL)
        {
        lst->prev[lst->next[free_cell_id]] = free_cell_id;
        }


    if (lst->prev[free_cell_id] != (int) NULL)
        {
        lst->next[lst->prev[free_cell_id]] = free_cell_id;
        }


    if (insert_after_this_cell_id == lst->tail)
        {
        lst->tail = free_cell_id;
        }


    // First time here? Set tail and head

    if (lst->head == EMPTY_LST_EDGE)
        {
        lst->head = free_cell_id;
        }


    #if VERBOSE >= FIRST_LVL


        puts ("[POST-INSERT]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    return free_cell_id;
    }


int lst_insert_back (List* lst, double value)
    {
    #if VERBOSE  >= SECOND_LVL


        puts ("[PRE-INSERT-BACK]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    int val_place = lst_insert_after (lst, value, lst->tail);


    #if VERBOSE >= FIRST_LVL


        puts ("[POST-INSERT-BACK]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    return val_place;
    }


// __LIST ELEMENT DELETE__

void lst_delete_cell (List* lst, size_t delete_cell_id)
    {
    #if VERBOSE >= SECOND_LVL


        puts ("[PRE-DELETE]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    int prev_cell = lst->prev[delete_cell_id];


    if (lst->prev[delete_cell_id] != FREE_CELL)
        {
        if (delete_cell_id == lst->head)
            {
            lst->head = lst->next[lst->head];
            }
        else if (delete_cell_id == lst->tail)
            {
            lst->tail = lst->prev[lst->tail];
            }


        // Connect before "delete_cell" and after cells

        if (prev_cell != (int) NULL)
            {
            lst->next[prev_cell] = lst->next[delete_cell_id];
            }


        if (lst->next[delete_cell_id] != (int) NULL)
            {
            lst->prev[lst->next[delete_cell_id]] = prev_cell;
            }


        // Add deleted cell to free list

        push_free (&lst->free, delete_cell_id);
        }
    else
        {
        puts ("[ERROR: TRY TO DELETE EMPTY CELL]");
        }


    #if VERBOSE >= FIRST_LVL


        puts ("[POST-DELETE]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif
    }


// __DUMP USING GRAPHVIZ__

void cool_dmp (List* lst)
    {
    fputs ("digraph G { rankdir = LR;\n", lst->graph_log);


    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        if (lst->prev[cell_id] == FREE_CELL)
            {
            fprintf (lst->graph_log, "node_%d[shape = record, rank = %d, label = \" <id_%d> cell_id:\n %d\"];\n",
                     cell_id, cell_id, cell_id, cell_id);
            }
        else
            {
            fprintf (lst->graph_log, "node_%d[shape = record, rank = %d, label = \" <id_%d> cell_id:\n %d |"
                               "{ <next_%d> next:\n%d | data:\n%.3f | prev:\n%d}\"];\n",
                    cell_id, cell_id, cell_id, cell_id,
                    cell_id, lst->next[cell_id], lst->data[cell_id], lst->prev[cell_id]);
            }
        }


    for (size_t cell_id = 0; cell_id < lst->lst_size; cell_id++)
        {
        if (lst->prev[cell_id] != FREE_CELL)
            {
            fprintf (lst->graph_log, "node_%d:<next_%d> -> node_%d:<id_%d> [color = \"red\"];\n",
                     cell_id, cell_id, lst->next[cell_id], lst->next[cell_id]);
            }
        }


    for (size_t cell_id = 0; cell_id < lst->lst_size - 1; cell_id++)
        {
        // Place in physical order

        fprintf (lst->graph_log, "node_%d:<id_%d> -> node_%d:<id_%d> [color = \"white\"];\n",
                 cell_id, cell_id, cell_id + 1, cell_id + 1);
        }


    fprintf (lst->graph_log, "tail[label = \"tail\"];\n"
                             "head[label = \"head\"];\n");


    fprintf (lst->graph_log, "tail -> node_%d: <id_%d> [color = \"green\"];\n"
                             "head -> node_%d: <id_%d>[color = \"green\"];\n",
            lst->tail, lst->tail, lst->head, lst->head);


    fputs ("}", lst->graph_log);


    fflush (lst->graph_log);


    cool_dmp_show ();


    // Clean .dot file

    lst->graph_log = fopen ("lst_log.dot", "w"); // Will be closed in destructor
    }


// __SHOW GRAPHVIZ DUMP__

void cool_dmp_show ()
    {
    FILE* show_bat = fopen ("show.bat", "w");


    int cur_clock = clock ();


    fprintf (show_bat, "dot lst_log.dot -T png -o dump\\list_log_%d.png\n"
                       "start dump\\list_log_%d.png\n", cur_clock, cur_clock);


    fclose (show_bat);


    system ("show.bat");
    }


// __CONSOLE DUMP__

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
        printf ("[%*.*f] ", WIDTH, ACCURACY, lst->data[cell_id]);
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


    printf ("LIST: head = %d, tail = %d\n", lst->head, lst->tail);


    printf ("FREE: head = %d, tail = %d\n", lst->free.head, lst->free.tail);


    putchar ('\n');
    }


//============================SLOW FUNCTIONS=======================================

void lst_linearize (List* lst)
    {
    #if VERBOSE >= SECOND_LVL


        puts ("[PRE-LINEARIZE]");


        lst_dmp (lst);


        #ifdef SHOW_MODE


            cool_dmp (lst);


        #endif


    #endif


    int cell_id = lst->head;


    if (lst->head != (int) NULL)
        {
        // In linearized list first physic element is first logic
        lst->head = 1;


        lst->tail = lst->head;
        }
    else
        {
        lst->head = (int) NULL;


        lst->tail = (int) NULL;
        }


    int first_free = true;


    // Sort using additional array

    for (size_t step_id = 1; step_id < lst->lst_size; step_id++)
        {
        // Process free cells

        if (cell_id == (int) NULL)
            {
            if (first_free)
                {
                lst->free.head = step_id;


                first_free = false;
                }


            lst->free.data[step_id] = FREE_CELL;


            if (step_id < lst->lst_size - 1)
                {
                lst->free.next[step_id] = step_id + 1;
                }


            lst->free.tail = step_id;


            continue;
            }


        // Process non-free cells

        lst->sorted_data[step_id] = lst->data[cell_id];


        cell_id = lst->next[cell_id];


        if (lst->next[cell_id] != (int) NULL)
            {
            lst->tail = lst->next[cell_id];
            }
        }


    // Put linear connections

    for (size_t step_id = 1; step_id <= lst->tail; step_id++)
        {
        lst->next[step_id] = step_id + 1;


        lst->prev[step_id] = step_id - 1;
        }


    // Put zeros

    lst->next[lst->free.head - 1] = (int) NULL;


    lst->prev[1] = (int) NULL;


    lst->free.next[lst->free.tail] = (int) NULL;


    // Transfer data from additional array to user's one

    memcpy (lst->data, lst->sorted_data, lst->lst_size * sizeof (double));


    #if VERBOSE >= FIRST_LVL

        puts ("POST-LINEARIZE]");

        lst_dmp (lst);

        #ifdef SHOW_MODE

            cool_dmp (lst);

        #endif

    #endif
    }


// God damn slow
int find_free_out_of_date (List* lst)
    {
    assert (lst);


    size_t cell_id = 0;


    // Find first free element by linear search
    while (cell_id < lst->lst_size)
        {
        if (lst->next[cell_id] == FREE_CELL)
            {
            return cell_id;
            }


        cell_id++;
        }


    return LST_IS_FULL;
    }


// God damn slow
int find_prev_out_of_date (List* lst, int cell_id)
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


// God damn slow
void lst_delete_cell_out_of_date (List* lst, size_t delete_cell_id)
    {
    int prev_cell = find_prev_out_of_date (lst, delete_cell_id);


    lst->next[prev_cell] = lst->next[delete_cell_id];


    lst->next[delete_cell_id] = FREE_CELL;


    lst->data[delete_cell_id] = (int) NULL;
    }
