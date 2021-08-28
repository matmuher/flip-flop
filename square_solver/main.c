#include "beer_equation.h"

//SOLVES 3 COEFS EQUATION
int main ()
    {
    float coefs[KOEFS_NUM] = {NAN, NAN, NAN};
    float answers_array[SQUARE_MAX_ROOTS_NUM] = {NAN, NAN};
    int num_of_answers = -1;

    puts ("The square equation looks like this:\n"
          "(coef_0)*x^2+(coef_1)*x+(coef_2)=0\n");

    coefs_input (coefs, KOEFS_NUM); //INPUT

    num_of_answers = solve_three_coefs_eq (coefs, answers_array); //SOLUTION

    eq_answer_print (num_of_answers, answers_array); //OUTPUT

    stop_inst_console_close ();

    return 0;
    }
