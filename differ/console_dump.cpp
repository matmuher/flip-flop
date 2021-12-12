#include <stdio.h>
#include "differ.h"


void cur_read_pos (tree_reader* t_reader)
    {
    puts (t_reader->root);

    int pointer = t_reader->begunok - t_reader->root + 1;

    printf ("%*c\n", pointer, '^');
    }
