//== knihovny ==
#include <stdio.h>
#include <math.h>

//== nove datove typy ==
typedef struct Sbod // definujeme strukturu jmenem "Sbod"
{                   // se dvema polozkami
    float x;        // polozka typu "float" jmenem "x"
    float y;        // polozka typu "float" jmenem "y"
} bod;              // deklarujeme jeji datovy typ "bod"

//== deklarace funkci ==
float vzdalenost(bod A, bod B);
float prumernaVzdalenost(int delka, bod pole[]);
void stredMnoziny(int delka, bod pole[], bod *S);
bod *nejblizsiBod(int delka, bod pole[], bod S);

//== definice funkci ==
float vzdalenost(bod A, bod B) // definice funkce "vzdalenost"
{
    float dx = A.x - B.x;           // pocitame rozdil na ose X
    float dy = A.y - B.y;           // pocitame rozdil na ose Y
    return sqrt(dx * dx + dy * dy); // pocitame Pythagorovu vetu
}

float prumernaVzdalenost(int delka, bod pole[])
{
    float celkova_vzdialenost = 0.0;
    float pocet_vzdialenosti = 0.0;

    for (int i = 0; i < delka; i++)
    {
        for (int j = i + 1; j < delka; j++)
        {
            celkova_vzdialenost += vzdalenost(pole[i], pole[j]);
            pocet_vzdialenosti++;
        }
    }

    float priemerna_vzdialenost = celkova_vzdialenost / pocet_vzdialenosti;
    return priemerna_vzdialenost;
}

void stredMnoziny(int delka, bod pole[], bod *S) // definice
{
    float sum_x = 0.0;
    float sum_y = 0.0;

    for (int i = 0; i < delka; i++)
    {
        sum_x += pole[i].x;
        sum_y += pole[i].y;
    }

    (*S).x = sum_x / delka;
    S->y = sum_y / delka;
}

bod *nejblizsiBod(int delka, bod pole[], bod S) // definice
{
    bod *najblizsi_bod = &pole[0];
    float najmensia_vzdialenost = vzdalenost(pole[0], S);

    for (int i = 1; i < delka; i++)
    {
        float aktualna_vzdialenost = vzdalenost(pole[i], S);

        if (aktualna_vzdialenost < najmensia_vzdialenost)
        {
            najmensia_vzdialenost = aktualna_vzdialenost;
            najblizsi_bod = &pole[i];
        }
    }
    return najblizsi_bod;
}

int main() // zacatek programu
{
    bod M1[4] = {{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}, {2.0, 0.0}}; // prvni mnozina
    bod M2[4] = {{0.0, 0.0}, {1.0, 3.0}, {3.0, 2.0}, {3.0, 0.0}}; // druha mnozina
    float d;                                                      // prumerna vzdalensot
    bod S;                                                        // stred mnoziny
    bod *N;                                                       // ukazatel na nejblizsi bod

    printf("Prvni mnozina:\n");
    d = prumernaVzdalenost(4, M1);                           // pocitame prumernou vzdalenost
    printf("Prumerna vzdalenost je %f\n", d);                // vypis
    stredMnoziny(4, M1, &S);                                 // pocitame souradnice stredu
    printf("Souradnice stredu  (%.2f, %.2f)\n", S.x, S.y);   // vypis
    N = nejblizsiBod(4, M1, S);                              // zjistujeme neblizsi bod
    printf("Nejblizsi prvek je (%.2f, %.2f)\n", N->x, N->y); // vypis

    printf("Druha mnozina:\n");
    d = prumernaVzdalenost(4, M2);                           // pocitame prumernou vzdalenost
    printf("Prumerna vzdalenost je %f\n", d);                // vypis
    stredMnoziny(4, M2, &S);                                 // pocitame souradnice stredu
    printf("Souradnice stredu  (%.2f, %.2f)\n", S.x, S.y);   // vypis
    N = nejblizsiBod(4, M2, S);                              // zjistujeme neblizsi bod
    printf("Nejblizsi prvek je (%.2f, %.2f)\n", N->x, N->y); // vypis

    return 0; // konec programu
}