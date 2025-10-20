// Pouzite kniznice
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Deklaracia funkcii
int neparny_palindrom(char vstup[], int dlzka_vstupu, int *zaciatok_neparny);
int parny_palindrom(char vstup[], int dlzka_vstupu, int *zaciatok_parny);
void print(int maximalna_dlzka, int zaciatok, char vstup[]);

// Funkcia na ziskanie palindromu neparnej dlzky
// Kazdy znak s indexom [i] vo vstupe pouzijeme ako stred palindromu a na zaklade porovnania rozsirujeme overovany retazec vlavo a vpravo
int neparny_palindrom(char vstup[], int dlzka_vstupu, int *zaciatok_neparny)
{
    int maximalna_dlzka_neparny = 1; // Inicializacia premennej
    for (int i = 0; i < dlzka_vstupu; i++)
    {
        // Zacatie overovania v 1 strednom bode (1 pismeno je stred)
        int vlavo = i;
        int vpravo = i;
        while (vpravo < dlzka_vstupu && vlavo >= 0 && (toupper(vstup[vlavo])) == (toupper(vstup[vpravo]))) // Ohranicenie a porovnanie lavej a pravej strany (nerozlišuje veľké a malé písmená)
        {
            int aktualna_dlzka = (vpravo - vlavo) + 1;    // Ziskanie dlzky naposledy overeneho palindromu
            if (aktualna_dlzka > maximalna_dlzka_neparny) // Ak je aktualna dlzka dlhsia ako ulozena maximalna dlzka
            {
                maximalna_dlzka_neparny = aktualna_dlzka; // Nova maximalna dlzka bude aktualna dlzka
                *zaciatok_neparny = vlavo;                // Cez pointer nasledne vratime index zaciatku do funckie main
            }
            vlavo--;  // Posunutie vlavo o 1
            vpravo++; // Posunutie vpravo o 1
        }
    }
    return maximalna_dlzka_neparny; // Navrat maximalnej dlzky neparneho palindromu
}

// Funkcia na ziskanie palindromu parnej dlzky
// Kazde 2 znaky s indexom[i] a [i+1] vo vstupe pouzijeme ako stred palindromu a na zaklade porovnania rozsirujeme overovany retazec vlavo a vpravo
int parny_palindrom(char vstup[], int dlzka_vstupu, int *zaciatok_parny)
{
    int maximalna_dlzka_parny = 0; // Inicializacia premennej
    for (int i = 0; i < dlzka_vstupu; i++)
    {
        // Stred su 2 pismena
        int vlavo = i;
        int vpravo = i + 1;
        while (vpravo < dlzka_vstupu && vlavo >= 0 && (toupper(vstup[vlavo])) == (toupper(vstup[vpravo]))) // Ohranicenie a porovnanie lavej a pravej strany (nerozlišuje veľké a malé písmená)
        {
            int aktualna_dlzka = (vpravo - vlavo) + 1;  // Ziskanie dlzky naposledy overeneho palindromu
            if (aktualna_dlzka > maximalna_dlzka_parny) // Ak je aktualna dlzka dlhsia ako ulozena maximalna dlzka
            {
                maximalna_dlzka_parny = aktualna_dlzka; // Nova maximalna dlzka bude aktualna dlzka
                *zaciatok_parny = vlavo;                // Cez pointer nasledne vratime index zaciatku do funckie main
            }
            vlavo--;  // Posunutie vlavo o 1
            vpravo++; // Posunutie vpravo o 1
        }
    }

    return maximalna_dlzka_parny; // Navrat maximalnej dlzky parneho palindromu
}

// Funkcia na vypisanie vysledku
void print(int maximalna_dlzka, int zaciatok, char vstup[])
{
    char vysledny_palindrom[maximalna_dlzka + 1]; // Inicializacia premennej
    for (int i = 0; i < maximalna_dlzka; i++)
    {
        vysledny_palindrom[i] = vstup[zaciatok + i]; // Doplnenie znak po znaku zo vstupu
    }
    vysledny_palindrom[maximalna_dlzka] = '\0';                // Ukoncenie koncovym znakom '\0' po ukonceni cyklu
    printf("Najdlhsi palindrom je: %s\n", vysledny_palindrom); // Vypisanie vysledku do stdout
}

// Funkcia main
int main(int argc, char *argv[])
{
    char *vstup = argv[1]; // Nacitanie vstupu z argumentu
    if (argc != 2)         // Overenie poctu argumentov
    {
        printf("Zadajte 1 argument!\n");
        return 1;
    }
    int dlzka_vstupu = strlen(vstup);                                                // Ziskanie dlzky vstupu pomocou strlen
    int zaciatok_neparny = 0, zaciatok_parny = 0, maximalna_dlzka = 0, zaciatok = 0; // Inicializacia premennych

    int maximalna_dlzka_neparny = neparny_palindrom(vstup, dlzka_vstupu, &zaciatok_neparny); // Volanie funkcie a navrat maximalna_dlzka_neparny a zaciatok_neparny pomocou pointera
    int maximalna_dlzka_parny = parny_palindrom(vstup, dlzka_vstupu, &zaciatok_parny);       // Volanie funkcie a navrat maximalna_dlzka_parny a zaciatok_parny pomocou pointera

    // Zistenie ci je najdlhsie palindrom parny alebo neparny
    if (maximalna_dlzka_neparny > maximalna_dlzka_parny)
    {
        maximalna_dlzka = maximalna_dlzka_neparny;
        zaciatok = zaciatok_neparny;
    }
    else
    {
        maximalna_dlzka = maximalna_dlzka_parny;
        zaciatok = zaciatok_parny;
    }

    print(maximalna_dlzka, zaciatok, vstup); // Volanie funkcie na vypisanie vysledku

    return 0;
}