#include <stdio.h>
#include "list.h"


// Verify function

int main ()
    {
    List lst = {};


    lst_ctor (&lst, 8);


    lst_dmp (&lst);


    for (size_t fil_val = 2; fil_val < 5; fil_val++)
        {
        lst_insert_back (&lst, fil_val);
        lst_dmp (&lst);
        }

    lst_insert_after (&lst, 6.66, 3);
    lst_dmp (&lst);
    lst_insert_after (&lst, 9.9, 1);
    lst_dmp (&lst);

    lst_delete_cell (&lst, 2);
    lst_dmp (&lst);
    }
