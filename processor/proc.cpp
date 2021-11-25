#include <stdio.h>
#include "processor.h"


int main ()
    {
    size_t bin_size = 0;

    // *имена файлов через args cmd

    // LOGS AND BINARY OPEN
    FILE* bin = fopen ("binary_my_binary", "rb");
    FILE* log = fopen ("stack_log.txt", "w");
    FILE* prc_log = fopen ("prc_log.txt", "w");


    // READ SERVICE STRUCTURE
    bin_info ma_bin = {0, 0, 0};
    if (!fread (&ma_bin, sizeof (char), sizeof (ma_bin), bin))
        {
        puts ("[ERROR] Bad structure reading");
        }


    // VERIFY SERVICE STRUCTURE
    assert (ma_bin.signature == MY_SIGN);
    assert (ma_bin.version == version);


    // READ ASSEMBLED CODE
    bin_size = ma_bin.bin_size;
    int* cooking_list = (int*) calloc (bin_size, sizeof (int));
    if (!fread (cooking_list, sizeof (int), bin_size, bin))
        {
        puts ("[ERROR] Bad cmds' binary reading");
        }


    // PROC CONSTRUCT
    proc prc = {};
    proc_ctor (&prc, bin_size, cooking_list, log, prc_log);


    // CODE EXECUTE
    doer (&prc);


    // LOGS AND BINARY CLOSE
    fclose (bin);
    fclose (log);
    fclose (prc_log);

    stop_inst_console_close ();
    }
