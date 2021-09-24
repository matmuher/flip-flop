#include "string_funk.h"


int string_cmp (char left_str[], char right_str[])
    {
    assert (left_str != NULL);
    assert (right_str != NULL);

    while ((*left_str == *right_str) &&
           (*left_str != '\0') && (*right_str != '\0'))
        {
        left_str++;
        right_str++;
        }

    return *left_str - *right_str;
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

    while (*str != '\0')
        {
        if (*str == chr)
            {
            return (char*) str;
            }

        str++;
        }

    // After "while" execution, str will contain
    // adress of '\0' i.e. end of string
    if (chr == '\0')
        {
        return (char*) str;
        }

    // There is no symbol in line:(
    return NULL;
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

    while (LOOP)
        {
        if (( (*str  = getc (file_pointer)) == '\n' ))
            {
            break;
            }
        else if ((*str != EOF) && *str)
            {
            break;
            }
        else if (!--n)
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

    int len = 0;

    len = (int)(find_char (str, '\0') - str);

    return len;
    }


char* string_stick (char destination[], char source[])
    {
    assert (destination != NULL);
    assert (source != NULL);

    char* destination_end = destination + string_len (destination) - 1;

    string_copy (destination_end, source);

    return destination;
    }
