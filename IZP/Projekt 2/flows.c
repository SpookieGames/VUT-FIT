#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Sflow
{
    int ID; // flow ID
    char src_ip[20];
    char dst_ip[20];
    int b;   // total_bytes
    int t;   // flow_duration
    float d; // avg_interarrival_time
    float s; // total_bytes / packet_count
} flow;

typedef struct Scluster
{
    /*int size;
    int *pole;*/
} cluster;

typedef struct Sweights
{
    float wb, wt, wd, ws; // vahy
} weights;

// Deklaracia funkcii
void load_weights(char *argv[], weights *W);
flow *load_file(const char *filename, int *count_out);
int verify_arguments(int argc, char *argv[], weights *W);

void load_weights(char *argv[], weights *W)
{
    W->wb = atof(argv[3]); // Mozeme pouzit atof nakolko sme vstup uz overili
    W->wt = atof(argv[4]); //
    W->wd = atof(argv[5]); //
    W->ws = atof(argv[6]); //
}

flow *load_file(const char *filename, int *count_out)
{
    int count, flow_id, flow_duration, total_bytes, packet_count;
    char src_ip[20];
    char dst_ip[20];
    float avg_interarrival;

    FILE *f = fopen(filename, "r"); // Otvorenie suboru na citanie
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    fscanf(f, "count=%d", &count); // Nacitanie poctu flows z 1. riadku
    if (count < 0)
    {
        fclose(f);
        return NULL;
    }

    flow *flow_array = malloc(count * sizeof(flow)); // Alokovanie pamata pre vsetky flow
    if (flow_array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        fclose(f);
        return NULL;
    }

    // Cyklus na populovanie flow_array
    for (int i = 0; i < count; i++)
    {
        if (fscanf(f, "%d %s %s %d %d %d %f", &flow_id, src_ip, dst_ip, &total_bytes, &flow_duration, &packet_count, &avg_interarrival) != 7)
        {
            fprintf(stderr, "Count is incorrect\n");
            fclose(f);
            free(flow_array);
            return NULL;
        }

        flow_array[i].ID = flow_id;           // Odovzdanie nacitanych hodnot do flow
        strcpy(flow_array[i].src_ip, src_ip); //
        strcpy(flow_array[i].dst_ip, dst_ip); //
        flow_array[i].b = total_bytes;        //
        flow_array[i].t = flow_duration;      //
        flow_array[i].d = avg_interarrival;   //

        if (packet_count <= 0) // Neda sa delit nulou a zaporny pocet paketov nedava zmysel
        {
            fprintf(stderr, "Packet count cannot be 0");
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

// Funkcia na verifikaciu platnosti zadanych argumentov
int verify_arguments(int argc, char *argv[], weights *W)
{
    char *p_end; // Potrebujeme na verifikovanie premien strtol a strtod
    if (argc != 7)
    {
        fprintf(stderr, "Enter correct number of arguments!\n");
        return 1;
    }
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
    return 0;
}

int main(int argc, char *argv[])
{
    weights W;
    int count;
    if (verify_arguments(argc, argv, &W) != 0)
    {
        return 1;
    }
    const char *filename = argv[1];
    const int n = atoi(argv[2]);
    flow *flow_array = load_file(filename, &count); // Vytvorime pole flows

    if (flow_array == NULL)
    {
        return 1;
    }

    // temporary
    // printf("%f %f %f %f\n", W.wb, W.wt, W.wd, W.ws);
    printf("Count: %d\n", count);

    free(flow_array);
    return 0;
}
