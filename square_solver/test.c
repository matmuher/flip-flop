#include "beer_equation.h"
/**
@brief Test solve_three_coefs_eq for extreme values
@param[in] test_id id of test
@param[in] coefs float array that stores 3 coefficients:\n
                 coefs[0] = A\n
                 coefs[1] = B\n
                 coefs[2] = C\n
@param[in] true_roots_num number of true roots
@param[in] true_roots float array, that includes true roots
@return 1 if test failed,\n
       0 if test was successful
*/
int test_square (int test_id, float *coefs, int true_roots_num, float *true_roots);

int main()
    {
    const int TESTS_NUM = 10;
    const int ARGS_NUM = 6;

    int failed = 0;
    float coefs[KOEFS_NUM] = {NAN,NAN,NAN};
    float true_roots[SQUARE_MAX_ROOTS_NUM] = {NAN,NAN};
    int true_roots_num = -1;
    float tests[TESTS_NUM][ARGS_NUM] =
    {
    //coefs[0] coefs[1] coefs[2] num_of_roots root#1  root#2   test
    {  0,      0,      -23,    ZERO_ROOTS,     NAN,     NAN}, // 1
    {  0,      0,        0,INFINITY_ROOTS,     NAN,     NAN}, // 2
    {  0,      2,       14,      ONE_ROOT,      -7,     NAN}, // 3
    {  7,      0,       14,    ZERO_ROOTS,     NAN,     NAN}, // 4
    { 15,      0,        0,     TWO_ROOTS,       0,       0}, // 5
    {  5,      0,     -125,     TWO_ROOTS,       5,      -5}, // 6
    {  1,     -8,       16,     TWO_ROOTS,       4,       4}, // 7
    {  1,      1,        1,    ZERO_ROOTS,     NAN,     NAN}, // 8
    {  1,      4,     1.75,     TWO_ROOTS,    -0.5,    -3.5}, // 9
    {  5,      6,        0,     TWO_ROOTS,    -1.2,       0}  // 10
    };

    for (int test_id = 0; test_id < TESTS_NUM; test_id++)
        {
        coefs[0] = tests[test_id][0];
        coefs[1] = tests[test_id][1];
        coefs[2] = tests[test_id][2];
        true_roots_num = tests[test_id][3];
        true_roots[0] = tests[test_id][4];
        true_roots[1] = tests[test_id][5];

        failed += test_square (test_id+1, coefs, true_roots_num, true_roots);
        }

    stop_inst_console_close ();

    return failed;
    }

int test_square (int test_id, float *coefs, int true_roots_num, float *true_roots)
    {
    int return_roots_num = -1;
    float return_roots[SQUARE_MAX_ROOTS_NUM] = {NAN,NAN};
    bool error = false;

    return_roots_num = solve_three_coefs_eq (coefs, return_roots);

    printf ("\nTest #%d\n", test_id);

    if (return_roots_num != true_roots_num ||
        !are_close_values (return_roots[0], true_roots[0]) ||
        !are_close_values (return_roots[1], true_roots[1]))
        {
        if (return_roots_num != true_roots_num)
            {
            puts ("[ERROR]: Wrong number of roots");
            error = true;
            }
        if (!are_close_values (return_roots[0], true_roots[0]))
            {
            puts ("[ERROR]: Wrong root#1");
            error = true;
            }
        if (!are_close_values (return_roots[1], true_roots[1]))
            {
            puts ("[ERROR]: Wrong root#2");
            error = true;
            }

        printf ("Coefficients are: %f, %f, %f\n"
                "True values:\n"
                " Number of roots = %d\n"
                " Root #1 = %f\n"
                " Root #2 = %f\n"
                "Return values:\n"
                " Number of roots = %d\n"
                " Root #1 = %f\n"
                " Root #2 = %f\n", coefs[0],  coefs[1], coefs[2], true_roots_num, true_roots[0], true_roots[1],
                                   return_roots_num, return_roots[0], return_roots[1]);
        }
    else
        {
        puts ("Successful!");
        }

    return error ? 1:0;
    }
