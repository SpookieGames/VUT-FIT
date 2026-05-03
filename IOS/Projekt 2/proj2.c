// Nacitanie potrebnych kniznic
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>

// Definicia nazvu vystupneho suboru
#define FILE_NAME "proj2.out"

// Deklaracia pouzitych funkcii
void log_action(const char *fmt, ...);
void cleanup(void);
void dispatcher_process(int N, int K, int O, int V);
void cart_process(int idV, int TV);
void visitor_process(int idN, int TN);

typedef struct
{
    int action_counter;
    int queue_size;
    int visitors_announced;
    int is_closing;
    int current_capacity;

    sem_t mutex;           // Chrani vsetky zdielane data
    sem_t cart_ready;      // Voziky cakaju na pokyn dispecera
    sem_t visitor_queue;   // Navstevnici cakaju na nastup do vozika
    sem_t all_boarded;     // Vozik caka cap-krat
    sem_t output_station;  // Mutex vystupnej stanice - len 1 vozik naraz
    sem_t leaving_sem;     // Navstevnici cakaju na vystup z vozika
    sem_t all_left;        // Vozik caka cap krat
    sem_t cart_departed;   // Dispecer caka
    sem_t visitor_arrived; // Dispecer caka ked je fronta prazdna
} SharedMem;

static FILE *f = NULL;
static SharedMem *shm = NULL;

void log_action(const char *fmt, ...)
{
    va_list ap;
    sem_wait(&shm->mutex);
    fprintf(f, "%d: ", shm->action_counter++); // Vypis cisla akcie a inkrementacia citaca
    va_start(ap, fmt);
    vfprintf(f, fmt, ap); // Vypis
    va_end(ap);
    fputc('\n', f);
    fflush(f); // Okamzite zapisanie do suboru
    sem_post(&shm->mutex);
}

void cleanup(void)
{
    if (shm != NULL)
    {
        sem_destroy(&shm->mutex);
        sem_destroy(&shm->cart_ready);
        sem_destroy(&shm->visitor_queue);
        sem_destroy(&shm->all_boarded);
        sem_destroy(&shm->output_station);
        sem_destroy(&shm->leaving_sem);
        sem_destroy(&shm->all_left);
        sem_destroy(&shm->cart_departed);
        sem_destroy(&shm->visitor_arrived);
        munmap(shm, sizeof(SharedMem));
        shm = NULL;
    }
    if (f != NULL)
    {
        fclose(f); // Zatvorenie vystupneho suboru
        f = NULL;
    }
}

void dispatcher_process(int N, int K, int O, int V)
{
    // Generator nahodnych cisel
    srand((unsigned int)getpid());
    log_action("D: started");

    int should_close = 0;

    while (!should_close)
    {
        sem_wait(&shm->mutex);

        int ready = 0;
        while (!ready)
        {
            if (shm->visitors_announced == N && shm->queue_size == 0)
            {
                should_close = 1;
                ready = 1;
            }
            else if (shm->queue_size >= K)
            {
                ready = 1;
            }
            else if (shm->visitors_announced == N && shm->queue_size > 0)
            {
                ready = 1;
            }
            else
            {
                sem_post(&shm->mutex);
                sem_wait(&shm->visitor_arrived);
                sem_wait(&shm->mutex);
            }
        }

        sem_post(&shm->mutex);

        // Ak sa ma zatvorit tak vyskocime z cyklu
        if (should_close)
            break;

        // Kapacita
        sem_wait(&shm->mutex);
        int cap = (shm->queue_size < K) ? shm->queue_size : K;
        shm->current_capacity = cap;
        shm->queue_size -= cap;
        sem_post(&shm->mutex);

        log_action("D: next cart");
        sem_post(&shm->cart_ready);
        sem_wait(&shm->cart_departed); // Odchod z nastupnej stanice
        usleep((unsigned int)O);
    }

    log_action("D: closing");
    sem_wait(&shm->mutex);
    shm->is_closing = 1;
    sem_post(&shm->mutex);
    for (int i = 0; i < V; i++)
        sem_post(&shm->cart_ready);
    exit(EXIT_SUCCESS);
}

void cart_process(int idV, int TV)
{
    srand((unsigned int)getpid() ^ (unsigned int)time(NULL)); // Inicializacia generatora nahodnych cisel
    log_action("V %d: started", idV);

    while (1)
    {
        sem_wait(&shm->cart_ready);

        sem_wait(&shm->mutex);
        int closing = shm->is_closing;
        int cap = shm->current_capacity;
        sem_post(&shm->mutex);

        // Atrakcia je zatvorena
        if (closing)
        {
            log_action("V %d: closed", idV);
            exit(EXIT_SUCCESS);
        }

        // Nastup navstevnikov do vozika
        log_action("V %d: boarding started", idV);
        for (int i = 0; i < cap; i++)
        {
            sem_post(&shm->visitor_queue); // Vyzvanie navstevnikov k nastupeniu
        }
        for (int i = 0; i < cap; i++)
        {
            sem_wait(&shm->all_boarded); // Potvrdenie nastupu
        }
        log_action("V %d: boarding complete", idV);

        sem_post(&shm->cart_departed); // Odchod zo stanice

        // Jazda po drahe [TV/2, TV] mikrosekund
        {
            int half = TV / 2;
            int range = TV - half + 1;
            int ride = half + rand() % range;
            usleep((unsigned int)ride);
        }

        // Uvolnenie stanice
        sem_wait(&shm->output_station);

        // Vystup navstevnikov z vozika
        log_action("V %d: leaving started", idV);
        for (int i = 0; i < cap; i++)
        {
            sem_post(&shm->leaving_sem); // Vyzvanie navstevnikov k vystupeniu
        }
        for (int i = 0; i < cap; i++)
        {
            sem_wait(&shm->all_left); // Potvrdenie vystupu
        }
        log_action("V %d: leaving complete", idV);

        // Uvolnenie stanice
        sem_post(&shm->output_station);
    }
}

