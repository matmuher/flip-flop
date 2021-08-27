#ifndef BEER_EQ_H
#define BEER_EQ_H

//#define NDEBUG

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#define KOEFS_NUM 3
#define MAX_ROOTS_NUM 2
//almost ZERO: is used for comparison of floats, considering error accumulation
#define BIT_GREATER_ZERO 0.0000000000000000001
enum num_of_roots {
                  ERROR = -1,
                    ZERO = 0,
                     ONE = 1,
                     TWO = 2,
                     INF = 8
                  };

/*!
@brief Inputs equations' coefficients
@param[in] koef float array for roots with size == 2
@param[in] num_of_koefs number of coefficients
*/
void koefs_input (float *koef, int num_of_koefs);

/*!
@brief Compares 2 normal float values (considering float error accumulation), 2 NANs or 2 INFINITY
@param[in] val1 first value to compare (can be NAN, )
@param[in] val2 second value to compare
@return If values are not NAN or INIFINITY,\n
         function compares them, considering float errors' accumulation\n\n
         If both of values are NAN or INFINITY,\n
         function returns true\n\n
         If one value is NAN and other is not\n
         function returns false\n\n
         If one value is INFINITY and other is not\n
         function returns INFINITY\n\n
*/
bool are_close_values (float val1, float val2);

/*!
@brief Solves liner equation
@param[in] koef float array of coefficients with size == 2
@param[in] x_ptr float array for roots with size == 1
@return number of roots: 0 or 1
*/
int solve_line_eq (float *koef, float *x_ptr);

/*!
@brief Solves square equation
@details Solves equation a*x^2 + b*x + c = 0,\n where a!=0, b and c can be any numbers
@param[in] koef float array of coefficients with size == 3, koef[0] != 0
@param[in] x_ptr float array for roots with size == 2
@return number of roots:
ZERO if 0 roots\n
ONE if 1 root\n
TWO if 2 roots\n
INF if infinity amount of roots\n
*/
int solve_square_eq (float *koef, float *x_ptr);

/*!
@brief Solves three coefficients equation
@details Solves equation a*x^2 + b*x + c = 0,\n
         where a, b and c can be any numbers.\n In other words  it is solve_square_eq + solve_line_eq
@param[in] koef float array of coefficients with size == 3
@param[in] x_ptr float array for roots 2  elements
@return number of roots:\n
ZERO if 0 roots\n
ONE if 1 root\n
TWO if 2 roots\n
INF if infinity amount of roots\n
*/
int solve_three_koefs_eq (float *koef, float *answers_array);

/*!
@brief Prints answers of equation
@param[in] num_of_answers number of answers
@param[in] arr_of_answers float array of roots for 2  elements
*/
void eq_answer_print (int num_of_answers, float *arr_of_answers);


/*!
@brief Stop instant console closing after execution
*/
void stop_inst_console_close (void);

#endif
