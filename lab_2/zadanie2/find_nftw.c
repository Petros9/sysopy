#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <limits.h>
#define __USE_XOPEN_EXTENDED 1
#include <ftw.h>

int max_depth;
char* a = NULL;
char* m = NULL;
char* file_type(mode_t mode) { //ok
    if(S_ISREG(mode)){
        return "file";
    }
    if(S_ISDIR(mode)){
        return "dir";
    }
    if(S_ISCHR(mode)){
        return "char dev";
    }
    if(S_ISBLK(mode)){
        return "block dev";
    }
    if(S_ISFIFO(mode)){
        return "fifo";
    }
    if(S_ISLNK(mode)){
        return "slink";
    }
    if(S_ISSOCK(mode)){
        return "sock";
    }
    return "unknown";
}
void print_file_info(const char* PATH, const struct stat* info){ //ok
    printf("FILE PATH: %s \n", PATH); //ścieżka bezwględna
    printf("LINKED FILES NUMBER: %lu \n", info ->st_nlink); //liczba dowiązań
    printf("FILE TYPE: %s \n", file_type(info->st_mode)); // rodzaj pliku
    printf ("FILE SIZE %ld BYTES\n", info->st_size); //rozmiar w bajtach

    char access_date[24];
    strftime(access_date, sizeof(access_date), "%d %m %Y %H:%M", localtime(&info->st_atime));    
    printf("LAST ACCESS DATE: %s\n", access_date); // data ostatniego dostępu

    char modification_date[24];
    strftime(modification_date, sizeof(modification_date), "%d %m %Y %H:%M", localtime(&info->st_mtime));
    printf("LAST MODIFICATION DATE: %s\n", modification_date); //data ostatniej modyfikacji
}
int day_diff(time_t date) {
    return difftime(time(NULL), date) / 86400;
}
int m_time_correct(const struct stat* stats,const char* command) { //data ostatniej modyfikacji
    if(command == NULL){
        return 1;
    }

    int diff = day_diff(stats->st_mtime);
    if(strcmp(command, "+") == 0){
        char str_number[strlen(command)-1];
        for(int i = 1; i<strlen(command);i++){
            str_number[i-1] = command[i];
        }
        int number = atoi(str_number);
        if(diff < number)
        {
            return 1;
        }
        else{
            return 0;
        }

    }

    else if(strcmp(command, "-") == 0){
        char str_number[strlen(command)-1];
        for(int i = 1; i<strlen(command);i++){
            str_number[i-1] = command[i];
        }
        int number = atoi(str_number); 
        if(diff > number)
        {
            return 1;
        }
        else{
            return 0;
        }
    }

    else {
        int number = atoi(command);
        if(number == diff){
            return 1;
        }
        else{
            return 0;
        }
    }
}

int a_time_correct(const struct stat* stats, const char* command){ //data ostatniego dostępu
    if(command == NULL){
        return 1;
    }
   int diff = day_diff(stats->st_atime);
    if(strcmp(command, "+") == 0){
        char str_number[strlen(command)-1];
        for(int i = 1; i<strlen(command);i++){
            str_number[i-1] = command[i];
        }
        int number = atoi(str_number);
        if(diff < number)
        {
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(strcmp(command, "-") == 0){
        char str_number[strlen(command)-1];
        for(int i = 1; i<strlen(command);i++){
            str_number[i-1] = command[i];
        }
        int number = atoi(str_number); 
        if(diff > number)
        {
            return 1;
        }
        else{
            return 0;
        }
    }

    else {
        int number = atoi(command);
        if(number == diff){
            return 1;
        }
        else{
            return 0;
        }
    }
}

static int nftw_find_function(const char* file_path, const struct stat* stats, int type_flag, struct FTW* ftw_buf) {
    int a_number = a_time_correct(stats, a);
    int m_number = m_time_correct(stats, m);
    if(a_number+m_number == 2 && (max_depth < 0 || ftw_buf->level <= max_depth)) {
        print_file_info(file_path, stats);
    }
    return 0;
}


int main(int arg_num, char** args) {
    max_depth = -1;
    a = NULL;
    m = NULL;
    int i = 0;
    char* PATH = args[1];
    while(i<arg_num){
        if(strcmp(args[i], "maxdepth")==0){
            max_depth = atoi(args[i+1]);
            i = i+2;
        }
        else if(strcmp(args[i],"mtime")==0){
            m = args[i+1];
            i = i+2;
        }
        else if(strcmp(args[i], "atime")==0){
            a = args[i+1];
            i = i+2;
        }
        else{
            i++;
        }
    }
    nftw(PATH, nftw_find_function, 10, FTW_PHYS);
    return 0;
}
