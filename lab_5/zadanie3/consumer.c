#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int arg_num, char** args)
{
    char *pipe_path = args[1];
    char *file_path = args[2];
    int N = atoi(args[3]);
    FILE *pipe_file = fopen(pipe_path, "r");
    FILE *output_file = fopen(file_path, "w");
    char buffer[N];
    while (fgets(buffer, N, pipe_file) != NULL)
    {
        fprintf(output_file, buffer, strlen(buffer));
    }
    fclose(pipe_file);
    fclose(output_file);
    return 0;
}