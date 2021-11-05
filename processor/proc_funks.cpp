#include <stdio.h>
#include "processor.h"
#define _USE_MATH_DEFINES
#include <math.h>
#undef NDEBUG
#include <assert.h>


#define DEF_CMD(name, id, args_num, code)\
        case cmd_##name:                 \
            {code;                       \
            prc->ip = prc->ip + args_num;\
            break;}


#define PUSH(ARG)                       \
    {int arg = ARG;\
    if ((cmd & CMD_MASK) == cmd_push)   \
        {                               \
        if (cmd & IMM_MASK)             \
            {                           \
            push (prc, arg);            \
            }                           \
        else if (cmd & REG_MASK)        \
            {                           \
            push (prc, prc->reg[arg]);  \
            }                           \
        else if (cmd & RAM_MASK)        \
            {                           \
            push (prc, prc->ram[arg]);  \
            }                           \
        }                               \
    else                                \
        {                               \
        push (prc, arg);                \
        } }

#define POPR(ARG)                                \
    {int arg = ARG;                              \
    if (cmd & REG_MASK)                          \
        {                                        \
        prc->reg[arg] = POP;                     \
        }                                        \
    else if (cmd & RAM_MASK)                     \
        {                                        \
        prc->ram[arg] = POP;                     \
        }   }

#define POP pop (prc)

#define JUMP(ARG) jump (prc, ARG)

#define CONDITION_JUMP(CONDITION)\
    if (CONDITION)               \
        JUMP(ARG1)

#define OUT out (prc)

#define IN in (prc)

#define ARG1 prc->recipe[prc->ip]

#define HLT hlt (prc)

#define VM_SHOW vm_show (prc)

#define CIRC circle (prc)

#define MAKE_DOT {make_dot (prc, POP, POP);} // x, y

#define R_COS (int) round (cos (POP * M_PI / 180) * POP)

#define R_SIN (int) round (sin (POP * M_PI / 180) * POP)

#define SQRT(ARG) (int) round (sqrt (ARG))


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

    for (size_t ram_id = 0; ram_id < RAM_SIZE; ram_id++)
        {
        prc->ram[ram_id] = '+';
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

    stop_inst_console_close ();

    exit (EXIT_SUCCESS);
    }


void vm_show (proc* prc)
    {
    for (size_t y = 0; y < VIDEO_HEIGHT; y++)
        {
        for (size_t x = 0; x < VIDEO_WEEDTH; x++)
            {
            printf ("%c ", prc->ram[NON_VIDEO_RAM_SIZE + y * VIDEO_WEEDTH + x]);
            }

        putchar ('\n');
        }
    }


void vm_print (proc* prc)
    {
    for (size_t y = 0; y < VIDEO_HEIGHT; y++)
        {
        for (size_t x = 0; x < VIDEO_WEEDTH; x++)
            {
            fprintf (prc->prc_log, "%c ", prc->ram[NON_VIDEO_RAM_SIZE + y * VIDEO_WEEDTH + x]);
            }

        putc ('\n', prc->prc_log);
        }
    }


void circle (proc* prc)
    {
    size_t R = 3;

    double angels [] = {0, M_PI / 6, M_PI / 4, M_PI / 2.5, M_PI / 2};

    size_t ANGELS_NUM = sizeof (angels) / sizeof (double);

    for (size_t angel_id = 0; angel_id < ANGELS_NUM; angel_id++)
        {
        int x = (int) round (cos (angels[angel_id]) * R);
        int y = (int) round (sin (angels[angel_id]) * R);

        make_dot (prc, x + 3, y + 3);
        make_dot (prc, x + 3, -y + 3);
        make_dot (prc, -x + 3, y + 3);
        make_dot (prc, -x + 3, -y + 3);
        }
    }


void make_dot (proc* prc, int x, int y)
    {
    prc->ram[NON_VIDEO_RAM_SIZE + y * VIDEO_WEEDTH + x] = 'o';
    }


int get_close_katet (int angel, int R)
    {
    return (int) round (cos (angel / 180.0 * M_PI) * R);
    }

int get_far_katet (int angel, int R)
    {
    return (int) round (sin (angel / 180.0 * M_PI) * R);
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

    vm_print (prc);

    LOG_NEW_LINE;
    LOG_NEW_LINE;
    fflush (prc->prc_log);
    }

