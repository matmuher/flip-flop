#include <stdio.h>
#include "processor.h"
#include "proc_defines.cpp"
#include <math.h>
#undef NDEBUG
#include <assert.h>


#define DEF_CMD(name, id, args_num, code)\
        case cmd_##name:                 \
            {code;                       \
            prc->ip = prc->ip + args_num;\
            break;}


#define PUSH(ARG)                    \
    int karg = ARG;                  \
    if ((cmd & CMD_MASK) == cmd_push)\
    {                                \
    if (cmd & 0x20)                  \
        {                            \
        push (prc, karg);            \
        }                            \
    else if (cmd & 0x40)             \
        {                            \
        push (prc, prc->reg[karg]);  \
        }                            \
    }                                \
    else                             \
        {                            \
        push (prc, karg);            \
        }

#define POPR(ARG) (pop_arg (prc, ARG))

#define POP pop (prc)

#define JUMP(ARG) jump (prc, ARG)

#define CONDITION_JUMP(CONDITION)\
    if (CONDITION)               \
        JUMP(ARG1)

#define OUT out (prc)

#define IN in (prc)

#define ARG1 prc->recipe[prc->ip]

#define HLT hlt (prc)


#define LOG_NEW_LINE putc ('\n', prc->prc_log)


void proc_ctor (proc* prc, size_t bin_size, int* cooking_list, FILE* log, FILE* prc_log)
    {
    stack_ctor (&prc->stk, log);

    prc->ip = 0;

    prc->hlt = 0;

    prc->bin_size = bin_size;

    prc->recipe = cooking_list;

    prc->prc_log = prc_log;

    for (int reg_id = 0; reg_id < 4; reg_id++)
        {
        prc->reg[reg_id] = 0;
        }

    puts ("START MASHINA!");
    }


void push (proc* prc, int val)
    {
    assert (prc);

    stack_push (&prc->stk, val);
    }


int pop (proc* prc)
    {
    assert (prc);

    return stack_pop (&prc->stk);
    }


void jump (proc* prc, size_t jump_to)
    {
    prc->ip = jump_to;
    }


void add (proc* prc)
    {
    assert (prc);

    stack_push (&prc->stk, stack_pop (&prc->stk) + stack_pop (&prc->stk));
    }


void out (proc* prc)
    {
    int temp = stack_pop (&prc->stk);
    stack_push (&prc->stk, temp);

    printf ("%d\n", temp);
    }


void in (proc* prc)
        {
        int arg = -1;

        if (!scanf ("%d", &arg))
            {
            puts ("Bad argument for in");
            }
        else
            {
            push (prc, arg);
            }
        }


void hlt (proc* prc)
    {
    puts ("STOP MASHINA!");

    exit (EXIT_SUCCESS);
    }


void doer (proc* prc)
    {
    while (prc->ip < prc->bin_size)
        {
        proc_dmp (prc);
        cmd_enum cmd = (cmd_enum) prc->recipe[prc->ip++];
        do_cmd (prc, cmd);
        }
    }


void do_cmd (proc* prc, cmd_enum cmd)
    {
    assert (prc);

    switch (cmd & CMD_MASK)
        {
        #include "cmd.h"
        default:
            printf ("Undefined cmd: %d\n", cmd);
        }
    }


void proc_dmp (proc* prc)
    {
    for (int byte_id = 0; byte_id < prc->bin_size; byte_id++)
        {
        fprintf (prc->prc_log, "%2d ", byte_id);
        }
    putc ('\n', prc->prc_log);

    for (int byte_id = 0; byte_id < prc->bin_size; byte_id++)
        {
        fprintf (prc->prc_log, "%2X ", prc->recipe[byte_id]);
        }
    LOG_NEW_LINE;

    fprintf (prc->prc_log, "%*s", prc->ip * 3 + 2, "^");

    LOG_NEW_LINE;

    fprintf (prc->prc_log, "Registers: ax = %d, bx = %d, cx = %d, dx = %d\n",
             prc->reg[0], prc->reg[1], prc->reg[2], prc->reg[3]);
    LOG_NEW_LINE;
    }


void pop_arg (proc* prc, int arg)
    {
    prc->reg[arg] = pop (prc);
    }
