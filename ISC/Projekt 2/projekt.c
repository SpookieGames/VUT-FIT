#include <stdio.h>
#include <string.h>

int neparny_palindrom(char vstup[], int *zaciatok_neparny);
int parny_palindrom(char vstup[], int *zaciatok_parny);
void print(int maximalna_dlzka, int zaciatok, char vstup[]);

int neparny_palindrom(char vstup[], int *zaciatok_neparny)
{
    int dlzka_vstupu = strlen(vstup);
    int maximalna_dlzka_neparny = 1;
    for (int i = 0; i < dlzka_vstupu; i++)
    {
        int vlavo = i;
        int vpravo = i;
        while (vpravo < dlzka_vstupu && vlavo >= 0 && vstup[vlavo] == vstup[vpravo])
        {
            int aktualna_dlzka = (vpravo - vlavo) + 1;
            if (aktualna_dlzka > maximalna_dlzka_neparny)
            {
                maximalna_dlzka_neparny = aktualna_dlzka;
                *zaciatok_neparny = vlavo;
            }
            vlavo--;
            vpravo++;
        }
    }
    return maximalna_dlzka_neparny;
}

int parny_palindrom(char vstup[], int *zaciatok_parny)
{
    int dlzka_vstupu = strlen(vstup);
    int maximalna_dlzka_parny = 0;
    for (int i = 0; i < dlzka_vstupu; i++)
    {
        int vlavo = i;
        int vpravo = i + 1;
        while (vpravo < dlzka_vstupu && vlavo >= 0 && vstup[vlavo] == vstup[vpravo])
        {
            int aktualna_dlzka = (vpravo - vlavo) + 1;
            if (aktualna_dlzka > maximalna_dlzka_parny)
            {
                maximalna_dlzka_parny = aktualna_dlzka;
                *zaciatok_parny = vlavo;
            }
            vlavo--;
            vpravo++;
        }
    }

    return maximalna_dlzka_parny;
}

void print(int maximalna_dlzka, int zaciatok, char vstup[])
{
    char vysledny_palindrom[maximalna_dlzka + 1];
    for (int i = 0; i < maximalna_dlzka; i++)
    {
        vysledny_palindrom[i] = vstup[zaciatok + i];
    }
    printf("Najdlhsi palindrom je: %s\n", vysledny_palindrom);
}

int main(int argc, char *argv[])
{
    char *vstup = argv[1];
    if (argc != 2)
    {
        printf("Zadajte 1 argument!\n");
        return 1;
    }
    int zaciatok_neparny = 0;
    int zaciatok_parny = 0;
    int maximalna_dlzka = 0;
    int zaciatok = 0;

    int maximalna_dlzka_neparny = neparny_palindrom(vstup, &zaciatok_neparny);
    int maximalna_dlzka_parny = parny_palindrom(vstup, &zaciatok_parny);

    if (maximalna_dlzka_neparny > maximalna_dlzka_parny)
    {
        maximalna_dlzka = maximalna_dlzka_neparny;
        zaciatok = zaciatok_neparny;
    }
    else
    {
        maximalna_dlzka = maximalna_dlzka_parny;
        zaciatok = zaciatok_parny;
    }

    print(maximalna_dlzka, zaciatok, vstup);

    return 0;
}