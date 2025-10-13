#include <stdio.h>
#include <ctype.h>

int main()
{
    int cisla = 0;
    int pismena = 0;
    int znaky = 0;
    char x;
    while (scanf("%c", &x) != EOF)
    {
        if (isdigit(x))
        {
            cisla++;
        }
        else if (isalpha(x))
        {
            pismena++;
        }
        else
        {
            znaky++;
        }
    }
    printf("%i %i %i\n", cisla, pismena, znaky);
}