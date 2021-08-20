#include <stdio.h>
#include <math.h>
#define NUM_OF_KOEFS 3

int main ()
{
    float koef[NUM_OF_KOEFS];   //array with koefs

    printf ("The square equation looks like this:\n(koef_0)*x^2+(koef_1)*x+(koef_2)=0\n");

    for (int i = 0; i < NUM_OF_KOEFS; i++)         /*koefs input*/
        {
            printf ("Please, enter koef_%d\n",i);
            scanf ("%f",&koef[i]);
        }                                          /*solution*/

    if (koef[0] == 0) // x = -koef[2]/koef[1]
        {
            if (koef[1] == 0 && koef[2] != 0)    // f.e.: -5 = 0
                {
                    printf ("No roots");
                }
            else if (koef[1] == 0 && koef[2] == 0)       // 0 = 0
                {
                    printf ("Infinity(ne predel) roots");
                }
            else                                           // koef[1]*x + koef[2] = 0
                {
                    printf ("x = %f", -koef[2]/koef[1]);
                }
        }
    else
        {
            float D = koef[1]*koef[1] - 4*koef[0]*koef[2]; //formule of Discriminant

            if (D == 0)
                {
                    printf ("Full square: x = %f", -koef[1]/( 2*koef[0] ) );
                }
            if (D < 0)
                {
                    printf ("No ru(le)ts");
                }
            if (D > 0)
                {
                    float x1 = ( -koef[1]+sqrt(D) )/( 2*koef[0] );
                    float x2 = ( -koef[1]-sqrt(D) )/( 2*koef[0] );
                    printf ("Two roots: x = %f, x = %f", x1, x2);
                }

        }



    return 0;
}
