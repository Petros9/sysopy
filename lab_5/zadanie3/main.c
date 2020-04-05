#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char *consumer[] = {"./consumer", "pipe", "./consumer_data/output", "10", NULL};
    char *producer_1[] = {"./producer", "pipe", "./producer_data/a", "5", NULL};
    char *producer_2[] = {"./producer", "pipe", "./producer_data/b", "5", NULL};
    char *producer_3[] = {"./producer", "pipe", "./producer_data/c", "5", NULL};
    char *producer_4[] = {"./producer", "pipe", "./producer_data/e", "5", NULL};

    mkfifo("pipe", S_IRUSR | S_IWUSR);

    pid_t pid_tab[5];
    if ((pid_tab[0] = fork()) == 0)
        execvp(consumer[0], consumer);

    if ((pid_tab[1] = fork()) == 0)
        execvp(producer_2[0], producer_2);

    if ((pid_tab[2] = fork()) == 0)
        execvp(producer_3[0], producer_3);

    if ((pid_tab[3] = fork()) == 0)
        execvp(producer_1[0], producer_1);

    if ((pid_tab[4] = fork()) == 0)
        execvp(producer_4[0], producer_4);

    for (int i = 0; i < 5; i++)
        waitpid(pid_tab[i], NULL, 0);

    printf("Programme has ended\n");
    return 0;
}