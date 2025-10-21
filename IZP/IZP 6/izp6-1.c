#include <stdio.h>

void vymenCisla(int *a, int *b);
void obratVektor(int n, int arr[n]);
void transponujMatici(int n, int arr[n][n]);

void vymenCisla(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void obratVektor(int n, int arr[n])
{
    for (int i = 0; i < n / 2; i++)
    {
        vymenCisla(&arr[i], &arr[(n - 1) - i]);
    }
}

void transponujMatici(int n, int arr[n][n])
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            vymenCisla(&arr[i][j], &arr[j][i]);
        }
    }
}

int main()
{
    int vektor[5] = {1, 2, 3, 4, 5};
    printf("Puvodni vektor: 1 2 3 4 5\n");
    obratVektor(5, vektor);
    printf("Obraceny vektor: ");
    for (int i = 0; i < 5; i++)
        printf("%i ", vektor[i]);
    printf("\n\n");

    int matice[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    printf("Puvodni matice:\n1 2 3\n4 5 6\n7 8 9\n");
    transponujMatici(3, matice);
    printf("Transponovana matice:\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%i ", matice[i][j]);
        printf("\n");
    }
    return 0;
}
