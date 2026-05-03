#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>

#define FILE_NAME "proj2.out"

typedef struct
{
    int action_counter;
    int queue_size;
    int visitors_announced;
    int is_closing;
    int current_capacity;

    sem_t mutex;
    sem_t cart_ready;
    sem_t visitor_queue;
    sem_t all_boarded;
    sem_t output_station;
    sem_t leaving_sem;
    sem_t all_left;
    sem_t cart_departed;
    sem_t visitor_arrived;
} SharedData;

static FILE *pfile = NULL;
static SharedData *shd = NULL;

void log_action(const char *fmt, ...)
{
    va_list ap;
    sem_wait(&shd->mutex);
    fprintf(pfile, "%d: ", shd->action_counter++);
    va_start(ap, fmt);
    vfprintf(pfile, fmt, ap);
    va_end(ap);
    fputc('\n', pfile);
    fflush(pfile);
    sem_post(&shd->mutex);
}

void cleanup(void)
{
    if (shd != NULL)
    {
        sem_destroy(&shd->mutex);
        sem_destroy(&shd->cart_ready);
        sem_destroy(&shd->visitor_queue);
        sem_destroy(&shd->all_boarded);
        sem_destroy(&shd->output_station);
        sem_destroy(&shd->leaving_sem);
        sem_destroy(&shd->all_left);
        sem_destroy(&shd->cart_departed);
        sem_destroy(&shd->visitor_arrived);
        (void)munmap(shd, sizeof(SharedData));
        shd = NULL;
    }
    if (pfile != NULL)
    {
        fclose(pfile);
        pfile = NULL;
    }
}

void dispatcher_process(int N, int K, int O, int V)
{
    srand((unsigned int)getpid());
    log_action("D: started");

    while (1)
    {
        sem_wait(&shd->mutex);
        while (1)
        {
            if (shd->visitors_announced == N && shd->queue_size == 0)
            {
                sem_post(&shd->mutex);
                goto closing;
            }
            if (shd->queue_size >= K)
                break;
            if (shd->visitors_announced == N && shd->queue_size > 0)
                break;

            sem_post(&shd->mutex);
            sem_wait(&shd->visitor_arrived);
            sem_wait(&shd->mutex);
        }

        int cap = (shd->queue_size < K) ? shd->queue_size : K;
        shd->current_capacity = cap;
        shd->queue_size -= cap;
        sem_post(&shd->mutex);

        log_action("D: next cart");
        sem_post(&shd->cart_ready);
        sem_wait(&shd->cart_departed);
        usleep((unsigned int)O);
    }

closing:
    log_action("D: closing");
    sem_wait(&shd->mutex);
    shd->is_closing = 1;
    sem_post(&shd->mutex);
    for (int i = 0; i < V; i++)
        sem_post(&shd->cart_ready);
    exit(EXIT_SUCCESS);
}

void cart_process(int idV, int TV)
{
    srand((unsigned int)getpid() ^ (unsigned int)time(NULL));
    log_action("V %d: started", idV);

    while (1)
    {
        sem_wait(&shd->cart_ready);

        sem_wait(&shd->mutex);
        int closing = shd->is_closing;
        int cap = shd->current_capacity;
        sem_post(&shd->mutex);

        if (closing)
        {
            log_action("V %d: closed", idV);
            exit(EXIT_SUCCESS);
        }

        log_action("V %d: boarding started", idV);
        for (int i = 0; i < cap; i++)
        {
            sem_post(&shd->visitor_queue);
        }
        for (int i = 0; i < cap; i++)
        {
            sem_wait(&shd->all_boarded);
        }
        log_action("V %d: boarding complete", idV);

        sem_post(&shd->cart_departed);

        {
            int half = TV / 2;
            int range = TV - half + 1;
            int ride = half + rand() % range;
            usleep((unsigned int)ride);
        }

        sem_wait(&shd->output_station);

        log_action("V %d: leaving started", idV);

        for (int i = 0; i < cap; i++)
        {
            sem_post(&shd->leaving_sem);
        }
        for (int i = 0; i < cap; i++)
        {
            sem_wait(&shd->all_left);
        }

        log_action("V %d: leaving complete", idV);

        sem_post(&shd->output_station);
    }
}

void visitor_process(int idN, int TN)
{
    srand((unsigned int)getpid() ^ (unsigned int)time(NULL));
    log_action("N %d: started", idN);

    usleep((unsigned int)(rand() % (TN + 1)));

    sem_wait(&shd->mutex);
    shd->visitors_announced++;
    shd->queue_size++;
    fprintf(pfile, "%d: N %d: queue\n", shd->action_counter++, idN);
    fflush(pfile);
    sem_post(&shd->mutex);
    sem_post(&shd->visitor_arrived);

    sem_wait(&shd->visitor_queue);

    sem_wait(&shd->mutex);
    fprintf(pfile, "%d: N %d: boarding\n", shd->action_counter++, idN);
    fflush(pfile);
    sem_post(&shd->mutex);
    sem_post(&shd->all_boarded);

    sem_wait(&shd->leaving_sem);

    sem_wait(&shd->mutex);
    fprintf(pfile, "%d: N %d: leaving\n", shd->action_counter++, idN);
    fflush(pfile);
    sem_post(&shd->mutex);
    sem_post(&shd->all_left);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{

    if (argc != 7)
    {
        fprintf(stderr, "Usage: %s V N K TV TN O\n", argv[0]);
        return 1;
    }

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

    int V = lV, N = lN, K = lK, TV = lTV, TN = lTN, O = lO;

    pfile = fopen(FILE_NAME, "w");
    if (pfile == NULL)
    {
        fprintf(stderr, "Error: cannot open output file %s\n", FILE_NAME);
        return 1;
    }

    shd = mmap(NULL, sizeof(SharedData),
               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shd == MAP_FAILED)
    {
        fprintf(stderr, "Error: mmap failed\n");
        fclose(pfile);
        return 1;
    }

    shd->action_counter = 1;
    shd->queue_size = 0;
    shd->visitors_announced = 0;
    shd->is_closing = 0;
    shd->current_capacity = 0;

    if (sem_init(&shd->mutex, 1, 1) != 0 ||
        sem_init(&shd->cart_ready, 1, 0) != 0 ||
        sem_init(&shd->visitor_queue, 1, 0) != 0 ||
        sem_init(&shd->all_boarded, 1, 0) != 0 ||
        sem_init(&shd->output_station, 1, 1) != 0 ||
        sem_init(&shd->leaving_sem, 1, 0) != 0 ||
        sem_init(&shd->all_left, 1, 0) != 0 ||
        sem_init(&shd->cart_departed, 1, 0) != 0 ||
        sem_init(&shd->visitor_arrived, 1, 0) != 0)
    {
        fprintf(stderr, "Error: sem_init failed\n");
        (void)munmap(shd, sizeof(SharedData));
        fclose(pfile);
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Error: fork failed (dispatcher)\n");
        cleanup();
        return 1;
    }
    if (pid == 0)
        dispatcher_process(N, K, O, V);

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
            cart_process(i + 1, TV);
    }

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
            visitor_process(i + 1, TN);
    }

    while (wait(NULL) > 0)
        ;

    cleanup();
    return 0;
}