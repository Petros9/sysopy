#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.c"

#include <time.h>
#include <sys/times.h>
#include <unistd.h>

array_of_blocks *exec_initialise_array(char *size_as_string)
{
    int size = (int)strtol(size_as_string, NULL, 10);
    size++;
    array_of_blocks *new_main_array =initialise_array(size);
    return new_main_array;
}

void exec_make_pairs(char *files_as_chars, array_of_blocks *main_array)
{
    int str_size = strlen(files_as_chars);
    char files_str[str_size];
    strcpy(files_str, files_as_chars);
    do_diff(make_pairs(files_str), main_array);
}

void exec_create_block(char *file_name, array_of_blocks *main_array)
{
    return create_block(file_name, main_array);
}

int exec_operationss_number(array_of_blocks *main_array, int index)
{
    return operationss_number(main_array,index);
}

void exec_delete_block(array_of_blocks *main_array, char *index_as_str)
{
    int index = atoi(index_as_str);
    delete_block(main_array, index);
}

void exec_delete_operation(array_of_blocks *main_array, char *block_index_as_str, char *operation_index_as_str)
{
    int block_index = atoi(block_index_as_str);
    int operation_index = atoi(operation_index_as_str);
    block *curr_block = main_array->blocks[block_index];
    delete_operation(curr_block, operation_index);
}

double calculate_time(clock_t start, clock_t end)
{
    return (double)(end - start) / (double)sysconf(_SC_CLK_TCK);
}

int main(int args_num, char *args[])
{
    
    array_of_blocks *main_array;
    int i = 1;

    // mierzenie czasu
    struct tms **program_time = calloc(2, sizeof(struct tms *));
    clock_t real_time_program[2];
    for (int j = 0; j < 2; j++)
    {
        program_time[j] = (struct tms *)calloc(1, sizeof(struct tms *));
    }
    real_time_program[0] = times(program_time[0]);
    struct tms **tms_time = calloc(2, sizeof(struct tms *));
    clock_t real_time[2];
    for (int j = 0; j < 2; j++)
    {
        tms_time[j] = (struct tms *)calloc(1, sizeof(struct tms *));
    }



        // START TIME
        real_time[0] = times(tms_time[0]);

        main_array = exec_initialise_array("3");
        
        exec_make_pairs("lamba.txt:beta.txt a.txt:b.txt c.txt:cprim.txt", main_array); 
        //exec_delete_operation(main_array,"0","1");


        real_time[1] = times(tms_time[1]);
        //printf("[REAL_TIME] Executing action %s took %fs\n", command, calculate_time(real_time[0], real_time[1]));
        //printf("[USER_TIME] Executing action %s took %fs\n", command, calculate_time(tms_time[0]->tms_utime, tms_time[1]->tms_utime));
        //printf("[SYSTEM_TIME] Executing action %s took %fs\n", command, calculate_time(tms_time[0]->tms_stime, tms_time[1]->tms_stime));
    real_time_program[1] = times(program_time[1]);
    printf("[REAL_TIME] Executing main.c took %fs\n", calculate_time(real_time_program[0], real_time[1]));
    printf("[USER_TIME] Executing main.c took %fs\n", calculate_time(program_time[0]->tms_utime, program_time[1]->tms_utime));
    printf("[SYSTEM_TIME] Executing main.c took %fs\n", calculate_time(program_time[0]->tms_stime, program_time[1]->tms_stime));
    return 0;
}