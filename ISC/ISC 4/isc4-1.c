#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        // int j = 0;
        /*do
        {
            printf("%c ", argv[i][j]);
            j++;
        } while (argv[i][j] != '\0');*/

        for (int j = 0; j < strlen(argv[i]); j++)
            if (argv[i][j] != '\0')
            {
                printf("%c ", argv[i][j]);
            }

        printf("\n");
    }

    return 0;
}