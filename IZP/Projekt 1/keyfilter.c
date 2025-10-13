// Nacitanie potrebnych kniznic
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHAR_RIADOK 251 // Definicia maximalneho poctu charakterov
#define MAX_MIEST 100       // Definicia maximalneho poctu riadkov (adries)

// Deklaracia pouzitych funkcii
void zoradenie_moznych_znakov(char znak[]);
int citanie_suboru(char mesta[][MAX_CHAR_RIADOK]);
int zistenie_dlzky_vstupu(char *vstup, int len_vstupu);
char *premena_vstupu_na_velke_pismena(char *vstup, char *velky_vstup);
void zistenie_nasledujucich_znakov(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], char *velky_vstup, int dlzka_vstupu, char *nasledujuci_znak);
void filtrovanie_nasledujucich_znakov(char *nasledujuci_znak);
void output(char mesta[][MAX_CHAR_RIADOK], int doplnenie_index, char *velky_vstup, int pocet_znakovych_zhod, char nasledujuci_znak[], int zhoda);
int najdenie_zhody(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], char *velky_vstup);
int spocitanie_zhod(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], const char *velky_vstup, int dlzka_vstupu, int *doplnenie_index);

/*
    Funkcia zoradenie_moznych_znakov sluzi na abecedne zoradnie znakov, ktore budu nasledne vypisane ako mozne znaky (Enable: MOZNE ZNAKY) podla algoritmu Selection Sort
    Parameter predavany funkcii je nasledujuci_znak[MAX_MIEST]
*/
void zoradenie_moznych_znakov(char znak[])
{
    int n = 0;
    while (znak[n] != '\0')
        n++; // Zistenie poctu znakov
    for (int i = 0; i < n - 1; i++)
    {
        int index_minima = i;
        for (int j = i + 1; j < n; j++)
        {
            if (znak[j] < znak[index_minima])
            {
                index_minima = j;
            }
        }
        char tmp = znak[index_minima]; //
        znak[index_minima] = znak[i];  // Zamena znaku (swap)
        znak[i] = tmp;                 //
    }
}

/*
    Funkcia citanie_suboru sluzi na nacitanie textu riadok po riadku pomocou stdin az kym nedojde na koniec suboru (EOF)
    Predavany parameter je 2D array char mesta
    Funkcia nam vracia pocet riadkov s textom a ignoruje prazdne riadky
*/
int citanie_suboru(char mesta[][MAX_CHAR_RIADOK])
{
    int pocet_miest = 0;
    while (scanf(" %250[^\n]", mesta[pocet_miest]) != EOF) // Nacitanie pomocou scanf
    {
        mesta[pocet_miest][strcspn(mesta[pocet_miest], "\r\n")] = 0; // Odstranenie \r (Windows) a \n z konca riadka

        if (strlen(mesta[pocet_miest]) > 0) // Overenie ci riadok nie je prazdny
        {
            for (int i = 0; mesta[pocet_miest][i] != '\0'; i++)
            {
                mesta[pocet_miest][i] = toupper(mesta[pocet_miest][i]); // Zmena po charakaktery na velke pismena
            }
            (pocet_miest)++;
        }
    }
    return pocet_miest; // Vratenie poctu neprazdnych riadkov
}

/*
    Funkcia zistenie_dlzky_vstupu sluzi na zistenie dlzky vstupu
    Predavany parameter je vstup a premenna len_vstupu ktoru nasledne vratime s konkretnym cislom, ktore nam udava dlzku vstupu
 */
int zistenie_dlzky_vstupu(char *vstup, int len_vstupu)
{
    len_vstupu = strlen(vstup);
    return len_vstupu;
}

char *premena_vstupu_na_velke_pismena(char *vstup, char *velky_vstup)
{
    int i = 0;
    while (vstup[i])
    {
        velky_vstup[i] = toupper(vstup[i]);
        i++;
    }
    velky_vstup[i] = '\0';
    return velky_vstup;
}

