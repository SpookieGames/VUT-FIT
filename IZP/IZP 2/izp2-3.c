#include <stdio.h>
#include <math.h>

int main()
{
    int x;

    scanf("%i", &x);

    while (x > 0)
    {
        printf("%i %i \n", x, x % 2);
        x = x / 2;
    }
}