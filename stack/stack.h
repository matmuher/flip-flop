#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/*!
@brief Contains codes of errors
*/
struct
    {
    unsigned int
        overflow : 1,
        nothing_to_pop : 1,
        invalid_size : 1,
        invalid_capacity : 1,
        bad_data_ptr : 1,
        bad_stack_ptr : 1;
    } ERROR_ID = {};

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
struct
    {
    unsigned int
        push : 1,
        pop : 1,
        is_capacity_change : 1,
        dctor : 1;
    } event = {};

/*!
@brief Last-out, First-in data structure
@params[in] data array than contains values of stack
@params[in] grosse (from german "size") number of user's elements in stack
@params[in] capacity maximum grosse for this moment
*/
struct stack
    {
    int* data;
    int grosse;
    int capacity;
    };

const int POISON = 0xDEADDEAD; ///< Value that marks non-valid elements

const int DELIMITER_LEN = 30;

const int TRY_LOG = 1; ///< To check log file pointer with IsBadReadPtr; kostyl?

// Stack functions

/*!
@brief Creates stack data structure
*/
void stack_ctor (stack* stk);

/*!
@brief Check is stk a valid stack
*/
void stack_verify (stack* stk, FILE* log);

/*!
@brief Prints information about stack and current operation in log file
*/
void stack_dump (stack* stk, FILE* log);

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
void stack_push (stack* stk, int value, FILE* log);


/*!
@brief Adds element to stk
@details stk dynamically expands due to find_stock function with shift
         to prevent frequent memory reallocation:
            [grosse] [capacity]\n
                6       8\n
                5       8\n
                4       8\n
                3       8\n
                2       4\n
                1       1\n
*/
int stack_pop (stack* stk, FILE* log);

/*!
@brief Poisons stk and free allocated memory
*/
void stack_dtor (stack* stk, FILE* log);

#endif // STACK_H_INCLUDED
