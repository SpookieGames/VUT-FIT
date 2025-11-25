//== knihovny ==
#include <stdio.h>

//== deklarace funkci ==
int rekurzeFibonacci(int n);
int cyklusFibonacci(int n);

//== definice funkci ==
// implementuje vypocet fibonacciho posloupnosti pomoci REKURZE
int rekurzeFibonacci(int n)
{
    if (n == 0)
    {
        return 0;
    }
    if (n == 1)
    {
        return 1;
    }
    return rekurzeFibonacci(n - 1) + rekurzeFibonacci(n - 2);
}

// implementuje vypocet fibonacciho posloupnosti pomoci CYKLU
int cyklusFibonacci(int n)
{
    if (n == 0)
    {
        return 0;
    }
    if (n == 1)
    {
        return 1;
    }
    int a = 0;
    int b = 1;
    int c;
    for (int i = 2; i <= n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

// hlavni funkce programu, vola ostatni funkce pro vzorove vstupy
int main(void)
{
    int a = 5;  // paty     clen
    int b = 10; // desaty   clen
    int c = 15; // patnacty clen

    // volani vypoctu pomoci rekurze
    printf("Rekurze f(%2i) = %3i\n", a, rekurzeFibonacci(a));
    printf("Rekurze f(%2i) = %3i\n", b, rekurzeFibonacci(b));
    printf("Rekurze f(%2i) = %3i\n", c, rekurzeFibonacci(c));

    // volani vypoctu pomoci cyklu
    printf("Cyklus  f(%2i) = %3i\n", a, cyklusFibonacci(a));
    printf("Cyklus  f(%2i) = %3i\n", b, cyklusFibonacci(b));
    printf("Cyklus  f(%2i) = %3i\n", c, cyklusFibonacci(c));
    return 0;
}