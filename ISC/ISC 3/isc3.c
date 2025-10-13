#include <stdio.h>

int add(int cislo_a, int cislo_b)
{
    int sucet;
    sucet = cislo_a + cislo_b;
    return sucet;
}

int div(int cislo_a, int cislo_b)
{
    if (cislo_b != 0)
    {
        int delenie;
        delenie = cislo_a / cislo_b;
        return delenie;
    }
    else
    {
        printf("Neda sa delit 0\n");
    }
}

int main()
{
    int cislo_a;
    int cislo_b;
    char operator;
    printf("Zadej operator (+ nebo /): ");
    scanf(" %c", &operator);
    printf("Zadej hodnotu a: ");
    scanf("%d", &cislo_a);
    printf("Zadej hodnotu b: ");
    scanf("%d", &cislo_b);

    printf("Operator: %c, cislo_a: %d, cislo_b: %d\n", operator, cislo_a, cislo_b);

    if (operator == '/')
    {
        printf("Vysledok delenia je %d\n", div(cislo_a, cislo_b));
    }
    else if (operator == '+')
    {
        printf("Vysledok suctu je %d\n", add(cislo_a, cislo_b));
    }
    else
    {
        printf("Zadaj spravny operator\n");
    }

    return 0;
}
