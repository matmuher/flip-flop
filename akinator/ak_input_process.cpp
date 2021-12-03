#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ak.h"


static void clear_stdin_buffer ()
    {
    while (getchar () != '\n') {continue;}
    }


char* safe_gets (char* write_to_buffer, size_t buffer_size)
    {
    assert (buffer_size > 1); //<-- There always should be place for '\0'
    assert (write_to_buffer);

    char* begunok = fgets (write_to_buffer, buffer_size, stdin);

    while (*begunok != '\n' && *begunok != '\0') {begunok++;}
    if (*begunok == '\n')
        {
        *begunok = '\0';
        }
    else
        clear_stdin_buffer ();


    return write_to_buffer;
    }


int get_answer (void)
    {
    const size_t MAX_ANSWER_LENGTH = 25;

    char user_answer[MAX_ANSWER_LENGTH] = {};
    safe_gets (user_answer, MAX_ANSWER_LENGTH);

    const int ATTEMPTS_ALLOWED = 5;

    for (int attempt_id = 0; attempt_id < ATTEMPTS_ALLOWED; attempt_id++)
        {
        if (strcmp (user_answer, "No") == 0)
            {
            return NO;
            }
        else if (strcmp (user_answer, "Yes") == 0)
            {
            return YES;
            }
        else
            {
            puts ("Incorrect vvod, kojanii meshok. Please, use Yes/No and try again");
            safe_gets (user_answer, MAX_ANSWER_LENGTH);
            }
        }

    puts ("Sorry, you're ne obuchaem\n");
    exit (777);
    }
