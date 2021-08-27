#include "beer_equation.h"

int main ()
    {
    float koef[KOEFS_NUM] = {NAN, NAN, NAN};
    float answers_array[MAX_ROOTS_NUM] = {NAN, NAN};
    int num_of_answers = -1;

    puts ("The square equation looks like this:\n"
          "(koef_0)*x^2+(koef_1)*x+(koef_2)=0\n");

    koefs_input (koef, KOEFS_NUM); //INPUT

    num_of_answers = solve_three_koefs_eq (koef, answers_array); //SOLUTION

    eq_answer_print (num_of_answers, answers_array); //OUTPUT

    stop_inst_console_close ();

    return 0;
    }
