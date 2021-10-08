#include <stdio.h>
#include "stack.h"

int main()
    {
    stack stk;
    stack_ctor (&stk);
    FILE* log = fopen ("log.txt", "w");

    for (int test_push = 0; test_push < 5; test_push++)
        {
        stack_push (&stk, test_push, log);
        }
    for (int test_shift = 0; test_shift < 5; test_shift++)
        {
        stack_pop(&stk, log);
        stack_push(&stk, test_shift, log);
        }
    for (int test_push = 0; test_push < 5; test_push++)
        {
        stack_pop (&stk, log);
        }

    stack_dtor (&stk, log);
    fclose (log);
    }

