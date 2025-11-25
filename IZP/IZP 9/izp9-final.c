//== knihovny ==
#include <stdio.h>
#include <string.h>

//== nove datove typy ==
typedef struct Sosoba // struktura "Sosoba"
{
    char jmeno[10]; // pole znaku "jmeno"
    int vek;        // cele cislo "vek"
} osoba;            // datovy typ "osoba"

//== deklarace funkci ==
void vypis(int delka, osoba pole[]);
int porovnej(osoba A, osoba B);
void vymen(osoba *A, osoba *B);
int nejmensi(int delka, osoba pole[]);
void serad(int delka, osoba pole[]);

//== definice funkci ==
// vypise jmeno a vek vsech osob pole
void vypis(int delka, osoba pole[])
{
    for (int i = 0; i < delka; i++) // pro vsechny prvky pole
    {
        printf("(%s %i) ", pole[i].jmeno, pole[i].vek); // vypis jmeno a vek
    }
    printf("\n"); // vypis konec radku
}

// Porovna JMENA dvou osob, a pokud jsou stejna tak porovna i jejich VEK.
// Pokud je jmeno prvni osoby abecedne mensi nez jmeno druhe osoby, vrati ZAPORNE cislo.
// Pokud je jmeno prvni osoby abecedne vetsi nez jmeno druhe osoby, vrati KLANDE cislo.
// Pokud jsou jmena stejna a prvni osoba je mladsi, vrati ZAPORNE cislo.
// Pokud jsou jmena stejna a prvni osoba je starsi, vrati KLADNE cislo.
// Pokud jsou jmena stejna a vek je take stejny, vrati NULU.
int porovnej(osoba A, osoba B)
{
    int vysledok = strcmp(A.jmeno, B.jmeno);
    if (vysledok == 0)
    {
        if (A.vek < B.vek)
        {
            return -1;
        }
        if (A.vek > B.vek)
        {
            return 1;
        }
        if (A.vek == B.vek)
        {
            return 0;
        }
    }
    if (vysledok < 0)
    {
        return -1;
    }
    if (vysledok > 0)
    {
        return 1;
    }
}

// vzajemne vymeni dve osoby predane odkazem
void vymen(osoba *A, osoba *B)
{
    osoba TMP = *A;
    *A = *B;
    *B = TMP;
}

// vrati index osoby s nejmensi hodnotu (podle funcke porovnej)
int nejmensi(int delka, osoba pole[])
{
    int minimum = 0;
    for (int i = 0; i < delka; i++)
    {
        if (porovnej(pole[i], pole[minimum]) < 0)
            minimum = i;
    }
    return minimum;
}

// pole osob seradi od nejmensi po nejvetsi, algoritmem SELECT-SORT
void serad(int delka, osoba pole[])
{
    for (int i = 0; i < delka - 1; i++)
    {
        int minimum_index = i;
        for (int j = i + 1; j < delka; j++)
        {
            if (porovnej(pole[j], pole[minimum_index]) < 0)
                minimum_index = j;
        }
        vymen(&pole[i], &pole[minimum_index]);
    }
}

// hlavni funkce programu, vytvori vzorova data a zavola implementovane funkce
int main()
{ // neserazene pole sosob
    osoba pole[5] = {{"Bob", 23}, {"Cecil", 23}, {"Bob", 22}, {"Alice", 24}, {"Daniela", 21}};
    vypis(5, pole); // vypis pole

    if (porovnej(pole[0], pole[1]) < 0) // test funkce porovnej (ruzna jemna)
        printf("Ruzna jmena    (0,1) OK\n");
    if (porovnej(pole[0], pole[2]) > 0) // test funkce porovnej (stejna jmena, ruzny vek)
        printf("Ruzny vek      (0,2) OK\n");

    vymen(&pole[0], &pole[4]); // vymena dvou osob
    vypis(5, pole);            // vypis pole

    if (porovnej(pole[0], pole[4]) > 0) // test funkce vymen
        printf("Vymena   osob  (0,4) OK\n");
    if (nejmensi(5, pole) == 3) // test funkce nejmensi
        printf("Nejmensi osoba (3)   OK\n");

    serad(5, pole); // test funkce serad
    vypis(5, pole); // vypis pole
    return 0;
}
