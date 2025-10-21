//== knihovny ==
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//== deklarace funkci ==
char *najdiPrvniVelke(char str[]);
char *najdiPosledniVelke(char str[]);
void vymenZnaky(char *a, char *b);

//== definice funkci ==
char *najdiPrvniVelke(char str[])
{
    int delka = strlen(str);
    for (int i = 0; i < delka; i++)
    {
        if (isupper(str[i]))
            return &str[i];
    }
    return NULL;
}

char *najdiPosledniVelke(char str[])
{
    int delka = strlen(str);
    for (int i = delka - 1; i >= 0; i--)
        if (isupper(str[i]))
        {
            return &str[i];
        }
    return NULL;
}

void vymenZnaky(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

int main() // zacatek programu
{

    //--vstupni data--
    char str1[5] = "ABCD";           // druhy retezec
    char str2[14] = "Hello, World!"; // prvni retezec
    char str3[5] = "ahoj";           // tretiRetezec
    char *prvni = NULL;              // ukazatel na prvni velke
    char *posledni = NULL;           // ukazatel na posledni velke

    //--prvni retezec--
    prvni = najdiPrvniVelke(str1);         // hledame prvni    velke
    posledni = najdiPosledniVelke(str1);   // hledame posledni velke
    if (prvni != NULL && posledni != NULL) // pokud jsme je nasli
    {
        vymenZnaky(prvni, posledni); // velka pismena spolu vymenime
        printf("%s\n", str1);        // vypisujeme upraveny retezec
    }
    else // jinak piseme chybove hlaseni
    {
        fprintf(stderr, "Neplatny ukazatel u %s\n", str1);
    }

    //--druhy retezec--
    prvni = najdiPrvniVelke(str2);         // hledame prvni    velke
    posledni = najdiPosledniVelke(str2);   // hledame posledni velke
    if (prvni != NULL && posledni != NULL) // pokud jsme je nasli
    {
        vymenZnaky(prvni, posledni); // velka pismena spolu vymenime
        printf("%s\n", str2);        // vypisujeme upraveny retezec
    }
    else // jinak piseme chybove hlaseni
    {
        fprintf(stderr, "Neplatny ukazatel u %s\n", str2);
    }

    //--treti retezec--
    prvni = najdiPrvniVelke(str3);         // hledame prvni    velke
    posledni = najdiPosledniVelke(str3);   // hledame posledni velke
    if (prvni != NULL && posledni != NULL) // pokud jsme je nasli
    {
        vymenZnaky(prvni, posledni); // velka pismena spolu vymenime
        printf("%s\n", str3);        // vypisujeme upraveny retezec
    }
    else // jinak piseme chybove hlaseni
    {
        fprintf(stderr, "Neplatny ukazatel u %s\n", str3);
    }

    return 0; // konec programu
}