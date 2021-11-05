#include <stdio.h>
#include "list.h"






int main ()
    {
    List lst = {};

    lst_ctor (&lst, 8);

    lst_dmp (&lst);

    printf ("free cell is %d\n", find_free (&lst));

    for (size_t fil_val = 2; fil_val < 7; fil_val++)
        {
        lst_insert_back (&lst, fil_val);
        lst_dmp (&lst);
        }

    lst_insert_after (&lst, 6.66, 3);
    lst_dmp (&lst);
    lst_insert_after (&lst, 9.9, 1);
    lst_dmp (&lst);

    printf ("Prev to 2 is %d\n", find_prev (&lst, 7));

    lst_delete_cell (&lst, 2);
    lst_dmp (&lst);
    }
