//== knihovny ==
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//== nove datove typy ==
typedef struct Svektor
{
    int delka; // delka pole
    int *pole; // ukazatel na dynamicky alokovane pole
} vektor;

//== deklarace funkci ==
vektor *alokujVektor(int delka);
void nactiVektor(vektor *A);
void vypisVektor(vektor *A);
bool jeMnozina(vektor *A);
vektor *konkatenace(vektor *A, vektor *B);
void uvolniVektor(vektor *A);
vektor *prunik(vektor *A, vektor *B);

//== definice funkci ==
vektor *alokujVektor(int delka)
{
    vektor *A = malloc(sizeof(vektor)); // alokujeme mnozinu
    if (A != NULL)                      // pokud alokace neselhala
    {
        A->delka = delka;                      // nastavujeme delku mnoziny
        A->pole = malloc(sizeof(int) * delka); // alokujeme pole mnoziny
    }
    return A; // vracime mnozinu
}

void nactiVektor(vektor *A)
{

    for (int i = 0; i < A->delka; i++)
    {
        scanf("%d", &(A->pole[i]));
    }
}

void vypisVektor(vektor *A)
{
    for (int i = 0; i < A->delka; i++)
    {
        printf("%d ", A->pole[i]);
    }
    printf("\n");
}

bool jeMnozina(vektor *A)
{
    for (int i = 0; i < A->delka; i++)
    {
        for (int j = i + 1; j < A->delka; j++)
        {
            if (A->pole[i] == A->pole[j])
                return false;
        }
    }
    return true;
}

vektor *konkatenace(vektor *A, vektor *B)
{
    int delka = A->delka + B->delka;
    vektor *K = alokujVektor(delka);

    int delka_A = A->delka;
    for (int i = 0; i < delka_A; i++)
    {
        K->pole[i] = A->pole[i];
    }
    for (int j = delka_A; j < delka; j++)
    {
        K->pole[j] = B->pole[j - delka_A];
    }
    return K;
}

void uvolniVektor(vektor *A)
{
    if (A != NULL)
    {
        if (A->pole != NULL)
        {
            free(A->pole);
        }
        free(A);
    }
}

vektor *prunik(vektor *A, vektor *B)
{
    int velkost_pruniku = 0;
    for (int i = 0; i < A->delka; i++)
    {
        for (int j = 0; j < B->delka; j++)
        {
            if (A->pole[i] == B->pole[j])
            {
                velkost_pruniku++;
            }
        }
    }

    int idx_pola_P = 0;
    vektor *P = alokujVektor(velkost_pruniku);
    for (int i = 0; i < A->delka; i++)
    {
        for (int j = 0; j < B->delka; j++)
        {
            if (A->pole[i] == B->pole[j])
            {
                P->pole[idx_pola_P] = B->pole[j];
                idx_pola_P++;
            }
        }
    }
    return P;
}

int main()
{
    vektor *A = alokujVektor(4);                                      // alokujeme mnozinu A (a jeji pole)
    vektor *B = alokujVektor(3);                                      // alokujeme mnozinu B (a jeji pole)
    if (A == NULL || A->pole == NULL || B == NULL || B->pole == NULL) // osetreni alokace
    {
        fprintf(stderr, "Alokace selhala\n");
        return 1;
    }

    nactiVektor(A); // do mnoziny A nacitame vstup
    nactiVektor(B); // do mnoziny B nacitame vstup
    printf("Vektor A:\n");
    vypisVektor(A); // vypisujeme mnozinu A
    printf("Vektor B:\n");
    vypisVektor(B); // vypisujeme mnozinu A

    if (!jeMnozina(A) || !jeMnozina(B)) // osetreni vstupu
    {
        fprintf(stderr, "Zadane vektory nejsou mnozinami\n");
        return 2;
    }
    printf("Zadane vektory jsou mnozinami\n");

    vektor *C = konkatenace(A, B);    // C je konkatenaci A a B
    if (C == NULL || C->pole == NULL) // osetreni alokace
    {
        fprintf(stderr, "Alokace konkatenace selhala\n");
        return 3;
    }
    printf("Konkatenace:\n");
    vypisVektor(C);  // vypisujeme mnozinu C
    uvolniVektor(C); // uvolnujeme mnozinu C

    C = prunik(A, B);                 // C je prunikem A a B
    if (C == NULL || C->pole == NULL) // osetreni alokace
    {
        fprintf(stderr, "Alokace pruniku selhala\n");
        return 4;
    }
    printf("Prunik:\n");
    vypisVektor(C); // vypisujeme mnozinu C

    uvolniVektor(C); // uvolnujeme vektory
    uvolniVektor(B); // uvolnujeme vektory
    uvolniVektor(A); // uvolnujeme vektory
    return 0;
}