#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// generalne uwagi:
// - sprawdzić rzutowanie calloca -> char* x = (char *) calloc(size, sizeof(char))
// - sprawdzić czy się length i records zgadzają

double time_diff(clock_t first_clock, clock_t second_clock){

    double result = ((double)second_clock - first_clock) / sysconf (_SC_CLK_TCK);
    return result;
}


void print_time_diffrence(struct tms* begin, struct tms* end, FILE * raport){
   char array1[10];
   sprintf(array1, "%f", time_diff(begin->tms_utime,end->tms_utime));
   fprintf(raport," ");    
   fprintf(raport, "USER_TIME: ");
   fprintf(raport," ");   
   fprintf(raport,array1);

   char array2[10];
   sprintf(array2, "%f", time_diff(begin->tms_stime,end->tms_stime));
   
   fprintf(raport," ");    
   fprintf(raport, "SYSTEM_TIME: ");
   fprintf(raport," ");   
   fprintf(raport,array2);
   fprintf(raport,"\n");

}


void generate(char* new_file_name, char* records_number, char* record_length){ //dziala

    char* touch_command = calloc(strlen(new_file_name)+6,sizeof(char));
    strcpy(touch_command, "touch ");
    strcat(touch_command, new_file_name);
    system(touch_command);
    free(touch_command);

    char * generate_command = calloc(100, sizeof(char));
    for(int i = 0; i < 100; i++){
        generate_command[i] = 0;	
    }

    strcpy(generate_command, "head -c 1000000000 /dev/urandom | tr -dc 'a-z'");
    strcat(generate_command, " | fold -w ");
    strcat(generate_command, record_length);
    strcat(generate_command, " | head -n ");
    strcat(generate_command, records_number); //
    strcat(generate_command, " > ");
    strcat(generate_command, new_file_name);
    system(generate_command);
    free(generate_command);
}

void swap_sys(int desc, int first, int second, int record_length)
{
    char* first_record = calloc(record_length+1, sizeof(char));
    char* second_record = calloc(record_length+1, sizeof(char));

    lseek(desc, (record_length+1)*first, 0);
    read(desc, first_record, record_length+1);
    lseek(desc, (record_length+1)*second, 0);
    read(desc, second_record, record_length+1);

    lseek(desc, (record_length+1)*second, 0);
    write(desc, first_record, record_length+1);

    lseek(desc, (record_length+1)*first, 0);
    write(desc, second_record, record_length+1);

    free(first_record);
    free(second_record);
}

int partition_sys (int desc, int p, int r, int record_length)
{
    char* pivot = calloc(record_length+1, sizeof(char));
    lseek(desc, (record_length+1)*r, 0);
    read(desc, pivot, record_length+1);
    int i = p-1;
    char* potential_swap = calloc(record_length+1, sizeof(char));
    for(int j = p; j<r; j++)
    {
        lseek(desc, (record_length+1)*j, 0);
        read(desc, potential_swap, record_length+1);

        if(strcmp(potential_swap, pivot)<0){
            i++;
            swap_sys(desc, i, j, record_length);
        }
    }
    free(pivot);
    free(potential_swap);
    i++;
    swap_sys(desc, i, r, record_length);
    return i;

}


void quick_sort_sys(int desc, int p, int r, int record_length)
{
    if(p<r){
        int q = partition_sys(desc, p,r,record_length);
        quick_sort_sys(desc, p, q-1, record_length);
        quick_sort_sys(desc, q+1, r, record_length);
    }
}

void sort_sys(char* file_name, char* records_number, char* record_length){

    int desc = open(file_name, O_RDWR);

    if(desc == -1){
        printf("%s","There are no file with such name!");
        exit(1);
    }

    quick_sort_sys(desc, 0 , atoi(record_length)-1, atoi(record_length));
    close(desc);
}



void swap_lib(FILE* sort_file, int first, int second, int record_length)
{
    char* first_record = calloc(record_length+1, sizeof(char));
    char* second_record = calloc(record_length+1, sizeof(char));

    fseek(sort_file, (record_length + 1)*first, 0);
    fread(first_record, sizeof(char), record_length+1, sort_file);

    fseek(sort_file, (record_length+1)*second, 0);
    fread(second_record, sizeof(char), record_length+1, sort_file);

	fseek(sort_file, (record_length+1)*second, 0);
	fwrite(first_record, sizeof(char), record_length+1 ,sort_file);

	fseek(sort_file, (record_length+1)*first, 0);
	fwrite(second_record, sizeof(char), record_length+1, sort_file);

    free(first_record);
    free(second_record);
}

int partition_lib(FILE* sort_file, int p, int r, int record_length)
{
    char* pivot = calloc(record_length+1, sizeof(char));
    fseek(sort_file, (record_length)*r, 0);
    fread(pivot, sizeof(char), record_length+1, sort_file);
    int i = p-1;
    char* potential_swap = calloc(record_length+1, sizeof(char));
    for(int j = p; j<r; j++)
    {
        fseek(sort_file, record_length+1, 0);
        fread(potential_swap, sizeof(char), record_length+1, sort_file);

        if(strcmp(potential_swap,pivot)<0){
            i++;
            swap_lib(sort_file, i, j, record_length);
        }
    }
    free(potential_swap);
    free(pivot);
    i++;
    swap_lib(sort_file, i, r, record_length);
    return i;
}

