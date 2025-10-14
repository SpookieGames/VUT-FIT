//== knihovny ==
#include <stdio.h>

//== definice funkci ==
int main(int argc, char *argv[]) // main s rozsirenou hlavickou
{
    for (int i = 1; i < argc; i++) // cyklus pres vsechny argumenty programu
    {
        FILE *vstup = fopen(argv[i], "r"); // otevreni souboru
        if (vstup != NULL)                 // pokud se otevreni podarilo
        {
            int delka = 0;                         // pocitadlo znaku
            char x;                                // znak kam budeme nacitat
            while (fscanf(vstup, "%c", &x) != EOF) // cyklus cteni celeho sobuboru
            {
                delka++; // inkrementace pocitadla znaku
            }
            fclose(vstup);
            printf("Subor %s ma %d znakov.\n", argv[i], delka); // uzavreni souboru a vypis
        }
        else // jinak (pokud otevreni selhalo)
        {
            fprintf(stderr, "Nepodarilo sa otvorit subor %s\n", argv[i]);
        }
    }
    return 0; // konec programu
}