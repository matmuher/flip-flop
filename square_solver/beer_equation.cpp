#include "beer_equation.h"

bool are_close_values (float val1, float val2)
    {
    if (isnan (val1) && isnan (val2) || isinf (val1) && isinf (val2))
        {
        return true;
        }
    if (!isnan (val1) && isnan (val2) || isnan (val1) && !isnan (val2) ||
        !isinf (val1) && isinf (val2) || isinf (val1) && !isinf (val2))
        {
        return false;
        }

    return (fabs (val1-val2) < FLOAT_ZERO) ? true : false;
    }


void wrong_input_proccess (int status)
    {
    assert (status >= 0);

    if (status == 0)
        {
        puts ("\nWrong input. Try again, please");

        scanf ("%*s");
        }
    }


void coefs_input (float *coefs, int num_of_coefs)
    {
    assert (coefs != NULL);
    assert (num_of_coefs > 0);

    for (int coef_id = 0; coef_id < num_of_coefs; coef_id++)
        {
        int status = 0;

        while (status != 1)
            {
            printf ("\nPlease, enter value of coefs_%d\n"
                    "coefs_%d = ", coef_id, coef_id);

            status = scanf ("%f", &coefs[coef_id]);

            wrong_input_proccess (status);
            }
        }
    }


// LINEAR EQUATION SOLVER: coefs[1]*x + coefs[2] = 0
int solve_line_eq (float *coefs, float *x_ptr)
    {
    assert (coefs != NULL);
    assert (x_ptr != NULL);
    assert (isfinite (coefs[0]));
    assert (isfinite (coefs[1]));

    if (are_close_values (coefs[0], 0) && !are_close_values (coefs[1], 0)) // f.e.: x*0 - 5 = 0
        {
        return ZERO_ROOTS;
        }
    else if (are_close_values (coefs[0], 0) && are_close_values (coefs[1], 0)) // f.e.: x*0 + 0 = 0
        {
        return INFINITY_ROOTS;
        }

    // coefs[1]*x + coefs[2] = 0
    // x = -coefs[2]/coefs[1]
    x_ptr[0] = -coefs[1] / coefs[0];

    return ONE_ROOT;
    }

/*!
@brief Optimize solve_square_eq ()
@details parameters are same as solve_square_eq () has
*/
static int square_eq_optimizer (float *coefs, float *x_ptr)
    {
    assert (coefs != NULL);
    assert (x_ptr != NULL);
    assert (isfinite (coefs[0]));
    assert (isfinite (coefs[1]));
    assert (isfinite (coefs[2]));

    if (are_close_values (coefs[1], 0)) // coefs_0 * x^2 + coefs_1 * 0 + coefs_2 = 0
        {
        float help_coefs = -coefs[2] / coefs[0];

        if (help_coefs < 0) // x = sqrt (help_coefs) -> sqrt (negative)
            {
            return ZERO_ROOTS; // no roots in real numbers
            }
        else
            {
            x_ptr[0] = sqrt (help_coefs); // x = sqrt (0 or positive)
            x_ptr[1] = -x_ptr[0];

            return TWO_ROOTS;
            }
        }
    else if (are_close_values (coefs[2], 0))  // x * (coefs_0 * x - coefs_1) = 0
        {
        x_ptr[1] = 0;

        float line_coefs[] = {coefs[0], coefs[1]};

        solve_line_eq (line_coefs, x_ptr); // will place root in x_ptr[0]

        return TWO_ROOTS;
        }

    return ROOT_ERROR;
    }


/*!
@brief Solves square equation using Discriminant
@details parameters are same as solve_square_eq () has
*/
static int solve_using_D (float *coefs, float *x_ptr)
    {
    assert (coefs != NULL);
    assert (x_ptr != NULL);
    assert (isfinite (coefs[0]));
    assert (isfinite (coefs[1]));
    assert (isfinite (coefs[2]));

    float D = coefs[1]*coefs[1] - 4*coefs[0]*coefs[2]; //la formule de Discriminante
    float double_coefs_0 = 2 * coefs[0];
    float minus_coefs_1 = -coefs[1];


    if (are_close_values (D, 0))
        {
        x_ptr[0] = minus_coefs_1 / double_coefs_0; // sqrt_D == 0
        x_ptr[1] = x_ptr[0];

        return TWO_ROOTS;
        }
    else if (D < -FLOAT_ZERO)
        {
        return ZERO_ROOTS;
        }

    float sqrt_D = sqrt(D);

    x_ptr[0] = (minus_coefs_1 + sqrt_D) / double_coefs_0;
    x_ptr[1] = (minus_coefs_1 - sqrt_D) / double_coefs_0;

    return TWO_ROOTS;
    }


//SQUARE EQUATION SOLVER: coefs[0]*x^2 + coefs[1]*x + coefs[2] = 0
int solve_square_eq (float *coefs, float *x_ptr)
    {
    assert (coefs != NULL);
    assert (x_ptr != NULL);
    assert (isfinite (coefs[0]));
    assert (isfinite (coefs[1]));
    assert (isfinite (coefs[2]));

    if (are_close_values (coefs[1], 0) || are_close_values (coefs[2], 0))
        {
        return square_eq_optimizer (coefs, x_ptr);
        }

    return solve_using_D (coefs, x_ptr);
    }


// 3 KOEFS EQUATION SOLVER
int solve_three_coefs_eq (float *coefs, float *answers_array)
    {
    assert (coefs != NULL);
    assert (answers_array != NULL);
    assert (isfinite (coefs[0]));
    assert (isfinite (coefs[1]));
    assert (isfinite (coefs[2]));

    int num_of_answers = -1;

    if (are_close_values (coefs[0], 0)) //LINEAR
        {
        float line_coefs[2] = {coefs[1], coefs[2]};

        num_of_answers = solve_line_eq (line_coefs, answers_array);
        }
    else //SQUARE
        {
        num_of_answers = solve_square_eq (coefs, answers_array);
        }

    return num_of_answers;
    }


// EQUATION ANSWER PRINTER
void eq_answer_print (int num_of_answers, float *arr_of_answers)
    {
    assert (num_of_answers >= 0);
    assert (arr_of_answers != NULL);

    char delimiter[] = "\n************";

    //switch (num_of_answers)
    switch (num_of_answers)
        {
        case ONE_ROOT:
        case TWO_ROOTS:
            {
            printf ("%s\nAnswers:\n", delimiter);

            for (int answer_id = 0; answer_id < num_of_answers; answer_id++)
                {
                printf ("x = %f\n", arr_of_answers[answer_id]);
                }

            break;
            }
        case ZERO_ROOTS:
            {
            printf ("%s\nNo roots\n", delimiter);
            break;
            }
        case INFINITY_ROOTS:
            {
            printf ("%s\nInfinity roots\n", delimiter);
            break;
            }
        default:
            {
            printf ("%s\nWrong number of answers\n", delimiter);
            break;
            }
        }
    }


void stop_inst_console_close (void)
    {
    system ("pause");
    }

