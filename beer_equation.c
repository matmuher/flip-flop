#include <stdio.h>
#include <math.h>
#define NUM_OF_KOEFS 3
#define ROOTS_LIMIT 2

int line_eq(float *koef, float *x_ptr);                   // returns 0, if no roots;
                                                          // returns 1, if there is 1 root;
                                                          // returns 2, if there are 2 roots
int square_eq (float *koef, float *x_ptr);                // returns 8, if there are infinity roots (only line_eq)

void eq_answer_print (int ans_type, float *arr_of_answers);

void koefs_input (float *koef, int num_of_koefs);




int main ()
{
    float koef[NUM_OF_KOEFS] = {0, 0, 0};
    float answers_array[ROOTS_LIMIT] = {0, 0};
    int answer_type = -1;

    puts ("The square equation looks like this:\n"
            "(koef_0)*x^2+(koef_1)*x+(koef_2)=0\n");

    koefs_input (koef, 3);                                      //INPUT

    if (koef[0] == 0)                                           //LINER
        {
            float line_koef[2] = {koef[1], koef[2]};

            answer_type = line_eq (line_koef, answers_array);
        }
    else                                                        //SQUARE
        {
            answer_type = square_eq (koef, answers_array);
        }

    eq_answer_print (answer_type, answers_array);               //OUTPUT

    return 0;
}




void koefs_input (float *koef, int num_of_koefs)
{
    char Junk = ' ';

    for (int num_of_koef = 0; num_of_koef < num_of_koefs; num_of_koef++)
        {
            printf ("\nPlease, enter value of koef_%d\n"
                    "koef_%d = ", num_of_koef, num_of_koef);

            int status = scanf ("%f", &koef[num_of_koef]);

            if (status == 0)
                {
                    puts ("\nWrong input. Try again (f.e. 8.5 or 9 or 3.14 or something like this)");

                    scanf ("%c", &Junk);

                    num_of_koef--;
                }
        }
}


int line_eq (float *koef, float *x_ptr)                       // LINER EQUATION SOLVER: koef_0*x + koef_1 = 0
{
    if (koef[0] == 0 && koef[1] != 0)                         // f.e.: x*0 - 5 = 0
        {
            return 0;
        }
    else if (koef[0] == 0 && koef[1] == 0)                    // x*0 + 0 = 0
        {
            return 8;
        }
    else                                                      // x_koef*x + free_koef = 0,
        {
            float x = -koef[1]/koef[0];                       // x = -free_koef/x_koef

            *x_ptr = x;
                                                              // transfer x value to x_ptr data adress
            return 1;
        }
}


void eq_answer_print (int ans_type, float *arr_of_answers)    // EQUATION ANSWER PRINTER
{
    if (ans_type == 1 || ans_type == 2)
        {
            int num_of_answers = ans_type;

            puts ("\n************"
                  "\nAnswers:");

            for (int num_of_answer = 0; num_of_answer < num_of_answers; num_of_answer++)
                {
                        printf ("x = %f\n", arr_of_answers[num_of_answer]);
                }
        }
    else if (ans_type == 0)
        {
            puts ("\n************"
                  "\nNo roots");
        }
    else if (ans_type == 8)
        {
             puts ("\n************"
                   "\nInfinity roots");
        }
}


int square_eq (float *koef, float *x_ptr)                        //SQUARE EQUATION SOLVER
{
    float multip_2_and_0_koefs = koef[2]*koef[0];

    if (koef[1] == 0 && multip_2_and_0_koefs > 0)                // f.e.: 5*x^2 + 0*x + 5 = 0
        {                                                        //       x = sqrt(-5/5) -> no roots from negative numbers
            return 0;
        }
    else if (koef[1] == 0 && multip_2_and_0_koefs <= 0)
        {
            if (multip_2_and_0_koefs == 0)                       // f.e.: 5*x^2 + 0*x + 0 = 0
                                                                 //       x = 0
                {
                    *x_ptr = 0;

                    return 1;
                }
            else                                                 // f.e.: 5*x^2 + 0*x - 5 = 0
                {                                                //       x = +sqrt(1)
                    *x_ptr = sqrt (-koef[2]/koef[0]);            //       x = -sqrt(1) -> 2 roots
                    *(x_ptr+1) = -(*x_ptr);

                    return 2;
                }
        }

    float D = koef[1]*koef[1] - 4*koef[0]*koef[2];               //la formule de Discriminante

    if (D == 0)
        {
            float x = -koef[1]/( 2*koef[0] );                    // sqrt_D == 0

            *x_ptr = x;

            return 1;
        }
    if (D < 0)
        {
            return 0;
        }
    if (D > 0)
        {
            float sqrt_D = sqrt(D);
            float x1 = ( -koef[1]+sqrt_D )/( 2*koef[0] );
            float x2 = ( -koef[1]-sqrt_D )/( 2*koef[0] );

            *x_ptr = x1;
            *(x_ptr+1) = x2;

            return 2;
        }
}
