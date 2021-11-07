#include <stdio.h>
#include "processor.h"
#undef NDEBUG
#include <assert.h>


#define DEF_CMD(name, id, args_num, code)                                    \
        case cmd_##name:                                                     \
            {                                                                \
            fprintf (disasm, "%s ", #name);                                  \
            for (int arg_id = 0; arg_id < args_num; arg_id++)                \
                {                                                            \
                if (cmd & IMM_MASK)                                          \
                    {                                                        \
                    fprintf (disasm, "%d ", cooking_list[ip + arg_id]);      \
                    }                                                        \
                else if (cmd & REG_MASK)                                     \
                    {                                                        \
                    fprintf (disasm, "(reg#%d) ", cooking_list[ip + arg_id]);\
                    }                                                        \
                else if (cmd & RAM_MASK)                                     \
                    {                                                        \
                    fprintf (disasm, "[%d] ", cooking_list[ip + arg_id]);    \
                    }                                                        \
                else                                                         \
                    {                                                        \
                    fprintf (disasm, "%d ", cooking_list[ip + arg_id]);      \
                    }                                                        \
                }                                                            \
            putc ('\n', disasm);                                             \
                                                                             \
            ip = ip + args_num;\                                             \
            break;                                                           \
            }
void recogniser (int bin_size, int* cooking_list, FILE* disasm)
    {
    int ip = 0;

    while (ip < bin_size)
        {
        cmd_enum cmd = (cmd_enum) cooking_list[ip++];

        switch (cmd & CMD_MASK)
        {
        #include "cmd.h"
        default:
            printf ("Undefined cmd: %d\n", cmd);
            fprintf (disasm, "non-recognised cmd\n");
        }
        }
    }
#undef DEF_CMD
