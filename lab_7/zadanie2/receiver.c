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


void add_order(){   

    sem_wait(semaphores[FREE_ARRAY]);
    sem_post(semaphores[FREE_INDEX]);


    ORDERS* orders = mmap(NULL, sizeof(ORDERS),PROT_READ | PROT_WRITE, MAP_SHARED, shm_desc, 0);

    int order_value = rand()%RAND_VALUE;
    int index = (get_value(FREE_INDEX) - 1)%ORDER_LIMIT;
    orders->array[index] = order_value;
    int pack_quant = get_value(PACK_QUANT) + 1;
    int send_quant = get_value(SEND_QUANT);

    printf("[Getter] (%d %ld) Dodałem liczbę: %d. Liczb zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n", 
        getpid(), time(NULL), order_value, pack_quant, send_quant);
        
    munmap(orders, sizeof(ORDERS));

    sem_post(semaphores[FREE_ARRAY]);
    sem_post(semaphores[PACK_QUANT]);
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
        if(get_value(PACK_QUANT) + get_value(SEND_QUANT) < ORDER_LIMIT){
            add_order();
        }
    }
    return 0;
}