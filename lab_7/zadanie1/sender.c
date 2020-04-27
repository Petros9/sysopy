#include "source.h"
int shm_ID;
int sem_ID;
key_t sem_KEY;
key_t shm_KEY;
typedef struct sembuf sembuf;

void send_order(){   
    sembuf* operations = (sembuf*)calloc(3, sizeof(sembuf));
    operations[0].sem_num = FREE_ARRAY;
    operations[0].sem_op = 0;
    operations[0].sem_flg = 0;

    operations[1].sem_num = FREE_ARRAY;
    operations[1].sem_op = 1;
    operations[1].sem_flg = 0;

    operations[2].sem_num = SEND_ORDER;
    operations[2].sem_op = 1;
    operations[2].sem_flg = 0;

    operations[3].sem_num = SEND_QUANT;
    operations[3].sem_op = -1;
    operations[3].sem_flg = 0;
    semop(sem_ID, operations, 4); 

    ORDERS* orders = shmat(shm_ID, NULL, 0);

    int index = (semctl(sem_ID, SEND_ORDER, GETVAL, NULL) - 1)%ORDER_LIMIT;
    orders->array[index] *= 3;
    
    printf("[Sender] (%d %ld) Wysłałem zamówienie o wielkości: %d. Liczb zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n", 
        getpid(), time(NULL), orders->array[index], semctl(sem_ID, PACK_QUANT, GETVAL, NULL), semctl(sem_ID, SEND_QUANT, GETVAL, NULL));

    orders->array[index] = 0;
    shmdt(orders);

    operations = (sembuf*)calloc(1, sizeof(sembuf));
    operations[0].sem_num = FREE_ARRAY;
    operations[0].sem_op = -1;
    operations[0].sem_flg = 0;
    semop(sem_ID, operations, 1);    
}

void stop(int signal){
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGINT, stop);
    sem_KEY = ftok(PATH, 0);
    sem_ID = semget(sem_KEY, 0, 0);

    shm_KEY = ftok(PATH, 1);
    shm_ID = shmget(shm_KEY, 0, 0);

    while(1){
        usleep(rand()%1000000);
        if(semctl(sem_ID, SEND_QUANT, GETVAL, NULL)>0){
            send_order();
        }
    }
    return 0;
}