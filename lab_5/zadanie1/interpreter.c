#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define TOP_LIMIT 3

char** get_arguments(char* arg_line){
    char** args = NULL;
    char dividers[2] = {' ', '\n'};
    arg_line = strtok(arg_line, dividers);
    int count = 0;
    while(arg_line){
        count ++;
        args = realloc(args, count*sizeof(char*));
        args[count - 1] = arg_line;
        arg_line = strtok(NULL, dividers); 
    }
    args = realloc(args, (count+1)*sizeof(char*));
    args[count] = NULL;
    return args;
}

int execute_commands(char * arg_line){

    int proc_numbec = 0;
    int fd[2][2];
    char * commands[TOP_LIMIT];
    arg_line = strtok(arg_line, "|");
    while (arg_line){
        if(proc_numbec != TOP_LIMIT){
            commands[proc_numbec++] = arg_line;
        }
        arg_line = strtok(NULL, "|");
    }

    int i = 0;

    for (i; i < proc_numbec; i++){

        if (i != 0){
            close(fd[i % 2][0]);
            close(fd[i % 2][1]);
        }

        pipe(fd[i % 2]);

        if (fork() == 0){
            char ** args = get_arguments(commands[i]);

            if (i != proc_numbec - 1){
                close(fd[i % 2][0]);
                dup2(fd[i % 2][1], STDOUT_FILENO);
            }

            if (i != 0) {
                close(fd[(i+1) % 2][1]);
                dup2(fd[(i+1) % 2][0], STDIN_FILENO);
            }

            execvp(args[0], args);
        }
    }
    close(fd[i%2][0]);
    close(fd[i%2][1]);
    wait(NULL);
    exit(0);
}

int main(int arg_num, char** args){
    if(arg_num != 2){
        printf("%s", "Zla liczba argumentow!");
        exit(1);
    }

    FILE* command_file;
    command_file = fopen(args[1], "r");
    if(command_file == NULL){
        printf("%s", "Zla nazwa pliku!");
        exit(1);
    }
    char* arg_line = NULL;
    size_t _ = 0;
    ssize_t reader;

    while((reader = getline(&arg_line, &_, command_file)) != -1){
        if(vfork() == 0){
            execute_commands(arg_line);
        }else{
            wait(NULL);
        }
    }
    fclose(command_file);
    free(arg_line);
    return 0;
}