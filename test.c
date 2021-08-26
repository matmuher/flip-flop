#include "beer_equation.h"

void test (float *koefs, int true_roots_num, float *true_roots, int test_id);

int main()
{
int test_id = 0;
float koefs[NUM_OF_KOEFS] = {NAN,NAN,NAN};
float true_roots[ROOTS_LIMIT] = {NAN,NAN};
int true_roots_num = NAN;


// test #1
test_id = 1;
koefs[0] = 0;
koefs[1] = 0;
koefs[2] = -23;
true_roots[0] = NAN;
true_roots[1] = NAN;
true_roots_num = ZERO;
test (koefs, true_roots_num, true_roots, test_id);

// test #2
test_id = 2;
koefs[0] = 0;
koefs[1] = 0;
koefs[2] = 0;
true_roots[0] = NAN;
true_roots[1] = NAN;
true_roots_num = INF;
test (koefs, true_roots_num, true_roots, test_id);


// test #3
test_id = 3;
koefs[0] = 0;
koefs[1] = 2;
koefs[2] = 14;
true_roots[0] = -7;
true_roots[1] = NAN;
true_roots_num = ONE;
test (koefs, true_roots_num, true_roots, test_id);

// test #4
test_id = 4;
koefs[0] = 7;
koefs[1] = 0;
koefs[2] = 14;
true_roots[0] = NAN;
true_roots[1] = NAN;
true_roots_num = ZERO;
test (koefs, true_roots_num, true_roots, test_id);

// test #5
test_id = 5;
koefs[0] = 15;
koefs[1] = 0;
koefs[2] = 0;
true_roots[0] = 0;
true_roots[1] = 0;
true_roots_num = TWO;
test (koefs, true_roots_num, true_roots, test_id);

// test #6
test_id = 6;
koefs[0] = 5;
koefs[1] = 0;
koefs[2] = -125;
true_roots[0] = 5;
true_roots[1] = -5;
true_roots_num = TWO;
test (koefs, true_roots_num, true_roots, test_id);

// test #7
test_id = 7;
koefs[0] = 1;
koefs[1] = -8;
koefs[2] = 16;
true_roots[0] = 4;
true_roots[1] = 4;
true_roots_num = TWO;
test (koefs, true_roots_num, true_roots, test_id);

// test #8
test_id = 8;
koefs[0] = 1;
koefs[1] = 1;
koefs[2] = 1;
true_roots[0] = NAN;
true_roots[1] = NAN;
true_roots_num = ZERO;
test (koefs, true_roots_num, true_roots, test_id);

// test #9
test_id = 9;
koefs[0] = 1;
koefs[1] = 4;
koefs[2] = 1.75;
true_roots[0] = -0.5;
true_roots[1] = -3.5;
true_roots_num = TWO;
test (koefs, true_roots_num, true_roots, test_id);

// to avoid instant close of console when call .exe
scanf ("%*s");

return 0;
}

void test (float *koefs, int true_roots_num, float *true_roots, int test_id)
{
float return_roots[ROOTS_LIMIT] = {NAN,NAN};
int return_roots_num = NAN;
bool error = 0;

return_roots_num = three_koefs_eq (koefs, return_roots);

if (true_roots_num == ZERO)
    {
    if (return_roots_num != true_roots_num)
        {
        puts ("[ERROR]: Wrong number of roots");
        error = 1;
        }
    if (!isnan(return_roots[0]))
        {
        puts ("[ERROR]: Wrong root #1");
        error = 1;
        }
    if (!isnan(return_roots[1]))
        {
        puts ("[ERROR]: Wrong root #2");
        error = 1;
        }
    if (!error)
        {
        printf ("Test #%d was successfully finished\n", test_id);
        }
    }
if (true_roots_num == ONE)
    {
    if (return_roots_num != true_roots_num)
        {
        puts ("[ERROR]: Wrong number of roots");
        error = 1;
        }
    if (return_roots[0] != true_roots[0])
        {
        puts ("[ERROR]: Wrong root #1");
        error = 1;
        }
    if (!isnan(return_roots[1]))
        {
        puts ("[ERROR]: Wrong root #2");
        error = 1;
        }
    if (!error)
        {
        printf ("Test #%d was successfully finished\n", test_id);
        }
    }
if (true_roots_num == TWO)
    {
    if (return_roots_num != true_roots_num)
        {
        puts ("[ERROR]: Wrong number of roots");
        error = 1;
        }
    if (return_roots[0] != true_roots[0])
        {
        puts ("[ERROR]: Wrong root #1");
        error = 1;
        }
    if (return_roots[1] != true_roots[1])
        {
        puts ("[ERROR]: Wrong root #2");
        error = 1;
        }
    if (!error)
        {
        printf ("Test #%d was successfully finished\n", test_id);
        }
    }
}