void visitor_process(int idN, int TN)
{
    // Generator nahodnych cisel
    srand((unsigned int)getpid() ^ (unsigned int)time(NULL));
    log_action("N %d: started", idN);

    // Simulacia trvania cesty do fronty
    usleep((unsigned int)(rand() % (TN + 1)));

    // Vstup do fronty
    sem_wait(&shm->mutex);
    shm->visitors_announced++; // Navstevnik dorazil do fronty
    shm->queue_size++;         // Zvysime pocet navstevnikov vo fronte
    fprintf(f, "%d: N %d: queue\n", shm->action_counter++, idN);
    fflush(f);
    sem_post(&shm->mutex);
    sem_post(&shm->visitor_arrived);

    sem_wait(&shm->visitor_queue);

    // Nastup do vozika
    sem_wait(&shm->mutex);
    fprintf(f, "%d: N %d: boarding\n", shm->action_counter++, idN);
    fflush(f);
    sem_post(&shm->mutex);
    sem_post(&shm->all_boarded); // Potvrdenie nastupenia

    sem_wait(&shm->leaving_sem);

    sem_wait(&shm->mutex);
    fprintf(f, "%d: N %d: leaving\n", shm->action_counter++, idN);
    fflush(f);
    sem_post(&shm->mutex);
    sem_post(&shm->all_left);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // Overenie poctu argumentov
    if (argc != 7)
    {
        fprintf(stderr, "Usage: %s V N K TV TN O\n", argv[0]);
        return 1;
    }

    // Validacia argumentov pomocou strtol
    char *endp;

    long lV = strtol(argv[1], &endp, 10);
    if (*endp != '\0' || lV <= 0 || lV >= 10)
    {
        fprintf(stderr, "Error: invalid V (0 < V < 10)\n");
        return 1;
    }
    long lN = strtol(argv[2], &endp, 10);
    if (*endp != '\0' || lN <= 0 || lN >= 10000)
    {
        fprintf(stderr, "Error: invalid N (0 < N < 10000)\n");
        return 1;
    }
    long lK = strtol(argv[3], &endp, 10);
    if (*endp != '\0' || lK < 4 || lK > 40)
    {
        fprintf(stderr, "Error: invalid K (4 <= K <= 40)\n");
        return 1;
    }
    long lTV = strtol(argv[4], &endp, 10);
    if (*endp != '\0' || lTV < 0 || lTV > 1000)
    {
        fprintf(stderr, "Error: invalid TV (0 <= TV <= 1000)\n");
        return 1;
    }
    long lTN = strtol(argv[5], &endp, 10);
    if (*endp != '\0' || lTN < 0 || lTN > 1000)
    {
        fprintf(stderr, "Error: invalid TN (0 <= TN <= 1000)\n");
        return 1;
    }
    long lO = strtol(argv[6], &endp, 10);
    if (*endp != '\0' || lO <= 0 || lO > 100)
    {
        fprintf(stderr, "Error: invalid O (0 < O <= 100)\n");
        return 1;
    }

    int V = lV, N = lN, K = lK, TV = lTV, TN = lTN, O = lO; // Priradenie hodnot do premennych

    // Otvorenie vystupneho suboru
    f = fopen(FILE_NAME, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Error: cannot open output file %s\n", FILE_NAME);
        return 1;
    }

    // Alokovanie zdielanej pamate
    shm = mmap(NULL, sizeof(SharedMem),
               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shm == MAP_FAILED)
    {
        fprintf(stderr, "Error: mmap failed\n");
        fclose(f);
        return 1;
    }

    // Inicializacia zdielanych premennych na vychodzi stav
    shm->action_counter = 1;
    shm->queue_size = 0;
    shm->visitors_announced = 0;
    shm->is_closing = 0;
    shm->current_capacity = 0;

    // Inicializacia vsetkych semaforov
    if (sem_init(&shm->mutex, 1, 1) != 0 || sem_init(&shm->cart_ready, 1, 0) != 0 || sem_init(&shm->visitor_queue, 1, 0) != 0 || sem_init(&shm->all_boarded, 1, 0) != 0 || sem_init(&shm->output_station, 1, 1) != 0 || sem_init(&shm->leaving_sem, 1, 0) != 0 || sem_init(&shm->all_left, 1, 0) != 0 || sem_init(&shm->cart_departed, 1, 0) != 0 || sem_init(&shm->visitor_arrived, 1, 0) != 0)
    {
        fprintf(stderr, "Error: sem_init failed\n");
        munmap(shm, sizeof(SharedMem));
        fclose(f);
        return 1;
    }

    // Vytvorenie procesu dispecera pomocou fork()
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Error: fork failed (dispatcher)\n");
        cleanup();
        return 1;
    }
    if (pid == 0)
        dispatcher_process(N, K, O, V); // Potomok sa nikdy nevracia

    // Vytvorenie V procesov vozikov pomocou fork()
    for (int i = 0; i < V; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Error: fork failed (cart %d)\n", i + 1);
            cleanup();
            return 1;
        }
        if (pid == 0)
            cart_process(i + 1, TV); // Potomok sa nikdy nevracia
    }

    // Vytvorenie N procesov navstevnikov pomocou fork()
    for (int i = 0; i < N; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Error: fork failed (visitor %d)\n", i + 1);
            cleanup();
            return 1;
        }
        if (pid == 0)
            visitor_process(i + 1, TN); // Potomok sa nikdy nevracia
    }

    while (wait(NULL) > 0)
        ;

    cleanup();
    return 0;
}