#include "beer_equation.h"

int test_square (int test_id, float *koefs, int true_roots_num, float *true_roots);

int main()
    {
    const int TESTS_NUM = 9;
    const int ARGS_NUM = 6;

    int failed = 0;
    float koefs[KOEFS_NUM] = {NAN,NAN,NAN};
    float true_roots[MAX_ROOTS_NUM] = {NAN,NAN};
    int true_roots_num = -1;
    float tests[TESTS_NUM][ARGS_NUM] =
    {
    //koef[0] koef[1] koef[2] num_of_roots root#1  root#2   test
    {  0,      0,      -23,       ZERO,     NAN,     NAN}, // 1
    {  0,      0,        0,        INF,     NAN,     NAN}, // 2
    {  0,      2,       14,        ONE,      -7,     NAN}, // 3
    {  7,      0,       14,       ZERO,     NAN,     NAN}, // 4
    { 15,      0,        0,        TWO,       0,       0}, // 5
    {  5,      0,     -125,        TWO,       5,      -5}, // 6
    {  1,     -8,       16,        TWO,       4,       4}, // 7
    {  1,      1,        1,       ZERO,     NAN,     NAN}, // 8
    {  1,      4,     1.75,        TWO,    -0.5,    -3.5}  // 9
    };

    for (int test_id = 0; test_id < TESTS_NUM; test_id++)
        {
        koefs[0] = tests[test_id][0];
        koefs[1] = tests[test_id][1];
        koefs[2] = tests[test_id][2];
        true_roots_num = tests[test_id][3];
        true_roots[0] = tests[test_id][4];
        true_roots[0] = tests[test_id][5];

        failed += test_square (test_id+1, koefs, true_roots_num, true_roots);
        }

    stop_inst_console_close ();

    return failed;
    }

int test_square (int test_id, float *koefs, int true_roots_num, float *true_roots)
    {
    int return_roots_num = NAN;
    float return_roots[MAX_ROOTS_NUM] = {NAN,NAN};
    bool error = false;

    return_roots_num = solve_three_koefs_eq (koefs, return_roots);

    printf ("\nTest #%d\n", test_id);

    if (return_roots_num != true_roots_num &&
        !are_close_values (return_roots[0], true_roots[0]) &&
        !are_close_values (return_roots[0], true_roots[0]))
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

        printf ("True values:\n"
                " Number of roots = %d\n"
                " Root #1 = %f\n"
                " Root #2 = %f\n"
                "Return values:\n"
                " Number of roots = %d\n"
                " Root #1 = %f\n"
                " Root #2 = %f\n", true_roots_num, true_roots[0], true_roots[1],
                                   return_roots_num, return_roots[0], return_roots[1]);
        }
    else
        {
        puts ("Successful!");
        }

    return error ? 1:0;
    }
