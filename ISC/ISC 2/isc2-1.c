#include <stdio.h>

int main()
{
    char ch = 'h';
    int posun = 5;
    printf("Znak: %c, posun: %d\n", ch, posun);

    ch = ch + posun;

    printf("Posunuty znak: %c\n", ch);

    ch = ch - 32;

    printf("Velky znak: %c\n", ch);
}