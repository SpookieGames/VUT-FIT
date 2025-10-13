#include <stdio.h>
#include <ctype.h>

int main()
{
    char x, y;
    scanf("%s %s", &x, &y);
    if (isdigit(x))
    {
        printf("Prvy znak je cislo\n");
    }
    else
    {
        printf(" Prvy znak nie je cislo\n");
    }

    if (!isalpha(y))
    {
        printf("Druhy znak nie je pismeno\n");
    }
    else
    {
        printf("Druhy znak je pismeno\n");
    }

    if (!isalnum(x) || !isalnum(y))
    {
        printf("Splnene 3.\n");
    }
    else
    {
        printf("Nesplnene 3.\n");
    }

    if (isalpha(x) && isalpha(y) && tolower(x) == tolower(y))
    {
        printf("Splnene 4.\n");
    }
    else
    {
        printf("Nesplnene 4.\n");
    }
    return 0;
}