#include <stdio.h>

int main()
{
    int x, y;
    scanf("%i %i", &x, &y);
    for (int i = x; i <= y; i++)
    {
        printf("%i ", i);
    }
    for (int i = y; i <= x; i++)
    {
        printf("%i ", i);
    }
}
