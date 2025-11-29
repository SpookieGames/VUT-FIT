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
    float wb, wt, wd, ws;
} weights;

void load_weights(char *argv[], weights *W)
{
    W->wb = atof(argv[3]);
    W->wt = atof(argv[4]);
    W->wd = atof(argv[5]);
    W->ws = atof(argv[6]);
}

int load_file(const char *filename, flow **flow_array)
{
    int count, flow_id, flow_duration, total_bytes, packet_count;
    char src_ip[20];
    char dst_ip[20];
    float avg_interarrival;
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }
    fscanf(f, "count=%d", &count);
    if (count < 0)
    {
        return 1;
    }

    *flow_array = malloc(count * sizeof(flow));
    for (int i = 0; i < count; i++)
    {
        fscanf(f, "%d %s %s %d %d %d %f", &flow_id, src_ip, dst_ip, &total_bytes, &flow_duration, &packet_count, &avg_interarrival);

        flow_array[i]->ID = flow_id;
        strcpy(flow_array[i]->src_ip, src_ip);
        strcpy(flow_array[i]->dst_ip, dst_ip);
        flow_array[i]->b = total_bytes;
        flow_array[i]->t = flow_duration;
        flow_array[i]->d = avg_interarrival;

        if (packet_count <= 0)
        {
            fprintf(stderr, "Packet count cannot be 0");
            fclose(f);
            free(flow_array);
            return 1;
        }
        else
        {
            flow_array[i]->s = ((float)total_bytes / packet_count);
        }

        printf("%d %s %s %d %d %d %f\n", flow_id, src_ip, dst_ip, total_bytes, flow_duration, packet_count, avg_interarrival);
    }
    fclose(f);
    return 0;
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
    for (int i = 3; i < argc; i++)
    {
        if (strtod(argv[i], &p_end) < 0.0 || *p_end != '\0' || p_end == argv[i])
        {
            fprintf(stderr, "Weights >= 0\n");
            return 1;
        }
    }
    load_weights(argv, W);
    return 0;
}

int main(int argc, char *argv[])
{
    weights W;
    flow *flow_array = NULL;
    if (verify_arguments(argc, argv, &W) != 0)
    {
        return 1;
    }
    const char *filename = argv[1];
    const int n = atoi(argv[2]);
    if (load_file(filename, &flow_array) != 0)
    {
        return 1;
    }

    // printf("%f %f %f %f\n", W.wb, W.wt, W.wd, W.ws);

    return 0;
}
