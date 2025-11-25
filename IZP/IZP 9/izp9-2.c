//== knihovny ==
#include <stdio.h>

//== deklarace funkci ==
int binarniVyhledavani(int zacatek, int konec, int pole[], int cislo);

//== definice funkci ==
// implementuje algoritmus binarniho vyhledavani pomoci REKURZE
int binarniVyhledavani(int zacatek, int konec, int pole[], int cislo)
{
    if (konec < zacatek)
        return -1;
    int stred = zacatek + ((konec - zacatek) / 2);
    if (pole[stred] == cislo)
    {
        return stred;
    }
    if (cislo < pole[stred])
    {
        return binarniVyhledavani(zacatek, stred - 1, pole, cislo);
    }
    else
    {
        return binarniVyhledavani(stred + 1, konec, pole, cislo);
    }
}
// hlavni funkce programu, vytvori serazene pole a vyhleda v nem nejake hodnoty
int main(void)
{
    int d = 20;                                // prvni hledane cislo
    int e = 60;                                // druhe hledane cislo
    int f = 70;                                // treti hledane cislo
    int arr[7] = {10, 20, 30, 50, 60, 80, 90}; // prohledavane pole (serazene)

    // volani hledani pomoci rekurze
    printf("Cislo %i je na indexu %i\n", d, binarniVyhledavani(0, 6, arr, d));
    printf("Cislo %i je na indexu %i\n", e, binarniVyhledavani(0, 6, arr, e));
    printf("Cislo %i je na indexu %i\n", f, binarniVyhledavani(0, 6, arr, f));
    return 0; // konec programu
}