#include <stdio.h>
#include <string.h>
#include <ctype.h>

int jeZhodne(int a[], int b[])
{
    int zhodne = 0;
    for (int i = 0; i < 5; i++)
    {
        int flag = 0;
        for (int j = 0; j < i; j++)
        {
            if (a[i] == b[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag)
        {
            continue;
        }
        for (int j = 0; j < 5; j++)
        {
            if (a[i] == b[j])
            {
                zhodne++;
                break;
            }
        }
    }
    return zhodne;
}

int main()
{
    int arr1[5] = {1, 2, 4, 4, 5};
    int arr2[5] = {1, 9, 4, 4, 5};
    printf("%d\n", jeZhodne(arr1, arr2));
}