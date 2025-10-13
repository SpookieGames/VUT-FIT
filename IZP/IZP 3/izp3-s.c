#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int array_find_item(int array[], int size, int item)
{
    for (int idx = 0; idx < size; idx++)
    {
        if (array[idx] == item)
        {
            return idx;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    int pole[] = {1, 2, 5, 6, 8, 9, 42, 15};

    array_find_item(pole, 7, 42);

    return 0;
}