#include "source.h"
int sem_ID;
int shm_ID;
key_t sem_KEY;
key_t shm_KEY;
pid_t workers[ALL_WORKERS];

void stop(int signal){
    for(int i = 0; i< ALL_WORKERS; i++){
        kill(workers[i], SIGINT);
    }
    semctl(sem_ID, FREE_ARRAY, IPC_RMID, NULL);
    shmctl(shm_ID, IPC_RMID, NULL);
    printf("Programme stops!\n");
    system("make remove");
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGINT, stop);
    sem_KEY = ftok(PATH, 0);
    sem_ID = semget(sem_KEY, 6, 0666 | IPC_CREAT);
    union semun arg;
    arg.val = 0;

    semctl(sem_ID, FREE_ARRAY, SETVAL, arg);
    semctl(sem_ID, FREE_INDEX, SETVAL, arg);
    semctl(sem_ID, PACK_ORDER, SETVAL, arg);
    semctl(sem_ID, PACK_QUANT, SETVAL, arg);

    shm_KEY = ftok(PATH, 1);

    shm_ID = shmget(shm_KEY, sizeof(struct ORDERS), 0666 | IPC_CREAT);
    for(int i = 0; i< RECEIVERS; i++){
        pid_t receiver_pid = fork();
        if(receiver_pid == 0){
            execlp("./receiver", "receiver", NULL);
        }
        workers[i] = receiver_pid;
    }
    for(int i = 0; i< PACKERS; i++){
        pid_t packer_pid = fork();
        if(packer_pid == 0){
            execlp("./packer", "packer", NULL);
        }
        workers[i + RECEIVERS] = packer_pid;
    }
    for(int i = 0; i< SENDERS; i++){
        pid_t sender_pid = fork();
        if(sender_pid == 0){
            execlp("./sender", "sender", NULL);
        }
        workers[i + RECEIVERS + PACKERS] = sender_pid;
    }        

    for(int i = 0; i< ALL_WORKERS; i++){
        wait(NULL);
    }

    semctl(sem_ID, FREE_ARRAY, IPC_RMID, NULL);
    shmctl(shm_ID, IPC_RMID, NULL);

    return 0;
}