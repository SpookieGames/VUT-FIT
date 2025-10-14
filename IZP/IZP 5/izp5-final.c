//== knihovny ==
#include <stdio.h>
#include <math.h>

//== nove datove typy ==
typedef struct Sbod // definujeme strukturu jmenem "Sbod"
{                   // se dvema polozkami
    float x;        // polozka typu "float" jmenem "x"
    float y;        // polozka typu "float" jmenem "y"
} bod;              // deklarujeme jeji datovy typ "bod"

typedef struct Strojuhelnik // vytvarime strukturu
{                           //"Strojuhelnik" s polozkami
    bod A;                  //  bod "A"
    bod B;                  //  bod "B"
    bod C;                  //  bod "C"
} trojuhelnik;              // struktura je typu "trojuhelnik"

//== deklarace funkci ==
float vzdalenost(bod A, bod B);
float obvod(trojuhelnik T);
float obsah(trojuhelnik T);
bod teziste(trojuhelnik T);

//== definice funkci ==
float vzdalenost(bod A, bod B) // definice funkce "vzdalenost"
{
    float dx = A.x - B.x;           // pocitame rozdil na ose X
    float dy = A.y - B.y;           // pocitame rozdil na ose Y
    return sqrt(dx * dx + dy * dy); // pocitame Pythagorovu vetu
}

float obvod(trojuhelnik T)
{
    float a = vzdalenost(T.B, T.C);
    float b = vzdalenost(T.A, T.C);
    float c = vzdalenost(T.A, T.B);
    return a + b + c;
}

float obsah(trojuhelnik T)
{
    // Shoelace formula (https://en.wikipedia.org/wiki/Shoelace_formula)
    return 0.5 * fabs((T.A.x * T.B.y + T.B.x * T.C.y + T.C.x * T.A.y) - (T.A.y * T.B.x + T.B.y * T.C.x + T.C.y * T.A.x)); // fabs (https://cplusplus.com/reference/cmath/fabs/)
}

bod teziste(trojuhelnik T)
{ // https://www.cuemath.com/geometry/centroid-of-a-triangle/
    bod S;
    S.x = (T.A.x + T.B.x + T.C.x) / 3.0; // suradnica X
    S.y = (T.A.y + T.B.y + T.C.y) / 3.0; // suradnica Y
    return S;
}

int main()
{
    trojuhelnik T;
    scanf("%f %f", &T.A.x, &T.A.y);
    scanf("%f %f", &T.B.x, &T.B.y);
    scanf("%f %f", &T.C.x, &T.C.y);

    printf("Obvod = %.3f\n", obvod(T));
    printf("Obsah = %.3f\n", obsah(T));

    bod S = teziste(T);
    printf("Teziste = (%.3f, %.3f)\n", S.x, S.y);

    return 0;
}