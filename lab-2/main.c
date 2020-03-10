#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

double time_diff(clock_t first_clock, clock_t second_clock){
    double result = (second_clock - first_clock) / sysconf (_SC_CLK_TCK);
}


void print_time_diffrence(struct tms* begin, struct tms* end, FILE * raport){
    fprintf(raport, "USER_TIME:", time_diff(begin->tms_utime, end->tms_utime));
    fprintf(raport, "SYSTEM_TIME", time_diff(begin->tms_stime, end->tms_stime));
}


void generate(char* new_file_name, int records_number, int record_length){

    char* command = calloc(len(new_file_name+6,sizeof(char));
    strcpy(command, "touch ");
    strcat(command, new_file_name);
    system(command);
    free(command);

    char * command = calloc(size, sizeof(char));
    for(int i = 0; i < 100; i++){
        command[i] = 0;	
    }

    strcpy(command, "head -c 1000000000 /dev/urandom | tr -dc 'a-z'");
    strcat(command, " | fold -w ");
    strcat(command, record_length);
    strcat(command, " | head -n ");
    strcat(command, records_number);
    strcat(command, " > ");
    strcat(command, new_file_name);
    system(command);
    free(command);
}

void sort_sys(){


}

void sort_lib(){


}

void copy_sys(){

}

void copy_lib(){



}