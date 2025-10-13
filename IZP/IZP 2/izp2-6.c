#include <stdio.h>

int main()
{
    int x, y;
    scanf("%i %i", &x, &y);
    if (x > y)
    {
        int z = x;
        x = y;
        y = z;
    }
    for (int i = x; i <= y; i++)
    {
        printf("%i ", i);
    }
}
