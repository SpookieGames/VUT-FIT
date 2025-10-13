#include <stdio.h>
#include <math.h>

int main()
{
    float y[5];
    for (int i = 0; i < 5; i++)
    {
        scanf("%f", &y[i]);
    }
    // Maximum
    float max = y[0];
    for (int i = 0; i < 5; i++)
    {
        if (y[i] > max)
        {
            max = y[i];
        }
    }
    // Minimum index
    float min = y[0];
    int minIndex = 0;
    for (int i = 0; i < 5; i++)
    {
        if (y[i] < min)
        {
            min = y[i];
            minIndex = i;
        }
    }

    // Opacne poradie
    for (int i = 4; i < 5 && i >= 0; --i)
    {
        printf("%f ", y[i]);
    }
    printf("\n%f\n", max);
    printf("%i\n", minIndex);
}