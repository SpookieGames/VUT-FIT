#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Sflow
{
    int ID; // flow ID
    char src_ip[16];
    char dst_ip[16];
    int b;   // total_bytes
    int t;   // flow_duration
    float d; // avg_interarrival_time
    float s; // total_bytes / packet_count
} flow;

typedef struct Scluster
{
    int size; // Velkost clustera (kolko ma v sebe indexov)
    // int capacity;   // Kapacita (kolko indexov sa do clustera celkovo zmesti)
    int *flow_idxs; // Ukazatel na zaciatok dynamickeho pola
} cluster;

typedef struct Sweights
{
    int n;
    float wb, wt, wd, ws; // vahy
} weights;

// Deklaracia funkcii
void id_sorting(cluster *cluster_array, flow *flow_array);
void single_linkage(cluster *cluster_array, int count, flow *flow_array, weights W);
void combine_clusters(cluster *A, cluster *B);
void free_cluster_idxs(cluster *cluster_array, int count);
cluster *allocate_clusters(int count);
double calculate_cluster_distance(cluster *A, cluster *B, flow *flow_array, weights W);
double calculate_distance(flow A, flow B, weights);
void load_weights(char *argv[], weights *W);
flow *load_file(char *filename, int *count_out);
int verify_arguments(int argc, char *argv[], weights *W);
void output(cluster *cluster_array, flow *flow_array);

// Funkcia na nacitanie vah do struktury
void load_weights(char *argv[], weights *W)
{
    W->wb = atof(argv[3]); // Mozeme pouzit atof a atoi nakolko sme vstup uz overili
    W->wt = atof(argv[4]); //
    W->wd = atof(argv[5]); //
    W->ws = atof(argv[6]); //
    W->n = atoi(argv[2]);  //
}

// Funkcia na uvolnnie pamate v clusteroch
void free_cluster_idxs(cluster *cluster_array, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (cluster_array[i].size > 0)
        {
            free(cluster_array[i].flow_idxs);
        }
    }
}

// Funkcia na najdenie najmensej vzdialenosti medzi clustermi
double calculate_cluster_distance(cluster *A, cluster *B, flow *flow_array, weights W)
{
    double min_distance = INFINITY; // Inicializujeme minimalnu vzdialenost ako nekonecno aby sme zabranili ze nieco bude vacsie ako min_distance
    for (int i = 0; i < A->size; i++)
    {
        int idx_a = A->flow_idxs[i];
        for (int j = 0; j < B->size; j++)
        {
            int idx_b = B->flow_idxs[j];
            double current_distance = calculate_distance(flow_array[idx_a], flow_array[idx_b], W);
            if (current_distance < min_distance) // Zistujeme ci je aktualna vzdialenost mensia ako najdena minimalna vzdialenost
            {
                min_distance = current_distance;
            }
        }
    }
    return min_distance;
}

// Single linkage algoritmus
void single_linkage(cluster *cluster_array, int count, flow *flow_array, weights W)
{
    int current_cluster_ammount = count; // Inicializujeme aktualny pocet clusterov na count nakolko sme si spravili funkciu v ktorej vytvorime rovnaky pocet clusterov ako flows
    if (W.n == 0)                        // Ak je N 0 co znamena ze N nebolo zadane a mame vypisat vsetky clustery tak preskocime cely single_linkage algoritmus
    {
        return;
    }
    while (current_cluster_ammount > W.n) // Cyklus ktory bezi pokial nie je aktualny pocet clusterov rovnaky ako N
    {
        double min_distance = INFINITY; // Inicializujeme minimalnu vzdialenost ako nekonecno aby sme zabranili ze nieco bude vacsie ako min_distance rovnako ako pri calculate_cluster_distance
        int idx_a = -1;                 // Lokalna premenna pre index clusteru A
        int idx_b = -1;                 // Lokalna premenna pre index clusteru B
        for (int a = 0; a < count; a++)
        {
            if (cluster_array[a].size == 0) // Prazdne clustery preskocime
            {
                continue;
            }
            for (int b = a + 1; b < count; b++)
            {
                if (cluster_array[b].size == 0) // Prazdne clustery preskocime
                {
                    continue;
                }
                double distance = calculate_cluster_distance(&cluster_array[a], &cluster_array[b], flow_array, W); // Vypocitame vzdialenost dvoch clusterov
                if (distance < min_distance)
                {
                    min_distance = distance; // Ak je najmensia vzdialenost mensia ako aktualna vzdialenost tak nastavime najmensiu vzdialenost ako aktualnu vzdialenost
                    idx_a = a;               // Ulozime si index A
                    idx_b = b;               // Ulozime si index B
                }
            }
        }

        // temp
        // Verze pro výpis skutečného Flow ID (např. 10, 11)
        printf("Slucuji cluster s ID %d a ID %d (vzdalenost: %lf)\n", flow_array[cluster_array[idx_a].flow_idxs[0]].ID, flow_array[cluster_array[idx_b].flow_idxs[0]].ID, min_distance);
        //
        //

        combine_clusters(&cluster_array[idx_a], &cluster_array[idx_b]); // Skombinujeme clustery s najdenymi indexami
        current_cluster_ammount--;                                      // Po skombinovani zmensime pocet o 1
    }
}