void quick_sort_lib(FILE* sort_file, int p, int r, int record_length)
{
    if(p < r)
    {
        int q = partition_lib(sort_file, p, r, record_length);
        quick_sort_lib(sort_file, p, q-1, record_length);
        quick_sort_lib(sort_file, q+1, r, record_length);
    }
}

void sort_lib(char* file_name, char* records_number, char* record_length){

    FILE* file = fopen(file_name, "r+"); // do czytania i nadpisywania

    if(file == NULL){
        printf("%s", "There is no file with such name!");
    }
    quick_sort_lib(file, 0, atoi(records_number)-1, atoi(record_length));
    fclose(file);
}









void copy_sys(char* first_file, char* second_file, int records_number, int record_length){

    int desc1 = open(first_file, O_RDONLY);
    int desc2 = open(second_file, O_WRONLY);

    if(desc1 == -1 || desc2 == -2){
        printf("%s", "Wrong file names");
        exit(1);
    }

    char buffor[record_length+1];
    for(int i = 0; i<records_number; i++)
    {
        read(desc1, buffor, record_length+1);
        write(desc2, buffor, record_length+1);
    }

    close(desc1);
    close(desc2);
}

void copy_lib(char* first_file_name, char* second_file_name, int records_number, int record_length){ //działa

    char* buffor = calloc(record_length+1, sizeof(char));

    FILE* first_file = fopen(first_file_name, "r");
    FILE* second_file = fopen(second_file_name,"w");

    if(first_file == NULL){
        printf("%s", "There is no file with such name!");
        exit(1);
    }

    if(second_file == NULL){
        printf("%s", "There is no file with such name!");
        exit(1);
    }    

    for(int i = 0; i<records_number; i++)
    {
        fread(buffor, sizeof(char), record_length+1, first_file);
        fwrite(buffor, sizeof(char), record_length+1, second_file);
    }

    fclose(first_file);
    fclose(second_file);

    free(buffor);
}

// tu trzeba jebitnego maina zrobić
int main(int args_num, char* args[])
{

    struct tms *tms[args_num + 1];
    for(int i = 0; i < args_num; i++){
        tms[i] = calloc(1, sizeof(struct tms*));
    }

    FILE* result = fopen("wyniki.txt", "a");

    int i = 1;
    int curr = 0;

    while(i < args_num)
    {
        if(strcmp(args[i], "generate") == 0){
            times(tms[curr]);
            generate(args[i+1], args[i+2], args[i+3]);
            times(tms[curr+1]);
            fprintf(result, "To generate %s records of length %s bytes:\n", args[i+2], args[i+3]);
            print_time_diffrence(tms[curr], tms[curr+1], result);
            i = i + 4;
            curr = curr + 2;
        }

        else if(strcmp(args[i], "sort_sys") == 0){
            times(tms[curr]);
            sort_sys(args[i+1], atoi(args[i+2]), atoi(args[i+3]));
            times(tms[curr+1]);
            fprintf(result, "To sort with system functions %s records of length %s bytes took:\n", args[i+2], args[i+3]);
            print_time_diffrence(tms[curr], tms[curr+1], result);
            i = i + 5;
            curr = curr + 2;            
        }

        else if(strcmp(args[i], "sort_lib") == 0){
            
            times(tms[curr]);
            sort_lib(args[i+1], atoi(args[i+2]), atoi(args[i+3]);
            times(tms[curr+1]);
            fprintf(result, "To sort with library functions %s records of length %s bytes took:\n", args[i+2], args[i+3]);
            print_time_diffrence(tms[curr], tms[curr+1], result);
            i = i + 5;
            curr = curr + 2;              
        }

        else if(strcmp(args[i], "copy_sys") == 0){
            
            times(tms[curr]);
            copy_sys(args[i+1], args[i+2], atoi(args[i+3]), atoi(args[i+4]));
            times(tms[curr+1]);
            fprintf(result, "To copy with system functions %s records of length %s bytes took:\n", args[i+3], args[i+4]);
            print_time_diffrence(tms[curr], tms[curr+1], result);
            i = i + 6;
            curr = curr + 2;           
        }

        else if(strcmp(args[i], "copy_lib") == 0){
            
            times(tms[curr]);
            copy_lib(args[i+1], args[i+2], atoi(args[i+3]), atoi(args[i+4]));
            times(tms[curr+1]);
            fprintf(result, "To copy with library functions %s records of length %s bytes took:\n", args[i+3], args[i+4]);
            print_time_diffrence(tms[curr], tms[curr+1],result);
            i = i + 6;
            curr = curr + 2;            
        }        
    }
}