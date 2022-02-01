#ifndef BEER_EQ_H
#define BEER_EQ_H

#define NDEBUG

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

const float FLOAT_ZERO = 1e-20; ///< 1.0 + FLOAT_ZERO == 1.0 is true;
const int KOEFS_NUM  = 3; ///< number of equation's coefficients
const int SQUARE_MAX_ROOTS_NUM = 2; ///< max number of equation's root

enum num_of_roots
    {
    ROOT_ERROR = -2, ///< something went wrong during roots calculation
    INFINITY_ROOTS = -1, ///< equation has infinity roots
    ZERO_ROOTS = 0, ///< equation has no roots
    ONE_ROOT = 1, ///< equation has one root
    TWO_ROOTS = 2, ///< equation has two roots
    };

/*!
@brief Clear input buffer and print "Try again", if status is equal to 0
@param[in] status if it is equal to 0 function works out, else it does not
*/
void wrong_input_proccess (int status);

/*!
@brief Inputs equations' coefficients
@param[in] coefs float array where the coefficients of equation will be placed,\n
           array must include at least num_of_coefs elements\n
@param[in] num_of_coefs number of coefficients
@param[out] coefs float array that stores coefficients\n
                 coefs[0] = first input coefficient\n
                 coefs[1] = second input coefficient\n
                 etc
*/
void coefs_input (float *coefs, int num_of_coefs);

/*!
@brief Compares 2 real values considering float error accumulation
@details Calculates whether absolute value of input values difference
         is less than machine null
@param[in] val1 first value to compare (can be real number, NAN, INFINITY)
@param[in] val2 second value to compare (can be real number, NAN, INFINITY)
@return If values are not NAN or INFINITY,\n
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
@brief Solves linear equation
@details Solves linear equation of this form:\n    A*x + B = 0\n
         where A and B can be any real numbers\n
@param[in] coefs float array that stores 2 coefficients:\n
                coefs[0] = A\n
                coefs[1] = B\n
@param[in] x_ptr float array where the roots of equation will be placed,\n
                 array must include at least (SQUARE_MAX_ROOTS_NUM - 1) elements\n
@param[out] x_ptr float array where the roots of equation were be placed
@return enum num_of_roots
*/
int solve_line_eq (float *coefs, float *x_ptr);

/*!
@brief Solves square equation
@details Solves equation of this form:\n    A*x^2 + B*x + C = 0\n
         where A is not equal to 0, B and C can be any real numbers\n
@param[in] coefs float array that stores 3 coefficients:\n
                coefs[0] = A\n
                coefs[1] = B\n
                coefs[2] = C\n
@param[in] x_ptr float array where the roots of equation will be placed,\n
                 array must include at least SQUARE_MAX_ROOTS_NUM elements\n
@param[out] x_ptr float array where the roots of equation were be placed
@return enum num_of_roots
*/
int solve_square_eq (float *coefs, float *x_ptr);

/*!
@brief Solves three coefficients equation
@details Solves equation of this form:\n    A*x^2 + B*x + C = 0,\n
         where A, B and C can be any real numbers\n
@param[in] coefs float array that stores 3 coefficients:\n
                coefs[0] = A\n
                coefs[1] = B\n
                coefs[2] = C\n
@param[in] x_ptr float array where the roots of equation will be placed,\n
                 array must include at least SQUARE_MAX_ROOTS_NUM elements\n
@return enum number_of_roots
*/
int solve_three_coefs_eq (float *coefs, float *answers_array);

/*!
@brief Prints answers of equation
@param[in] num_of_answers number of answers to print
@param[in] arr_of_answers float array of roots,
                          including at least num_of_answers elements\n
                          (in case if num_of_answers > 0)\n
*/
void eq_answer_print (int num_of_answers, float *arr_of_answers);


/*!
@brief Stop instant console closing after execution
@details scanf () that is waiting for string input
*/
void stop_inst_console_close (void);

#endif
