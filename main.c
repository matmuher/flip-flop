#include "beer_equation.h"

int main ()
{
float koef[NUM_OF_KOEFS] = {NAN, NAN, NAN};
float answers_array[ROOTS_LIMIT] = {NAN, NAN};
int num_of_answers = -1;

puts ("The square equation looks like this:\n"
      "(koef_0)*x^2+(koef_1)*x+(koef_2)=0\n");

koefs_input (koef, NUM_OF_KOEFS); //INPUT    //  *sizeof?

num_of_answers = three_koefs_eq (koef, answers_array);

eq_answer_print (num_of_answers, answers_array); //OUTPUT

// to avoid instant close of console when call .exe
scanf ("%*s");

return 0;
}
