#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please enter 1 command-line argument\n");
        return 1;
    }
    char str[11];
    scanf("%10s", str);
    if (strcmp(argv[1], "tolower") == 0)
    {
        for (int i = 0; str[i]; i++)
        {
            str[i] = tolower(str[i]);
        }
        printf("%s\n", str);
    }
    if (strcmp(argv[1], "notalnum") == 0)
        for (int i = 0; str[i]; i++)
        {
            if (!isalnum(str[i]))
            {
                printf("%c", str[i]);
            }
        }
    if (strcmp(argv[1], "palindrom") == 0)
    {

        int lavo = 0;
        int pravo = strlen(str) - 1;
        int jePalindrom = 1;

        while (pravo > lavo)
        {
            if (str[lavo++] != str[pravo--])
            {
                jePalindrom = 0;
                break;
            }
        }

        if (jePalindrom)
        {
            printf("%s je palindrom\n", str);
        }
        else
        {
            printf("%s neni palindrom\n", str);
        }
        printf("\n");
    }
}