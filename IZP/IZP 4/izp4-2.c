#include <stdio.h>                        // vlozeni knihovny
int nejvetsiDelitel(int a, int b);        // deklarace funkce
int nejmensiNasobek(int a, int b, int z); // deklarace funkce

int nejvetsiDelitel(int a, int b)
{
    int najmensie_cislo = 0;
    if (a >= 0 && b >= 0)
    {
        if (a < b)
        {
            najmensie_cislo = a;
        }
        else
        {
            najmensie_cislo = b;
        }

        for (int i = najmensie_cislo; i > 1; i--)
        {
            if (a % i == 0 && b % i == 0)
            {
                return i;
            }
        }
    }
    else
    {
        return 0;
    }
}
int nejmensiNasobek(int a, int b, int z)
{
    if (a >= 0 && b >= 0)
    {
        int c = a * b;
        int x = c / z;
        return x;
    }
    else
    {
        return 0;
    }
}

int main() // zacatek programu
{
    int x, y;                                     // vytvor promenne
    scanf("%i %i", &x, &y);                       // nacti do nich vstup
    int delitel = nejvetsiDelitel(x, y);          // zavolej prvni funkci
    int nasobek = nejmensiNasobek(x, y, delitel); // zavolej druhou funkci
    printf(" delitel = %i\n", delitel);           // vypis
    printf(" nasobek = %i\n", nasobek);           // vypis
    return 0;                                     // konec programu
}