void zistenie_nasledujucich_znakov(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], char *velky_vstup, int dlzka_vstupu, char *nasledujuci_znak)
{
    int index_znaku = 0;
    for (int n = 0; n < pocet_miest; n++)
    {
        if (strncmp(mesta[n], velky_vstup, dlzka_vstupu) == 0)
        {
            if (mesta[n][dlzka_vstupu] != '\0')
            {
                nasledujuci_znak[index_znaku++] = mesta[n][dlzka_vstupu];
            }
        }
    }
}

void output(char mesta[][MAX_CHAR_RIADOK], int doplnenie_index, char *velky_vstup, int pocet_znakovych_zhod, char nasledujuci_znak[], int zhoda)
{
    if (strcmp(mesta[doplnenie_index], velky_vstup) != 0 && pocet_znakovych_zhod == 1)
    {
        printf("Found: %s\n", mesta[doplnenie_index]);
        return;
    }

    if (zhoda)
    {
        printf("Found: %s\n", velky_vstup);
    }
    if (nasledujuci_znak[0] != '\0')
    {
        if (nasledujuci_znak[0] == ' ')
        {
            printf("Enable: '%s'\n", nasledujuci_znak);
        }
        else
        {
            printf("Enable: %s\n", nasledujuci_znak);
        }
    }
    else if (!zhoda)
    {
        printf("Not Found\n");
    }
}

void filtrovanie_nasledujucich_znakov(char *nasledujuci_znak)
{
    int dlzka = strlen(nasledujuci_znak);
    for (int i = 0; i < dlzka; i++)
    {
        for (int j = i + 1; j < dlzka;)
        {
            if (nasledujuci_znak[i] == nasledujuci_znak[j])
            {
                for (int k = j; k < dlzka; k++)
                {
                    nasledujuci_znak[k] = nasledujuci_znak[k + 1];
                }
                dlzka--;
            }
            else
                j++;
        }
    }
}

int najdenie_zhody(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], char *velky_vstup)
{
    int zhoda = 0;
    for (int j = 0; j < pocet_miest; j++)
    {
        if (strcmp(mesta[j], velky_vstup) == 0)
        {
            zhoda = 1;
        }
    }
    return zhoda;
}

int spocitanie_zhod(int pocet_miest, char mesta[][MAX_CHAR_RIADOK], const char *velky_vstup, int dlzka_vstupu, int *doplnenie_index)
{
    int pocet_znakovych_zhod = 0;
    *doplnenie_index = -1;

    for (int i = 0; i < pocet_miest; i++)
    {
        if (strncmp(mesta[i], velky_vstup, dlzka_vstupu) == 0)
        {
            pocet_znakovych_zhod++;
            *doplnenie_index = i;
        }
    }
    return pocet_znakovych_zhod;
}

int main(int argc, char *argv[])
{
    // Inicializacia char vstup s hodnotou 1. argumentu
    char *vstup = argv[1];

    // Overenie na zaklade poctu argumentov
    if (argc < 2)
    {
        vstup = "";
    }
    else if (argc > 2)
    {
        printf("Enter 1 argument only\n");
        return 0;
    }

    // Inicializovanie premennych
    char mesta[MAX_MIEST][MAX_CHAR_RIADOK];
    int len_vstupu = 0;
    int dlzka_vstupu = zistenie_dlzky_vstupu(vstup, len_vstupu);
    char velky_vstup[dlzka_vstupu + 1];
    int pocet_miest = citanie_suboru(mesta);
    char nasledujuci_znak[MAX_MIEST];
    int doplnenie_index = -1;
    int pocet_znakovych_zhod = 0;

    premena_vstupu_na_velke_pismena(vstup, velky_vstup);
    zistenie_nasledujucich_znakov(pocet_miest, mesta, velky_vstup, dlzka_vstupu, nasledujuci_znak);
    filtrovanie_nasledujucich_znakov(nasledujuci_znak);
    zoradenie_moznych_znakov(nasledujuci_znak);

    int zhoda = najdenie_zhody(pocet_miest, mesta, velky_vstup);

    pocet_znakovych_zhod = spocitanie_zhod(pocet_miest, mesta, velky_vstup, dlzka_vstupu, &doplnenie_index);
    output(mesta, doplnenie_index, velky_vstup, pocet_znakovych_zhod, nasledujuci_znak, zhoda);

    return 0;
}