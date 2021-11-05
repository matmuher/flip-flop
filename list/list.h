#include <stdlib.h>


struct OWList
    {
    int lst_size;

    int* data;

    int* next;

    int tail, head;
    };


struct List
    {
    int lst_size;

    double* data;

    int* next;

    int tail, head;

    int* prev;

    OWList free;
    };


const int FREE_CELL = -1,
          EMPTY_LST_EDGE = 0,
          LST_IS_FULL = 0;


const size_t ACCURACY = 1, WIDTH = 3; //<- for dump format output


void lst_ctor (List* lst, size_t lst_size);


void owlist_ctor (List* lst);


void lst_dmp (List* lst);


int find_free (List* lst);


int lst_insert_back (List* lst, double value);


int lst_insert_after (List* lst, double value, size_t insert_after_this_cell_id);


int find_prev (List* lst, int cell_id);


void lst_delete_cell (List* lst, size_t delete_cell_id);


void push_free (OWList* free, int free_cell);


int pop_free (OWList* free);