// Funkcia na nacitanie dat zo suboru
flow *load_file(char *filename, int *count_out)
{
    int count, flow_id, flow_duration, total_bytes, packet_count; // Docasne lokalne premenne
    char src_ip[16];                                              //
    char dst_ip[16];                                              //
    float avg_interarrival;                                       //
    FILE *f = fopen(filename, "r");                               // Otvorenie suboru na citanie
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    fscanf(f, "count=%d", &count); // Nacitanie poctu flows z 1. riadku
    if (count < 0)
    {
        fprintf(stderr, "Count cannot be negative\n");
        fclose(f);
        return NULL;
    }

    flow *flow_array = malloc(count * sizeof(flow)); // Alokovanie pamate pre vsetky flow
    if (flow_array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(f);
        return NULL;
    }
    // Cyklus na populovanie flow_array
    for (int i = 0; i < count; i++)
    {
        if (fscanf(f, "%d %15s %15s %d %d %d %f", &flow_id, src_ip, dst_ip, &total_bytes, &flow_duration, &packet_count, &avg_interarrival) != 7) // Pouzijeme fscanf a rovno overime ci je pocet nacitanych hodnot spravny
        {
            fprintf(stderr, "Count is incorrect or %s has invalid data\n", filename);
            fclose(f);
            free(flow_array);
            return NULL;
        }
        flow_array[i].ID = flow_id;           // Odovzdanie nacitanych hodnot do flow_array
        strcpy(flow_array[i].src_ip, src_ip); //
        strcpy(flow_array[i].dst_ip, dst_ip); //
        flow_array[i].b = total_bytes;        //
        flow_array[i].t = flow_duration;      //
        flow_array[i].d = avg_interarrival;   //

        if (packet_count <= 0) // Neda sa delit nulou a zaporny pocet paketov nedava zmysel
        {
            fprintf(stderr, "Packet count cannot be 0 or negative");
            fclose(f);
            free(flow_array);
            return NULL;
        }
        else
        {
            flow_array[i].s = ((float)total_bytes / packet_count);
        }
        // temporary
        printf("%d %s %s %d %d %d %f\n", flow_id, src_ip, dst_ip, total_bytes, flow_duration, packet_count, avg_interarrival);
    }
    fclose(f);
    *count_out = count;
    return flow_array;
}

// Funkcia na skombinovanie dvoch clusterov do jedneho
void combine_clusters(cluster *A, cluster *B)
{
    A->flow_idxs = realloc(A->flow_idxs, ((A->size + B->size) * sizeof(int)));
    if (A->flow_idxs == NULL)
    {
        fprintf(stderr, "Failed to reallocate memory");
        free(A->flow_idxs);
        free(B->flow_idxs);
        return;
    }
    for (int i = A->size; i < (A->size + B->size); i++)
    {
        A->flow_idxs[i] = B->flow_idxs[i - A->size];
    }
    A->size = A->size + B->size; // Zvacsi velkost v A
    B->size = 0;                 // Nastavi velkost v B na 0 "deaktivuje cluster"
    free(B->flow_idxs);
}

// Funkcia ktora vytvori pocet clusterov = poctu flows
cluster *allocate_clusters(int count)
{
    cluster *cluster_array = malloc(count * sizeof(cluster)); // Alokacia pamate pre count clusterov
    if (cluster_array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        return NULL;
    }
    for (int i = 0; i < count; i++)
    {
        cluster_array[i].flow_idxs = malloc(sizeof(int)); // Zatial nam staci pole pre 1 integer
        if (cluster_array[i].flow_idxs == NULL)
        {
            fprintf(stderr, "Failed to allocate memory\n");
            return NULL;
        }
        // cluster_array[i].capacity = 1;     // Celkova kapacita clusteru
        cluster_array[i].size = 1;         // Pociatocnu velkost dame na 1 nakolko budeme mat ulozeny iba 1 idx
        cluster_array[i].flow_idxs[0] = i; // Na prvu poziciu v poli ulozime i az do count-1
    }
    return cluster_array;
}

// Funkcia na overenie IP adries. IP nesmie mat cislo vacsie ako 255 a mensie ako 0 medzi bodkami. Pouzil som sscanf co je posobny ako fscanf ale namiesto suboru cita z textoveho retazca
int verify_ips(int count, flow *flow_array)
{
    int as, bs, cs, ds; // Lokalne premenna na pracu s sscanf
    int ad, bd, cd, dd; // Lokalne premenna na pracu s sscanf
    for (int i = 0; i < count; i++)
    {
        if (sscanf(flow_array[i].src_ip, "%d.%d.%d.%d", &as, &bs, &cs, &ds) != 4) // sscanf vracia pocet nacitanych znakov
        {
            fprintf(stderr, "Invalid IP address\n");
            return 1;
        }
        else if (as < 0 || as > 255 || bs < 0 || bs > 255 || cs < 0 || cs > 255 || ds < 0 || ds > 255)
        {
            fprintf(stderr, "Invalid IP address\n");
            return 1;
        }
        if (sscanf(flow_array[i].dst_ip, "%d.%d.%d.%d", &ad, &bd, &cd, &dd) != 4)
        {
            fprintf(stderr, "Invalid IP address\n");
            return 1;
        }
        else if (ad < 0 || ad > 255 || bd < 0 || bd > 255 || cd < 0 || cd > 255 || dd < 0 || dd > 255)
        {
            fprintf(stderr, "Invalid IP address\n");
            return 1;
        }
    }
    return 0;
}

