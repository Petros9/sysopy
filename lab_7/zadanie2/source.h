#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ORDER_LIMIT 10
#define RAND_VALUE 20
#define PATH (getenv("HOME"))

#define SH_M "/SHARED_MEMORY"

const char *SEMAPHORES[6] = {"/FREE_ARRAY", "/FREE_INDEX", "/PACK_ORDER", "/SEND_ORDER", "/PACK_QUANT", "/SEND_QUANT"};

#define RECEIVERS 3
#define PACKERS 3
#define SENDERS 3

#define ALL_WORKERS (RECEIVERS + PACKERS + SENDERS)

#define FREE_ARRAY 0
#define FREE_INDEX 1
#define PACK_ORDER 2
#define SEND_ORDER 3
#define PACK_QUANT 4
#define SEND_QUANT 5

struct ORDERS{
    int array[ORDER_LIMIT];
}typedef ORDERS;

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};