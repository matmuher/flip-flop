#include "string_funk.h"


int string_compare (char* left_start, char* left_end,
                    char* right_start, char* right_end, const int shift)
    {
    // make function for pointer several pointers null check
    // with variable num of args
    assert (left_start != NULL);
    assert (left_end != NULL);
    assert (right_start != NULL);
    assert (right_end != NULL);
    assert (shift * shift == 1);
    if (shift == -1)
        {
        // make function assert + print
        assert ("If shift = 1 must be" && left_start >= left_end);
        assert ("If shift = 1 must be" && right_start >= right_end);
        }
    else if (shift == 1)
        {
        assert ("If shift = -1 must be" && left_start <= left_end);
        assert ("If shift = -1 must be" && right_start <= right_end);
        }


    while (*left_start == *right_start)
        {
        if (left_start == left_end || right_start == right_end)
            {
            break;
            }

        left_start = left_start + shift;
        right_start = right_start + shift;
        }

    return *left_start - *right_start;
    }

int say (const char str[])
    {
    assert (str != NULL);

    int symbol_id = 0;

    while (str[symbol_id] != '\0')
        {
        putchar (str[symbol_id++]);
        }
    putchar ('\n');

    return symbol_id;
    }


char* find_char (const char str[], char chr)
    {
    assert (str != NULL);

    while (*str != chr)
        {
        if (*str == '\0')
            {
            return NULL;
            }
        str++;
        }

    return (char*) str;
    }


char* fget_string (char str[], int n, FILE* file_pointer)
    {
    assert (str != NULL);
    assert (file_pointer != NULL);
    // at lest n = 1 otherwise there is no sense
    // to use it
    assert (n > 0);

    n = n + 1; // considering '\0' character in the end

    char* str_beggining = str;

    for (int char_id = 0; char_id < n; char_id++)
        {
        if ((*str  = getc (file_pointer)) == '\n')
            {
            break;
            }
        else if ((*str == EOF) || (*str == '\0'))
            {
            break;
            }

        str++;
        }
    *str = '\0';

    if (str == str_beggining)
        {
        return NULL;
        }
    else if (ferror (file_pointer))
        {
        puts ("[File reading error]");
        return NULL;
        }

    return str_beggining;
    }


char* string_dup (char str[])
    {
    assert (str != NULL);

    int str_size = string_len (str) + 1;
    char *dup_pointer = (char*)(calloc (str_size, sizeof (char)));

    if (dup_pointer != NULL)
        {
        string_copy (dup_pointer, str);
        }

    return dup_pointer;
    }


char* string_copy (char destination[], char source[])
    {
    assert (destination != NULL);
    assert (source != NULL);
    assert (destination != source);

    char* destination_begin = destination;

    while (*destination++ = *source++) {;}

    return destination_begin;
    }


int string_len (const char str[])
    {
    assert (str != NULL);

    int len = 0;

    len = (int) (find_char (str, '\0') - str);

    return len;
    }


char* string_stick (char destination[], char source[])
    {
    assert (destination != NULL);
    assert (source != NULL);
    assert (destination != source);

    char* destination_end = destination + string_len (destination) - 1;

    string_copy (destination_end, source);

    return destination;
    }

void print_line (const char symbol, size_t line_len)
    {
    assert (line_len > 0);

    fprint_line (stdout, symbol, line_len);
    }

void fprint_line (FILE* file_ptr, const char symbol, size_t line_len)
    {
    assert (line_len > 0);

    for (size_t sym_id = 0; sym_id < line_len; sym_id++)
        {
        fputc (symbol, file_ptr);
        }
    fputc ('\n', file_ptr);
    }
