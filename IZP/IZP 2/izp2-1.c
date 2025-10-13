#include <stdio.h>
#include <math.h>

int main()
{
    float a, b, c;

    scanf("%f %f %f", &a, &b, &c);

    float d = b * b - 4 * a * c;
    if (a == 0)
    {
        printf("Nie je kvadraticka rovnica \n");
        return 0;
    }
    if (d < 0)
    {
        printf("Nie je riesenie \n");
    }
    else if (d == 0)
    {
        float x = -b / (2 * a);
        printf("Ma jedno riesenie: %f \n", x);
    }
    else
    {
        float x1 = (-b + sqrt(d)) / (2 * a);
        float x2 = (-b - sqrt(d)) / (2 * a);
        printf("Ma dve riesenia: %f a %f \n", x1, x2);
    }
    return 0;
}