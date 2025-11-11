//== knihovny ==
#include <stdio.h>   //vstup a vystup
#include <stdlib.h>  //dynamicka alokace pameti
#include <stdbool.h> //pravdivostni hodnoty

//== definice funkci ==
int main() // zacatek programu
{
    int delka = 3;                           // pocet nacitanych cisel
    int *pole = malloc(delka * sizeof(int)); // alokujeme pole pro "delka" cisel
    FILE *soubor = fopen("input.txt", "r");  // otevirame vstupni soubor
    if (pole != NULL)
    {
        if (soubor != NULL)
        {
            for (int i = 0; i < delka; i++) // pro vsechny prvky pole
            {
                if (fscanf(soubor, "%i", &pole[i]) != 1)
                {
                    fprintf(stderr, "Neplatny vstup\n");
                    fclose(soubor);
                    free(pole);
                    return 1;
                }
            }
        }
        else
        {
            fprintf(stderr, "Nepodarilo sa otvorit subor\n");
            free(pole);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Nepodarilo sa alokovat pamat\n");
        return 1;
    }

    bool rostouci = true; // posloupnost je rostouci

    for (int i = 0; i < delka - 1; i++) // pro vsechny prvky pole
    {
        if (!(pole[i] < pole[i + 1])) // pokud dalsi neni vetsi
        {
            rostouci = false; // posloupnost neni rostouci
        }
    }

    if (rostouci) // pokud jsme nenasli problem
    {
        printf("JE rostouci\n");
        fclose(soubor);
        free(pole); // posloupnost JE rostouci
    }
    else // jinak
    {
        printf("NENI rostouci\n");
        fclose(soubor);
        free(pole); // posloupnost NENI rostouci
    }
    return 0; // konec programu
}