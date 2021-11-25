#include <stdio.h>
#include "list.h"

// Html log
// Verify function

int main ()
    {
    List lst = {};


    lst_ctor (&lst, 8);


    for (size_t fil_val = 2; fil_val < 5; fil_val++)
        {
        lst_insert_back (&lst, fil_val);
        }

    lst_insert_after (&lst, 6.66, 3);

    lst_insert_after (&lst, 9.9, 1);

    lst_delete_cell (&lst, 2);

    lst_delete_cell (&lst, lst.head);

    lst_delete_cell (&lst, lst.tail);

    for (double filler = 13.5; filler < 50; filler = filler + 2.4)
        {
        lst_insert_back (&lst, filler);
        }

    lst_linearize (&lst);

    lst_dtor (&lst);
    }
