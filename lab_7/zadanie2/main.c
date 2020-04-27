#include "source.h"

pid_t workers[ALL_WORKERS];

void stop(int signal){
    for(int i = 0; i< ALL_WORKERS; i++){
        kill(workers[i], SIGINT);
    }

    for(int i = 0; i<6; i++){
        sem_unlink(SEMAPHORES[i]);
    }
    shm_unlink(SH_M);

    printf("Programme stops!\n");
    exit(EXIT_SUCCESS);
}

int main(){
    signal(SIGINT, stop);


    sem_t *semaphore = sem_open(SEMAPHORES[0], O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG, 1);
    sem_close(semaphore);

    for(int i = 1; i<6; i++){
        semaphore = sem_open(SEMAPHORES[i], O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG, 0);
        sem_close(semaphore);
    }

    int fd = shm_open(SH_M, O_CREAT | O_RDWR, S_IRWXO | S_IRWXU | S_IRWXG);
    ftruncate(fd, sizeof(ORDERS));



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
    for(int i = 0; i<6; i++){
        sem_unlink(SEMAPHORES[i]);
    }
    shm_unlink(SH_M);
    return 0;
}