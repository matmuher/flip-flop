#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void proniknovenie_v_stack ()
    {
    FILE* log = fopen ("log.txt", "w");

    const int ARRAY_SIZE = 10;
    struct
        {
        int left_noj_v_selezenku[ARRAY_SIZE];
        stack stk;
        int right_noj_v_pechen[ARRAY_SIZE];
        } canaries_attack = {};

    stack_ctor (&canaries_attack.stk, log);

    for (size_t natural_number = 1; natural_number < 8; natural_number++)
        {
        stack_push (&canaries_attack.stk, natural_number);
        }

    // LEFT STRUCTURE ATTACK
    for (int i = 0; i <= ARRAY_SIZE; i++)
        {
        canaries_attack.left_noj_v_selezenku[i] = 4;
        }

    stack_pop (&canaries_attack.stk);

    // RIGHT STRUCTURE ATTACK
    for (int j = ARRAY_SIZE; j >= -2; --j)
        {
        canaries_attack.right_noj_v_pechen[j] = 5;
        }

    stack_pop (&canaries_attack.stk);

    char* left_val_killer = (char*) canaries_attack.stk.capacity_ptr;
    char* right_val_killer = (char*)
                             (canaries_attack.stk.data + canaries_attack.stk.capacity - 1) + sizeof (double);

    // LEFT VALUES ARRAY ATTACK
    *left_val_killer = 12;

    stack_pop (&canaries_attack.stk);

    // RIGHT VALUES ARRAY ATTACK
    *right_val_killer = 1;

    stack_pop (&canaries_attack.stk);

    char* hash_medvejatnik = (char*) canaries_attack.stk.data;

    hash_medvejatnik[2] = 22;

    stack_pop (&canaries_attack.stk);

    stack_dtor (&canaries_attack.stk);

    fclose (log);
    }