// Funkcia na vypocitanie vazenej Euklidovkej vzdialenosti medzi dvoma flowmi
double calculate_distance(flow A, flow B, weights W)
{
    double distance;
    float difference_b = A.b - B.b;                                                                                                                   // Lokalne premenne pre zmensenie vzorca
    float difference_t = A.t - B.t;                                                                                                                   //
    float difference_d = A.d - B.d;                                                                                                                   //
    float difference_s = A.s - B.s;                                                                                                                   //
    distance = sqrt(((W.wb * pow(difference_b, 2)) + (W.wt * pow(difference_t, 2)) + (W.wd * pow(difference_d, 2)) + (W.ws * pow(difference_s, 2)))); // Vzorec pre vazenu Euklidovsku vzdialenost
    return distance;
}

// Funkcia na verifikaciu platnosti zadanych argumentov
int verify_arguments(int argc, char *argv[], weights *W)
{
    char *p_end; // Potrebujeme na verifikovanie premien strtol a strtod (ukazovatel na koniec) podla dokumentacie
    if (argc > 7 || argc == 1 || argc == 3 || argc == 4 || argc == 5 || argc == 6)
    {
        fprintf(stderr, "Enter correct number of arguments!\n");
        return 1;
    }
    if (argc == 7)
    {
        if ((strtol(argv[2], &p_end, 10) <= 0) || *p_end != '\0' || p_end == argv[2]) // *pEnd != '\0' nam zarucuje ze bude precitany cely argument a nie len cast pri napr. 1n1
        {                                                                             // pEnd == argv[2] zarucuje kontrolu ak bude hned prvy znak neplatny
            fprintf(stderr, "N > 0\n");
            return 1;
        }
        for (int i = 3; i < argc; i++) // Cyklus na overenie vah
        {
            if (strtod(argv[i], &p_end) < 0.0 || *p_end != '\0' || p_end == argv[i])
            {
                fprintf(stderr, "Weights >= 0\n");
                return 1;
            }
        }
        load_weights(argv, W); // Pri overeni platnosti nacitat vahy do struct W
    }
    if (argc == 2) // Pri nezadanom N nacitame defaultne hodnoty pre vahy
    {
        W->n = 0;
        W->wb = 1.0;
        W->wd = 1.0;
        W->ws = 1.0;
        W->wt = 1.0;
    }
    return 0;
}

void id_sorting(cluster *cluster_array, flow *flow_array)
{
    // TODO
}

void output(cluster *cluster_array, flow *flow_array)
{
    // TODO
    // Clusters:
    // cluster 0: 10 12
    // cluster 1: 11 13
}

// Funkcia main
int main(int argc, char *argv[])
{
    weights W;
    int count;
    if (verify_arguments(argc, argv, &W) != 0)
    {
        return 1;
    }
    char *filename = argv[1];                          // Na filename pouzivam ukazaten aby sa predoslo buffer overflow
    flow *flow_array = load_file(filename, &count);    // Vytvorime pole pre flows
    cluster *cluster_array = allocate_clusters(count); // Vytvorime pole pre clusters
    if (flow_array == NULL)
    {
        return 1;
    }
    if (cluster_array == NULL)
    {
        free(flow_array);
        return 1;
    }
    if (verify_ips(count, flow_array) != 0)
    {
        free_cluster_idxs(cluster_array, count); //
        free(cluster_array);                     // Uvolnenie pamata ak mame neplatnu IP
        free(flow_array);                        //
        return 1;
    }
    single_linkage(cluster_array, count, flow_array, W);
    // output(cluster_array);
    //  temporary
    double test_distance = calculate_distance(flow_array[0], flow_array[1], W);

    for (int i = 0; i < count; i++)
    {
        printf("ID: %d\n", flow_array[i].ID);
    }
    for (int i = 0; i < count; i++)
    {
        printf("saved IDXs: %d\n", cluster_array[i].flow_idxs[0]);
    }

    // printf("%f %f %f %f\n", W.wb, W.wt, W.wd, W.ws);
    printf("Count: %d\n", count);
    printf("Distance: %lf\n", test_distance);
    //
    free_cluster_idxs(cluster_array, count); // Uvolnenie pamate pri bezchybnom bezani
    free(cluster_array);                     //
    free(flow_array);                        //
    return 0;
}
