#include <stdio.h>
#include "processor.h"


int main ()
    {
    size_t bin_size = 0;


    // DISASM AND BINARY OPEN
    FILE* bin = fopen ("binary_my_binary", "rb");
    FILE* disasm = fopen ("disasm.txt","w");


    // READ SERVICE STRUCTURE
    bin_info ma_bin = {0, 0, 0};
    fread (&ma_bin, sizeof (char), sizeof (ma_bin), bin);


    // VERIFY SERVICE STRUCTURE
    assert (ma_bin.signature == MY_SIGN);
    assert (ma_bin.version == version);


    // READ ASSEMBLED CODE
    bin_size = ma_bin.bin_size;
    int* cooking_list = (int*) calloc (bin_size, sizeof (int));
    fread (cooking_list, sizeof (int), bin_size, bin);


    // CODE RECOGNITION
    recogniser (bin_size, cooking_list, disasm);


    // DISASM AND BINARY CLOSE
    fclose (disasm);
    fclose (bin);
    }
