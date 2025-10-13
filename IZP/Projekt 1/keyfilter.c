// Nacitanie potrebnych kniznic
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Deklaracia pouzitych funkcii
void zoradenie_moznych_znakov(char znak[]);
int citanie_suboru(char mesta[][251]);
int zistenie_dlzky_vstupu(char *vstup, int len_vstupu);
char *premena_vstupu_na_velke_pismena(char *vstup, char *velky_vstup);
void zistenie_nasledujucich_znakov(int pocet_miest, char mesta[][251], char *velky_vstup, int dlzka_vstupu, char *nasledujuci_znak);
void filtrovanie_nasledujucich_znakov(char *nasledujuci_znak);
void output(char mesta[][251], int doplnenie_index, char *velky_vstup, int pocet_znakovych_zhod, char nasledujuci_znak[], int zhoda);
int najdenie_zhody(int pocet_miest, char mesta[][251], char *velky_vstup);
int spocitanie_zhod(int pocet_miest, char mesta[][251], const char *velky_vstup, int dlzka_vstupu, int *doplnenie_index);

/*
    Funkcia 'zoradenie_moznych_znakov' sluzi na abecedne zoradnie znakov, ktore budu nasledne vypisane ako mozne znaky (Enable: MOZNE ZNAKY) podla algoritmu Selection Sort
    Parameter predavany funkcii je nasledujuci_znak[251]
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
            if (znak[j] < znak[index_minima]) // Zistenie ci aktualny znak je mensi ako aktualny najmensi znak
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
    Funkcia 'citanie_suboru' sluzi na nacitanie textu riadok po riadku pomocou stdin az kym nedojde na koniec suboru (EOF)
    Predavany parameter je 2D retazec mesta
    Funkcia nam vracia pocet riadkov s textom (ako integer) a ignoruje prazdne riadky
*/
int citanie_suboru(char mesta[][251])
{
    int pocet_miest = 0;
    while (scanf(" %250[^\n]", mesta[pocet_miest]) != EOF) // Nacitanie pomocou scanf, kym nebude zadany enter alebo nedosiahne pocet znakov
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
    Funkcia 'zistenie_dlzky_vstupu' sluzi na zistenie dlzky vstupu
    Predavany parameter je vstup a premenna len_vstupu ktoru nasledne vratime s konkretnym cislom (integer), ktore nam udava dlzku vstupu
 */
int zistenie_dlzky_vstupu(char *vstup, int len_vstupu)
{
    len_vstupu = strlen(vstup);
    return len_vstupu;
}

/*
    Funkcia 'premena_vstupu_na_velke_pismena' sluzi na premenu vstupu na velke pisemno, pismeno po pismene
    Predavane parametre su *vstup a *velky_vstup
    Funkcia vracia pointer na retazec znakov velky_vstup
*/
char *premena_vstupu_na_velke_pismena(char *vstup, char *velky_vstup)
{
    int i = 0;
    while (vstup[i])
    {
        velky_vstup[i] = toupper(vstup[i]); // Zmena na velke pismena pomocou toupper
        i++;
    }
    velky_vstup[i] = '\0';
    return velky_vstup;
}

/*
    Funkcia 'zistenie_nasledujucich_znakov' sluzi na zistenie nasledujucich znakov
*/
void zistenie_nasledujucich_znakov(int pocet_miest, char mesta[][251], char *velky_vstup, int dlzka_vstupu, char *nasledujuci_znak)
{
    int index_znaku = 0;
    for (int n = 0; n < pocet_miest; n++)
    {
        if (strncmp(mesta[n], velky_vstup, dlzka_vstupu) == 0) // Overuje ci sa pismeno z retazca mesta == pismenu z retazca velky_vstup az do dlzky vstupu
        {
            if (mesta[n][dlzka_vstupu] != '\0')                           //
            {                                                             //
                nasledujuci_znak[index_znaku++] = mesta[n][dlzka_vstupu]; // Ak sa znak != koncovemu znaku '\0' tak zapise tento znak do retazca nasleducuji_znak
            }
        }
    }
}

/*
    Funkcia 'output' sluzi na vypisanie vysledkov na zaklade splnenia podmienok
*/
void output(char mesta[][251], int doplnenie_index, char *velky_vstup, int pocet_znakovych_zhod, char nasledujuci_znak[], int zhoda)
{
    if (strcmp(mesta[doplnenie_index], velky_vstup) != 0 && pocet_znakovych_zhod == 1) // Ak sa mesta[index] presne nezhoduju so vstupom a nasla sa presne 1 znakova zhoda
    {                                                                                  // tak vieme doplnit cely nazov nakolko existuje iba 1 mozna zhoda
        printf("Found: %s\n", mesta[doplnenie_index]);
        return;
    }

    if (zhoda)
    {
        printf("Found: %s\n", velky_vstup); // Ak sa nasla presna zhoda tak sa vypise
    }
    if (nasledujuci_znak[0] != '\0')
    {
        if (nasledujuci_znak[0] == ' ') // Ak je nasledujuci znak medzera tak sa vypise ako ' '
        {
            printf("Enable: '%s'\n", nasledujuci_znak);
        }
        else
        {
            printf("Enable: %s\n", nasledujuci_znak);
        }
    }
    else if (!zhoda) // Ak sa nenasla ziadna zhoda tak sa vypise "Not Found"
    {
        printf("Not Found\n");
    }
}

/*
    Funkcia 'filtrovanie_nasledujucich_znakov' sluzi na odstranenie duplikatov z retazca nasleduci_znak
    Predavame parameter *nasledujuci_znak
*/
void filtrovanie_nasledujucich_znakov(char *nasledujuci_znak)
{
    int dlzka = strlen(nasledujuci_znak); // Zistujeme dlzku retazca
    for (int i = 0; i < dlzka; i++)
    {
        for (int j = i + 1; j < dlzka;)
        {
            if (nasledujuci_znak[i] == nasledujuci_znak[j]) // Zistenie zhody
            {
                for (int k = j; k < dlzka; k++)
                {
                    nasledujuci_znak[k] = nasledujuci_znak[k + 1];
                }
                dlzka--; // Pri odstraneni duplikatu sa celkova dlzka znizi
            }
            else
                j++; // Pri nenajdeni duplikatu sa inkrementuje a posuva na dalsi znak
        }
    }
}

/*
    Funkcia 'najdenie_zhody' sluzi na najdenie zhody
    Funkcia vracia hodnotu zhoda ktora udava, ci sa zhoda nasla (1) alebo nenasla (0)
*/
int najdenie_zhody(int pocet_miest, char mesta[][251], char *velky_vstup)
{
    int zhoda = 0;
    for (int j = 0; j < pocet_miest; j++)
    {
        if (strcmp(mesta[j], velky_vstup) == 0) // Porovnanie mesta a vstupu
        {
            zhoda = 1;
        }
    }
    return zhoda;
}

/*
    Funkcia 'spocitanie_zhod' sluzi na spocitanie zhod a ziskanie indexu poslednej zhody, ktore su potrebne na neskorsi vypis moznej zhody
    Funkcia vracia celkovy pocet zhod ako integer
 */
int spocitanie_zhod(int pocet_miest, char mesta[][251], const char *velky_vstup, int dlzka_vstupu, int *doplnenie_index)
{
    int pocet_znakovych_zhod = 0;
    *doplnenie_index = -1;

    for (int i = 0; i < pocet_miest; i++)
    {
        if (strncmp(mesta[i], velky_vstup, dlzka_vstupu) == 0) // Overuje ci sa pismeno z retazca mesta == pismenu z retazca velky_vstup az do dlzky vstupu
        {                                                      //
            pocet_znakovych_zhod++;                            // Pri najdeni zhody sa inkrementuje pocet_znakovych_zhod
            *doplnenie_index = i;                              // Do doplenenie_index sa zapise index poslednej zhody
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
    char mesta[100][251];
    int len_vstupu = 0;
    int dlzka_vstupu = zistenie_dlzky_vstupu(vstup, len_vstupu);
    char velky_vstup[dlzka_vstupu + 1];
    int pocet_miest = citanie_suboru(mesta);
    char nasledujuci_znak[100];
    int doplnenie_index = -1;
    int pocet_znakovych_zhod = 0;

    premena_vstupu_na_velke_pismena(vstup, velky_vstup);                                                     //
    zistenie_nasledujucich_znakov(pocet_miest, mesta, velky_vstup, dlzka_vstupu, nasledujuci_znak);          //
    filtrovanie_nasledujucich_znakov(nasledujuci_znak);                                                      //
    zoradenie_moznych_znakov(nasledujuci_znak);                                                              //
                                                                                                             // Volanie funkcii v spravnom poradi
    int zhoda = najdenie_zhody(pocet_miest, mesta, velky_vstup);                                             //
                                                                                                             //
    pocet_znakovych_zhod = spocitanie_zhod(pocet_miest, mesta, velky_vstup, dlzka_vstupu, &doplnenie_index); //
    output(mesta, doplnenie_index, velky_vstup, pocet_znakovych_zhod, nasledujuci_znak, zhoda);              //

    return 0;
}