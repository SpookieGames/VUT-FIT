#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char arr[101];
    scanf("%100s", arr);
    int x = strlen(arr);
    printf("%s\n", arr);

    int pocet_velkych = 0;
    int pocet_malych = 0;
    int ostatne_znaky = 0;

    for (int i = 0; i < x; i++)
        if (isalpha(arr[i]))
        {
            if (isupper(arr[i]))
            {
                pocet_velkych += 1;
            }
            else
            {
                pocet_malych += 1;
            }
        }
        else
        {
            ostatne_znaky += 1;
            arr[i] = '-';
        }
    printf("%i %i %i\n", pocet_velkych, pocet_malych, ostatne_znaky);
    printf("%s\n", arr);

    return 0;
}