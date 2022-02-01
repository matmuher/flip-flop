#include <stdio.h>
#include "processor.h"
#define _USE_MATH_DEFINES
#include <math.h>
#undef NDEBUG
#include <assert.h>


void proc_ctor (proc* prc, size_t bin_size, int* cooking_list, FILE* log, FILE* prc_log)
    {
    stack_ctor (&prc->stk, log);

    prc->ip = 0;

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

    puts ("START MASHINA!\n");
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


#include "DSL.cpp"
#define DEF_CMD(name, id, args_num, code)\
        case cmd_##name:                 \
            {code;                       \
            prc->ip = prc->ip + args_num;\
            break;}
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
#undef DEF_CMD


void vm_print (proc* prc);

#define LOG_NEW_LINE putc ('\n', prc->prc_log)
void proc_dmp (proc* prc)
    {
    for (int byte_id = 0; byte_id < prc->bin_size; byte_id++)
        {
        fprintf (prc->prc_log, "%2d ", byte_id);
        }
    LOG_NEW_LINE;

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
#undef LOG_NEW_FILE


// Proc commands
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
    assert (prc);

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


