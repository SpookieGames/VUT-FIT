#include <stdio.h>
#include <math.h>

typedef struct
{
    float x;
    float y;
} bod;

float vzdalenostDvouBodu(bod A, bod B);
float prumernaVzdalenost(int delka, bod pole[]);
void stredMnoziny(int delka, bod pole[], bod *S);
bod *nejblizsiBod(int delka, bod pole[], bod S);

float vzdalenostDvouBodu(bod A, bod B)
{
    return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

float prumernaVzdalenost(int delka, bod pole[])
{
    float total_distance = 0.0;
    int pair_count = 0;

    for (int i = 0; i < delka; i++)
    {
        for (int j = i + 1; j < delka; j++)
        {
            total_distance += vzdalenostDvouBodu(pole[i], pole[j]);
            pair_count++;
        }
    }
    float ap = total_distance / pair_count;

    return ap;
}

void stredMnoziny(int delka, bod pole[], bod *S)
{
    float sum_x = 0.0;
    float sum_y = 0.0;

    for (int i = 0; i < delka; i++)
    {
        sum_x += pole[i].x;
        sum_y += pole[i].y;
    }
    S->x = sum_x / delka;
    S->y = sum_y / delka;
}

bod *nejblizsiBod(int delka, bod pole[], bod S)
{
    bod *closest_point = &pole[0];
    float min_distance = vzdalenostDvouBodu(pole[0], S);

    for (int i = 1; i < delka; i++)
    {
        float current_distance = vzdalenostDvouBodu(pole[i], S);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            closest_point = &pole[i];
        }
    }

    return closest_point;
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
