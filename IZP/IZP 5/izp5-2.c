//== knihovny ==
#include <stdio.h>

//== deklarace funkci ==
void vypisPole(int delka, int pole[delka][delka]);
void naplnPole(int delka, int pole[delka][delka]);
void vypisHlavni(int delka, int pole[delka][delka]);
void vypisVedlejsi(int delka, int pole[delka][delka]);
void vyhledejCislo(int delka, int pole[delka][delka], int cislo);

//== definice funkci ==
void vypisPole(int delka, int pole[delka][delka])
{
    for (int radek = 0; radek < delka; radek++) // pro kazdy radek
    {
        for (int sloupec = 0; sloupec < delka; sloupec++) // pro kazdy sloupec
        {
            printf("%2i ", pole[radek][sloupec]); // vypisujeme polozku pole
        }
        printf("\n"); // vypisujeme konec radku
    }
}

void naplnPole(int delka, int pole[delka][delka])
{
    int hodnota = 0;
    for (int i = 0; i < delka; i++)
    {
        for (int j = 0; j < delka; j++)
        {
            pole[i][j] = hodnota++;
        }
    }
}

void vypisHlavni(int delka, int pole[delka][delka])
{
    for (int i = 0; i < delka; i++)
    {
        printf("%d ", pole[i][i]);
    }
    printf("\n");
}

void vypisVedlejsi(int delka, int pole[delka][delka])
{
    for (int i = 0; i < delka; i++)
    {
        printf("%d ", pole[i][delka - i - 1]);
    }
    printf("\n");
}

void vyhledejCislo(int delka, int pole[delka][delka], int cislo)
{
    for (int i = 0; i < delka; i++)
    {
        for (int j = 0; j < delka; j++)
        {
            if (pole[i][j] == cislo)
            {
                printf("Cislo %d nalezeno na souradnicich [%d][%d]\n", cislo, i, j);
                return;
            }
        }
    }
    printf("Cislo %d nebylo nalezeno\n", cislo);
}

int main()
{
    int pole[5][5] = {};
    printf("Prazdne pole:\n");
    vypisPole(5, pole);

    naplnPole(5, pole);
    printf("Naplnene pole:\n");
    vypisPole(5, pole);

    printf("Hlavni diagonala:\n");
    vypisHlavni(5, pole);

    printf("Vedlejsi diagonala:\n");
    vypisVedlejsi(5, pole);

    vyhledejCislo(5, pole, 14);
    vyhledejCislo(5, pole, 30);

    return 0;
}