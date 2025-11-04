//== knihovny ==
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//== deklarace funkci ==
int *vektorovySoucet(int delka, int arr1[], int arr2[]);
int *skalarniSoucin(int delka, int arr1[], int arr2[]);
char *konkatenaceRetezcu(char str1[], char str2[]);

//== definice funkci ==
int *vektorovySoucet(int delka, int arr1[], int arr2[])
{
    int *arr3 = malloc(sizeof(int) * delka); // dynamicky alokujeme pole

    if (arr3 != NULL) // pokud alokace uspela
    {
        for (int i = 0; i < delka; i++)
        {
            arr3[i] = arr2[i] + arr1[i];
        } // spocitame vektorovy soucet
    }

    return arr3; // vracime nove pole
}

int *skalarniSoucin(int delka, int arr1[], int arr2[])
{
    int *soucin = malloc(sizeof(int));
    if (soucin != NULL)
    {
        *soucin = 0;
        for (int i = 0; i < delka; i++)
        {
            *soucin += arr1[i] * arr2[i];
        }
    }
    return soucin;
}

char *konkatenaceRetezcu(char str1[], char str2[])
{
    int delka = strlen(str1) + strlen(str2);
    char *konkatenace = malloc(sizeof(char) * (delka + 1));

    if (konkatenace != NULL)
    {
        int delka_str1 = strlen(str1);
        for (int i = 0; i < strlen(str1); i++)
        {
            konkatenace[i] = str1[i];
        }
        for (int j = delka_str1; j < delka; j++)
        {
            konkatenace[j] = str2[j-delka_str1];
        }
        konkatenace[delka + 1] = '\0';
    }
    return konkatenace;
}

int main()
{
    int arr1[3] = {10, 20, 30}; // prvni pole
    int arr2[3] = {40, 50, 60}; // druhe pole
    char str1[6] = "Hello";     // prvni retezec
    char str2[5] = "Ahoj";      // druhy retezec

    int *arr3 = vektorovySoucet(3, arr1, arr2); // volame funkci soucet
    if (arr3 != NULL)                           // pokud alokace uspela
    {
        printf("Soucet: ");
        for (int i = 0; i < 3; i++) // pro vsechny prvky pole
            printf("%i ", arr3[i]); // vypisujeme soucty
        printf("\n");               // vypisujeme konec radku
    }

    int *soucin = skalarniSoucin(3, arr1, arr2); // volame funkci soucin
    if (soucin != NULL)                          // pokud alokace uspela
        printf("Soucin: %i\n", *soucin);         // vypisujeme soucin

    char *str3 = konkatenaceRetezcu(str1, str2); // volame funkci konkatenace
    if (str3 != NULL)                            // pokud alokace uspela
        printf("Konkatenace: %s\n", str3);       // vypisujeme konkatenaci

    if (arr3 != NULL)   // pokud alokace pole pro soucet uspela
        free(arr3);     // uvolnujeme dynamicky alokovanou pamet
    if (soucin != NULL) // pokud alokace cisla pro soucin uspela
        free(soucin);   // uvolnujeme dynamicky alokovanou pamet
    if (str3 != NULL)   // pokud alokace pole pro konkatenaci
        free(str3);     // uvolnujeme dynamicky alokovanou pamet

    return 0; // konec programu
}
