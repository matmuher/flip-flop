#include <stdio.h>
#include "processor.h"


#define DEF_CMD(name, id, args_num, code)                                    \
        if (string_equal (#name, code_line->words_ptr))                      \
            {                                                                \
            int cmd_id = cmd_##name;                                         \
                                                                             \
            *begunok++ = cmd_id;                                             \
                                                                             \
            for (int arg_id = 1; arg_id <= args_num; arg_id++)               \
                {                                                            \
                int arg = -1;                                                \
                                                                             \
                char* str_arg = apply_to (code_line, arg_id + 1);            \
                if (str_arg[0] == ':')                                       \
                    {                                                        \
                    size_t marker_id = 0;                                    \
                    if (!sscanf (str_arg, ":%d", &marker_id))                \
                            {                                                \
                            printf ("%s is bad mark\n", str_arg);            \
                            }                                                \
                                                                             \
                    *begunok++ = markers[marker_id];                         \
                    }                                                        \
                else if (str_arg[0] == '[')                                  \
                    {                                                        \
                    size_t ram_id = 0;                                       \
                    if (!sscanf (str_arg, "[%d]", &ram_id))                  \
                        {                                                    \
                        printf ("%s is bad ram_id\n", str_arg);              \
                        }                                                    \
                                                                             \
                    *(begunok - 1) = cmd_id | 0x80;                          \
                    *begunok++ = ram_id;                                     \
                    }                                                        \
                else if (sscanf (str_arg, "%d", &arg))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x20;                          \
                    *begunok++ = arg;                                        \
                    }                                                        \
                else if (string_equal (str_arg, "ax"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 0;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "bx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 1;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "cx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 2;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "dx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 3;                                          \
                    }                                                        \
                else                                                         \
                    {                                                        \
                    printf ("[WARNING] %s is bad arg\n", str_arg);           \
                    }                                                        \
                }                                                            \
            }                                                                \
        else

                                                                              // *дмс регистры, копипаст -> функция
line_buf* get_code (char* file_name, size_t* lines_num)
    {
    assert (file_name != NULL);
    assert (lines_num != NULL);

    line_buf* code = NULL;

    code = get_strings (file_name, lines_num);

    return code;
    }


int* create_binary (line_buf* code, size_t lines_num, size_t* bin_size)
    {
    assert (code);
    assert (lines_num > 0);
    assert (bin_size);

    // 1 for command code and MAX_ARGS_NUM for his argument-fellows  --\/--
    int* binary = (int*) calloc (lines_num, sizeof (int) *     (1 + MAX_ARGS_NUM));
    int* begunok = binary;
    int markers[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (size_t iteration_id = 0; iteration_id < 2; iteration_id++) // *запомнить адреса меток
        {
        begunok = binary;

        for (size_t line_id = 0; line_id < lines_num; line_id++)
            {
            parsed_line code_line = string_delimit (code[line_id].beg_ptr, ' ');

            if (code_line.words_ptr[0] == ':')
                {
                size_t marker_id = 0;
                sscanf (code_line.words_ptr, ":%d", &marker_id);

                size_t jump_to = 0;
                jump_to = begunok - binary - 1;

                markers[marker_id] = jump_to;

                continue; // *метки словами, комментарии
                }

            if (sqtr_equal) // *дефайн который ссылается на cmd_process
            begunok = cmd_line_process (&code_line, begunok, markers);
            }
        }

    *bin_size = begunok - binary;

    binary = (int*) realloc (binary, *bin_size * sizeof (int));

    return binary;
    }

void write_binary (int* binary, size_t bin_size)
    {
    assert (binary);
    assert (bin_size > 0);

    FILE* bin_file = fopen ("binary_my_binary", "wb");

    bin_info ma_bin = {MY_SIGN, bin_size, version};

    if (!fwrite (&ma_bin, sizeof (char), sizeof (ma_bin), bin_file))
        {
        puts ("Trouble during writing struct to file");
        }

    if (!fwrite (binary, sizeof (int), bin_size, bin_file))
        {
        puts ("Trouble during writing recipe to file");
        }

    fclose (bin_file);
    }

int* cmd_line_process (parsed_line* code_line, int* begunok, int* markers)
    {

            {                                                                \
            int cmd_id = cmd_##name;                                         \
                                                                             \
            *begunok++ = cmd_id;                                             \
                                                                             \
            for (int arg_id = 1; arg_id <= args_num; arg_id++)               \
                {                                                            \
                int arg = -1;                                                \
                                                                             \
                char* str_arg = apply_to (code_line, arg_id + 1);            \
                if (str_arg[0] == ':')                                       \
                    {                                                        \
                    size_t marker_id = 0;                                    \
                    if (!sscanf (str_arg, ":%d", &marker_id))                \
                            {                                                \
                            printf ("%s is bad mark\n", str_arg);            \
                            }                                                \
                                                                             \
                    *begunok++ = markers[marker_id];                         \
                    }                                                        \
                else if (str_arg[0] == '[')                                  \
                    {                                                        \
                    size_t ram_id = 0;                                       \
                    if (!sscanf (str_arg, "[%d]", &ram_id))                  \
                        {                                                    \
                        printf ("%s is bad ram_id\n", str_arg);              \
                        }                                                    \
                                                                             \
                    *(begunok - 1) = cmd_id | 0x80;                          \
                    *begunok++ = ram_id;                                     \
                    }                                                        \
                else if (sscanf (str_arg, "%d", &arg))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x20;                          \
                    *begunok++ = arg;                                        \
                    }                                                        \
                else if (string_equal (str_arg, "ax"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 0;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "bx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 1;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "cx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 2;                                          \
                    }                                                        \
                else if (string_equal (str_arg, "dx"))                       \
                    {                                                        \
                    *(begunok - 1) = cmd_id | 0x40;                          \
                    *begunok++ = 3;                                          \
                    }                                                        \
                else                                                         \
                    {                                                        \
                    printf ("[WARNING] %s is bad arg\n", str_arg);           \
                    }                                                        \
                }                                                            \
            }                                                                \
        else


    #include "cmd.h"
    // else
        {
        *begunok++ = unknow_cmd;
        printf ("%s: UNKNOW COMMAND USED!\n", code_line->words_ptr);
        }

    return begunok;
    }

#undef DEF_CMD(name, id, args_num, code)
#define DEF_CMD(name, id, args_num, code) case cmd_##name:

void non_unique_cmd_ids ()
    {
    switch (1)
        {
        #include "cmd.h"
        default:
            puts ("Don't joke with clones"
                  "(c) Pennywise the Dancing Clown");
        }
    }
