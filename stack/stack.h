#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>

#define CANARY_DEFENCE
#define HASH_DEFENCE

#define NDEBUG

/*!
@brief Contains codes of errors
*/
struct error_codes
    {
    unsigned int
        overflow : 1,
        nothing_to_pop : 1,
        invalid_size : 1,
        invalid_capacity : 1,
        bad_data_ptr : 1,
        bad_capacity_ptr : 1,
        bad_stack_ptr : 1,
        stack_is_destroyed : 1,
        bad_struc_left_canary : 1,
        bad_struc_right_canary : 1,
        bad_values_left_canary : 1,
        bad_values_right_canary : 1,
        bad_hash : 1;
    };

struct debug_info
    {
    FILE* log = NULL;
    error_codes error_id = {};
    };

/*!
@brief Determines event status
*/
enum EVENT
    {
    PASSED = 0,
    HAPPENED = 1
    };

/*!
@brief Shows what event just happened
*/
struct event
    {
    unsigned int
        ctor : 1,
        push : 1,
        pop : 1,
        is_capacity_change : 1,
        dctor : 1;
    };

/*!
@brief Last-out, First-in data structure
@details You must interact with it strongly via functions,\n
         that are determined in stack.h
@params[in] data array than contains values of stack
@params[in] grosse (from german "size") number of user's elements in stack
@params[in] capacity maximum grosse for this moment
@params[in] capacity_ptr ptr to dynamically allocated memory for user's values
@params[in] debug_info structure with errors and log file
@params[in] stack_hash
*/
struct stack
    {
    #ifdef CANARY_DEFENCE
    double left_struct_canary;
    #endif

    int* data;
    int* capacity_ptr;

    debug_info stk_debug;
    event stk_event;

    #ifdef HASH_DEFENCE
    int stack_hash;
    #endif

    int grosse;
    int capacity;

    #ifdef CANARY_DEFENCE
    double right_struct_canary;
    #endif
    };

const int POISON = 0xDEADDEAD; ///< Value that marks non-valid elements

const int DELIMITER_LEN = 30;

const int TRY_LOG = 1; ///< To check log file pointer with IsBadReadPtr; kostyl?

// Stack functions

/*!
@brief Creates stack data structure
*/
void stack_ctor (stack* stk, FILE* log);

/*!
@brief Check is stk a valid stack
*/
void stack_verify (stack* stk);

/*!
@brief Calculates hash for stack
*/
int get_stack_hash (stack* stk);

/*!
@brief Prints information about stack and current operation in log file
*/
void stack_dump (stack* stk);

/*!
@brief Calculates what capacity should have stk with its grosse
*/
int find_stock (int cur_size);

/*!
@brief Adds element to stk
@details stk dynamically expands due to find_stock function:\n
        [grosse] [capacity]\n
            1       2\n
            2       4\n
            3       4\n
            4       4\n
            5       8\n
            6       8\n
*/
void stack_push (stack* stk, int value);


/*!
@brief Adds element to stk
@details stk dynamically decreases due to find_stock function with shift
         to prevent frequent memory reallocation:
            [grosse] [capacity]\n
                6       8\n
                5       8\n
                4       8\n
                3       8\n
                2       4\n
                1       1\n
*/
int stack_pop (stack* stk);

/*!
@brief Poisons stk and free allocated memory
*/
void stack_dtor (stack* stk);

// Support functions

/*!
@brief Chistim hvosty
@details Poison realloc's trash
*/
void clear_tales (stack* stk, size_t start_element);

void update_capacity (stack* stk);

int stack_realloc_default (stack* stk);

int stack_realloc_with_canaries (stack* stk);


#endif // STACK_H_INCLUDED
