//== knihovny ==
#include <stdio.h>
#include <stdlib.h>

//== nove datove typy ==
typedef struct Svektor // deklarujeme datovy typ pro strukturu
{                      // jmenem "Svektor" se dvema polozkami
    int delka;         // pocet polozek
    int *pole;         // dynamicky alokovane pole
} vektor;              // jmeno tohoto typu je "vektor"

//== deklarace funkci ==
void vypisVektor(vektor *A);
void pridejPosledni(vektor *A, int cislo);
void odeberPosledni(vektor *A);
void odeberVsechny(vektor *A);
void pridejPrvni(vektor *A, int cislo);
void odeberIndex(vektor *A, int index);

//== definice funkci ==
void vypisVektor(vektor *A)
{
    printf("Vektor: ");
    for (int i = 0; i < A->delka; i++)
    {
        printf("%d ", A->pole[i]);
    }
    printf("\n");
}

void pridejPosledni(vektor *A, int cislo)
{
    A->delka++;
    A->pole = realloc(A->pole, A->delka * sizeof(int));
    if (A->pole != NULL)
    {
        A->pole[A->delka - 1] = cislo;
    }
    else
    {
        free(A->pole);
    }
}

void odeberPosledni(vektor *A)
{
    if (A->delka > 0)
    {
        A->delka--;
        A->pole = realloc(A->pole, A->delka * sizeof(int));
        if (A->pole == NULL)
        {
            free(A->pole);
        }
    }
}

void odeberVsechny(vektor *A)
{
    if (A->pole != NULL)
    {
        free(A->pole);
    }
    A->delka = 0;
    A->pole = NULL;
}

void pridejPrvni(vektor *A, int cislo)
{
    A->delka++;
    A->pole = realloc(A->pole, A->delka * sizeof(int));
    if (A->pole != NULL)
    {
        for (int i = A->delka - 1; i > 0; i--)
        {
            A->pole[i] = A->pole[i - 1];
        }
        A->pole[0] = cislo;
    }
    else
    {
        free(A->pole);
    }
}

void odeberIndex(vektor *A, int index)
{
    if (index >= 0 && index < A->delka)
    {
        for (int i = index; i < A->delka - 1; i++)
        {
            A->pole[i] = A->pole[i + 1];
        }
        A->delka--;
        A->pole = realloc(A->pole, A->delka * sizeof(int));
        if (A->pole == NULL)
        {
            free(A->pole);
        }
    }
}

int main() // zacatek programu
{
    vektor A = {0, NULL}; // vytvarime vektor
                          //"A" je automaticky alokovana struktura
                          // obsahujici dynamicky alokovane pole
    vypisVektor(&A);
    pridejPosledni(&A, 10); // pridavame posledni prvek
    vypisVektor(&A);
    pridejPosledni(&A, 20); // pridavame posledni prvek
    vypisVektor(&A);
    pridejPosledni(&A, 30); // pridavame posledni prvek
    vypisVektor(&A);
    odeberPosledni(&A); // odebirame posledni prvek
    vypisVektor(&A);
    odeberVsechny(&A); // odebirame vsechny prvky
    vypisVektor(&A);
    odeberPosledni(&A); // odebirame neexistujici prvek
    vypisVektor(&A);
    pridejPrvni(&A, 40); // pridavame prvni prvek
    vypisVektor(&A);
    pridejPrvni(&A, 50); // pridavame prvni prvek
    vypisVektor(&A);
    pridejPrvni(&A, 60); // pridavame prvni prvek
    vypisVektor(&A);
    odeberIndex(&A, -1); // odebirame neexistujici prvek
    vypisVektor(&A);
    odeberIndex(&A, 3); // odebirame neexistujici prvek
    vypisVektor(&A);
    odeberIndex(&A, 1); // odebirame konkretni prvek
    vypisVektor(&A);
    odeberVsechny(&A); // odebirame vsechny prvky
    vypisVektor(&A);
    return 0; // konec programu
}