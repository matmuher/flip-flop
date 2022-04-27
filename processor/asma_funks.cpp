#include <stdio.h>
#include "processor.h"
#include "..\dictionary\dict.cpp" // Include cpp - cringe?


int* arg_process (char* str_arg, int* begunok, int* markers, int cmd_id, int arg, dict word_markers);

/*!
@brief Analyzes C-string and return command id
*/
int* cmd_line_process (parsed_line* code_line, int* begunok, int* markers, dict word_markers);



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

    // 1 for command code and MAX_ARGS_NUM for his argument-fellows   --\/--
    int* binary = (int*) calloc (lines_num, sizeof (int)    *   (1 + MAX_ARGS_NUM));
    int* begunok = binary;
    const int MARKERS_NUM = 9;
    int markers[MARKERS_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    dict word_markers = NULL;

    for (size_t iteration_id = 0; iteration_id < 2; iteration_id++)
        {
        begunok = binary;

        for (size_t line_id = 0; line_id < lines_num; line_id++)
            {
            parsed_line code_line = string_delimit (code[line_id].beg_ptr, ' ');

            // Marker process
            if (code_line.words_ptr[0] == ':')
                {
                size_t marker_id = 0;
                sscanf (code_line.words_ptr, ":%d", &marker_id);

                size_t jump_to = 0;
                jump_to = begunok - binary - 1;

                markers[marker_id] = jump_to;

                continue;
                }

            // Word marker process
            if (string_equal (code_line.words_ptr, "def"))
                {
                size_t marker_id = 0;

                const int MARK_LENGTH = 20;
                char word_mark[MARK_LENGTH] = {};

                if (!sscanf (apply_to (&code_line, 2), "%[^:]", word_mark))
                    {
                    printf ("Bad define line: %s\n", code_line.words_ptr);
                    }

                size_t jump_to = 0;

                jump_to = begunok - binary - 1;

                // If there is no record in dictionary yet
                // function will just drop warning
                word_markers = add_dict_cell (word_markers, word_mark, jump_to);

                continue;
                }

            // Command process
            begunok = cmd_line_process (&code_line, begunok, markers, word_markers);
            }
        }

    *bin_size = begunok - binary;

    // If there were commands without arguments
    // less memory was used than calloc'ed
    binary = (int*) realloc (binary, *bin_size * sizeof (int));

    return binary;
    }

//-----------------------------------------------------------------------------
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
                                                                             \
                begunok = arg_process (str_arg, begunok, markers, cmd_id, arg, word_markers);   \
                }                                                            \
            }                                                                \
        else

int* cmd_line_process (parsed_line* code_line, int* begunok, int* markers, dict word_markers)
    {
    #include "cmd.h"
    // else
        {
        *begunok++ = unknow_cmd;
        printf ("%s: UNKNOW COMMAND USED!\n", code_line->words_ptr);
        }

    return begunok;
    }

#undef DEF_CMD(name, id, args_num, code)
//-----------------------------------------------------------------------------


#define REG_PROC(REG_STR, REG_ID)            \
    else if (string_equal (str_arg, REG_STR))\
        {                                    \
        *(begunok - 1) = cmd_id | REG_MASK;  \
        *begunok++ = REG_ID;                  \
        }
int* arg_process (char* str_arg, int* begunok, int* markers, int cmd_id, int arg, dict word_markers)
    {
    if (str_arg[0] == ':') // For markers
        {
        size_t marker_id = 0;

        const int MARK_LENGTH = 20;
        char word_mark[MARK_LENGTH] = {};

        if (sscanf (str_arg, ":%d", &marker_id))
            {
            *begunok++ = markers[marker_id];
            }
        else if (sscanf (str_arg, ":%s", &word_mark))
            {
            dict_cell* mark_in_dict = NULL;

            if (mark_in_dict = search_in_dict (word_markers, word_mark))
                {
                *begunok++ = mark_in_dict->value;
                }
            else // For pass when there is no mark in the mark_dict yet
                {
                *begunok++ = -1;
                }

            }
        }
    else if (str_arg[0] == '[') // For ram's arguments
        {
        int ram_shift = 0;

        if (sscanf (str_arg, "[%d]", &ram_shift))
            {
            *(begunok - 1) = cmd_id | RAM_MASK;
            *begunok++ = ram_shift;
            }
        else if (sscanf (str_arg, "[bx+%d]", &ram_shift))
            {

            *(begunok - 1) = cmd_id | REG_REL_MASK;
            *begunok++ = ram_shift;
            }
        else
            {
            printf ("%s is bad ram_id\n", str_arg);
            }
        }
    else if (sscanf (str_arg, "%d", &arg)) // For immediate arguments
        {
        *(begunok - 1) = cmd_id | IMM_MASK;
        *begunok++ = arg;
        }

    REG_PROC("ax", 0)

    REG_PROC("bx", 1)

    REG_PROC("cx", 2)

    REG_PROC("dx", 3)

    else // For bad arguments
        {
        printf ("[WARNING] %s is bad arg\n", str_arg);
        }

    return begunok;
    }
#undef REG_PROC


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

    puts ("Binary have been successfully written somewhere");
    }


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
#undef DEF_CMD
