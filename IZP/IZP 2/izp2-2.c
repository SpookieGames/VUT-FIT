#include <stdio.h>
#include <math.h>

int main()
{
    float x;
    float sucet = 0;

    scanf("%f", &x);
    sucet += x;
    printf("Sucet je: %f \n", sucet);

    while (x != 0)
    {
        scanf("%f", &x);
        sucet += x;
        printf("Sucet je: %f \n", sucet);
    }
}