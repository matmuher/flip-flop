#ifndef DIFF_DSL_H_INCLUDED
#define DIFF_DSL_H_INCLUDED


#include "differ.h"


// (f_x + g_x)' = f_x' + g_x'
node* diff_sum (node* cur_node);

// (f_x * g_x)' = f_x' * g_x + g_x' * f_x
node* diff_mlt (node* cur_node);

// ((f_x) ^ A)'= A * (f_x) ^ (A - 1) * (f_x)'
node* diff_pow (node* cur_node);

// (x / y) = (x * y ^ (-1))
node* transform_division (node* cur_node);

// (sin_f_x)' = cos * (f_x)'
node* diff_sin (node* cur_node);

// (cos_f_x)' = (-1) * sin (f_x) * (f_x)'
node* diff_cos (node* cur_node);


#endif // DIFF_DSL_H_INCLUDED
