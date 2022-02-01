#include <stdio.h>
#include "list.h"

// Html log
// Verify function

int main ()
    {
    List lst = {};


    lst_ctor (&lst, 8);


    for (size_t fil_val = 2; fil_val < 4; fil_val++)
        {
        lst_insert_after (&lst, fil_val, lst.tail);
        }


    lst_dtor (&lst);
    }
