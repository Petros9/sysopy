#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int arg_num, char** args)
{   
    srand(time(NULL));
    char *pipe_path = args[1];
    char *file_path = args[2];
    int N = atoi(args[3]);

    FILE *input_file = fopen(file_path, "r");
    int pipe_file = open(pipe_path, O_WRONLY);
    char buffer[N];

    while (fgets(buffer, N, input_file) != NULL)
    {
        sleep(rand()%2+1);
        char message[N + 20];
        sprintf(message, "#%d#%s\n", getpid(), buffer);
        write(pipe_file, message, strlen(message));
    }
    close(pipe_file);
    fclose(input_file);
    return 0;
}