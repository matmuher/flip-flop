#include "beer_equation.h"


bool close_to_zero (float val)
{
assert (isfinite (val));
return (fabs (val) < BIT_GREATER_ZERO)? true : false;
}


void wrong_input_proccess (int status)
{
assert (status >= 0);

if (status == 0)
    {
    puts ( "\nWrong input. Try again (f.e. 8.5 or 9 or 3.14 or something like this)" );

    scanf ( "%*s" );
    }
}


void koefs_input (float *koef, int num_of_koefs)
{
assert (koef != NULL);
assert (num_of_koefs > 0);

void wrong_input_proccess (int status);
    
for (int koef_id = 0; koef_id < num_of_koefs; koef_id++)
    {
    int status = 0;

    while (status != 1)
        {
        printf ("\nPlease, enter value of koef_%d\n"
                "koef_%d = ", koef_id, koef_id);
        
        status = scanf ("%f", &koef[koef_id]);
        
        puts("");

        wrong_input_proccess (status);
        }
    }
}


// LINER EQUATION SOLVER: koef[1]*x + koef[2] = 0
int line_eq (float *koef, float *x_ptr)
{
assert (koef != NULL);
assert (x_ptr != NULL);
assert (isfinite (koef[0]));
assert (isfinite (koef[1]));

if (close_to_zero (koef[0]) && !close_to_zero (koef[1])) // f.e.: x*0 - 5 = 0
    {
    return ZERO;
    }
else if (close_to_zero (koef[0]) && close_to_zero (koef[1])) // f.e.: x*0 + 0 = 0
    {
    return INF;
    }

// koef[1]*x + koef[2] = 0
// x = -koef[2]/koef[1]
x_ptr[0] = -koef[1]/koef[0];

return ONE;
}


int square_eq_optimizer (float *koef, float *x_ptr)
{
float multip_2_and_0_koefs = koef[2]*koef[0];

if (close_to_zero (koef[1]) && multip_2_and_0_koefs > 0) // f.e.: 5*x^2 + 0*x + 5 = 0
    {
    return ZERO; // e.continue: x = sqrt(-5/5) -> no roots from negative numbers
    }
else if (close_to_zero (koef[1]) && multip_2_and_0_koefs <= 0)
    {
    if (close_to_zero (multip_2_and_0_koefs)) // f.e.: 5*x^2 + 0*x + 0 = 0
        {
        x_ptr[0] = 0; // e.continue: x = 0
        x_ptr[1] = 0;

        return TWO;
        }
    // f.e.: 5*x^2 + 0*x - 5 = 0
    // e.continue: x = -sqrt(1) and x = +sqrt(1) -> 2 roots
    x_ptr[0] = sqrt (-koef[2]/koef[0]);
    x_ptr[1] = -(x_ptr[0]);

    return TWO;
    }
}


// SOLVER VIA DISCRIMINANT
int via_D_solver (float *koef, float *x_ptr)
{
assert (koef != NULL);
assert (x_ptr != NULL);
assert (isfinite (koef[0]));
assert (isfinite (koef[1]));
assert (isfinite (koef[2]));

float D = koef[1]*koef[1] - 4*koef[0]*koef[2]; //la formule de Discriminante

if (close_to_zero (D))
    {
    x_ptr[0] = (-koef[1])/(2*koef[0]); // sqrt_D == 0
    x_ptr[1] = x_ptr[0];

    return TWO;
    }
if (D < 0)
    {
    return ZERO;
    }
if (D > 0)
    {
    float sqrt_D = sqrt(D);

    x_ptr[0] = (-koef[1] + sqrt_D)/(2*koef[0]);
    x_ptr[1] = (-koef[1] - sqrt_D)/(2*koef[0]);

    return TWO;
    }

return ERROR;//?
}


//SQUARE EQUATION SOLVER: koef[0]*x^2 + koef[1]*x + koef[2] = 0
int square_eq (float *koef, float *x_ptr)
{
assert (koef != NULL);
assert (x_ptr != NULL);
assert (isfinite (koef[0]));
assert (isfinite (koef[1]));
assert (isfinite (koef[2]));

int via_D_solver (float *koef, float *x_ptr);
int square_eq_optimizer (float *koef, float *x_ptr);

if (close_to_zero(koef[1]))
    {
    return square_eq_optimizer (koef, x_ptr);
    }

return via_D_solver (koef, x_ptr);
}


// 3 KOEFS EQUATION SOLVER
int three_koefs_eq (float *koef, float *answers_array)
{
assert (koef != NULL);
assert (answers_array != NULL);
assert (isfinite (koef[0]));
assert (isfinite (koef[1]));
assert (isfinite (koef[2]));

int num_of_answers = -1;
    
if (close_to_zero(koef[0])) //LINER
    {
    float line_koef[2] = {koef[1], koef[2]};

    num_of_answers = line_eq (line_koef, answers_array);
    }
else //SQUARE
    {
    num_of_answers = square_eq (koef, answers_array);
    }
    
return num_of_answers;
}


// EQUATION ANSWER PRINTER
void eq_answer_print (int num_of_answers, float *arr_of_answers)
{
assert (num_of_answers >= 0);
assert (arr_of_answers != NULL);

char delimiter[] = "\n************";

if (num_of_answers == ONE || num_of_answers == TWO)
    {
    printf ("%s", delimiter);
    puts ("\nAnswers:");

    for (int answer_id = 0; answer_id < num_of_answers; answer_id++)
        {
        printf ("x = %f\n", arr_of_answers[answer_id]);
        }
    }
else if (num_of_answers == ZERO)
    {
    printf ("%s", delimiter);
    puts ("\nNo roots");
    }
else if (num_of_answers == INF)
    {
    printf ("%s", delimiter);
    puts ("\nInfinity roots");
    }
else
    {
    printf ("%s", delimiter);
    puts ("\nWrong number of answers");
    }
}
