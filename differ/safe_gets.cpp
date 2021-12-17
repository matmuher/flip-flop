#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


static void clear_stdin_buffer ()
    {
    while (getchar () != '\n') {continue;}
    }


char* safe_gets (char* write_to_buffer, size_t buffer_size, FILE* file_stream)
    {
    assert (buffer_size > 1); //<-- There always should be place for '\0'
    assert (write_to_buffer);

    char* begunok = fgets (write_to_buffer, buffer_size, file_stream);

    while (*begunok != '\n' && *begunok != '\0') {begunok++;}

    if (*begunok == '\n')
        {
        *begunok = '\0';
        }
    else
        {
        if (file_stream == stdin)
            {
            clear_stdin_buffer ();
            }
        }

    return write_to_buffer;
    }
