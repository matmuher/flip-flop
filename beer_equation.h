#ifndef BEER_EQ_H
#define BEER_EQ_H

#include <stdio.h>
#include <math.h>
#include <assert.h>
#define NUM_OF_KOEFS 3
#define ROOTS_LIMIT 2
#define BIT_GREATER_ZERO 0.0000000000000000001
enum num_of_roots {ERROR = -1, ZERO = 0, ONE = 1, TWO = 2, INF = 8};

/*!
@brief Compares float value with zero
@param[in] val value that is compared with zero
@return 1 if value is close to 0\n
0 if value is not close to\n
*/
bool close_to_zero (float  val);

/*!
@brief Inputs equations' coefficients
@param[in] koef float array for roots with size == 2
@param[in] num_of_koefs number of coefficients
*/
void koefs_input (float *koef, int num_of_koefs);

/*!
@brief Solves liner equation
@param[in] koef float array of coefficients with size == 2
@param[in] x_ptr float array for roots with size == 1
@return number of roots: 0 or 1
*/
int line_eq (float *koef, float *x_ptr);

/*!
@brief Solves square equation
@param[in] koef float array of coefficients with size == 3
@param[in] x_ptr float array for roots with size == 2
@return number of roots:
0 if 0 roots\n
1 if 1 root\n
2 if 2 roots\n
8 if infinity amount of roots\n
*/
int square_eq (float *koef, float *x_ptr);

/*!
@brief Solves three coefficients equation
@param[in] koef float array of coefficients with size == 3
@param[in] x_ptr float array for roots 2  elements
@return number of roots:\n
0 if 0 roots\n
1 if 1 root\n
2 if 2 roots\n
8 if infinity amount of roots\n
*/
int three_koefs_eq (float *koef, float *answers_array);

/*!
@brief Prints answers of equation
@param[in] num_of_answers number of answers
@param[in] arr_of_answers float array of roots for 2  elements
*/
void eq_answer_print (int num_of_answers, float *arr_of_answers);

 #endif
