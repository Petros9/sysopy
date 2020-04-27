#include "source.h"
int shm_desc;
sem_t *semaphores[6];

void stop(int signal){
    for(int i = 0; i<6; i++){
        sem_close(semaphores[i]);
    }
    exit(EXIT_SUCCESS);
}

int get_value(int index){
    int value;
    sem_getvalue(semaphores[index], &value);
    return value;
}

void send_order(){

    sem_wait(semaphores[FREE_ARRAY]);
    sem_post(semaphores[SEND_ORDER]);
    sem_wait(semaphores[SEND_QUANT]);

    ORDERS* orders = mmap(NULL, sizeof(ORDERS),PROT_READ | PROT_WRITE, MAP_SHARED, shm_desc, 0);
    int index = (get_value(SEND_ORDER)- 1)%ORDER_LIMIT;
    int pack_quant = get_value(PACK_QUANT);
    int send_quant = get_value(SEND_QUANT);

    orders->array[index] *= 3;
    
    printf("[Sender] (%d %ld) Wysłałem zamówienie o wielkości: %d. Liczb zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n", 
        getpid(), time(NULL), orders->array[index], pack_quant, send_quant);

    orders->array[index] = 0;
    
    munmap(orders, sizeof(ORDERS));

    sem_post(semaphores[FREE_ARRAY]);
}


int main(){
    srand(time(NULL));
    signal(SIGINT, stop);
    for(int i = 0; i<6; i++){
        semaphores[i] = sem_open(SEMAPHORES[i], O_RDWR);
    }
    shm_desc = shm_open(SH_M, O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG);

    while(1){
        usleep(rand()%1000000);
        if(get_value(SEND_QUANT) > 0){
            send_order();
        }
    }
    return 0;
}