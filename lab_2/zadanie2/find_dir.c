#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
// stat, fstat, lstat - pobieranie stanu pliku


char* file_type(mode_t mode) { //ok
    if(S_ISREG(mode))
        return "file";
    if(S_ISDIR(mode))
        return "dir";
    if(S_ISCHR(mode))
        return "char dev";
    if(S_ISBLK(mode))
        return "block dev";
    if(S_ISFIFO(mode))
        return "fifo";
    if(S_ISLNK(mode))
        return "slink";
    if(S_ISSOCK(mode))
        return "sock";
    return "unknown";
}

void print_file_info(char* PATH, struct stat* info){ //ok
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


int m_time_correct(const struct stat* stats, char* command) { //data ostatniej modyfikacji
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

int a_time_correct(const struct stat* stats, char* command){ //data ostatniego dostępu
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

void find_function(char* PATH, int curr_depth, int max_depth, char* a_command, char* m_command){ // compiluje sie
    if(PATH == NULL){
        return;
    }
    if(curr_depth == max_depth){
        return;
    }
    DIR* dirent = opendir(PATH);

    if(dirent == NULL){
        printf("%s", "That dirent is empty");
        exit(1);
    }
    struct dirent* sub_files;
    char sub_PATH[100];
    while((sub_files = readdir(dirent))!=NULL){

        strcpy(sub_PATH, PATH);
        strcat(sub_PATH, "/");
        strcat(sub_PATH,sub_files -> d_name);

        struct stat* path_stats = (struct stat*)malloc(sizeof(struct stat));
        lstat(sub_PATH, path_stats);

        if (S_ISDIR(path_stats->st_mode))
        {
            if (strcmp(sub_files->d_name, ".") == 0 || strcmp(sub_files->d_name, "..") == 0)
            {
                continue;
            }
            find_function(sub_PATH, curr_depth + 1, max_depth, a_command, m_command);
        }

        int m = m_time_correct(path_stats, m_command);
        int a = a_time_correct(path_stats, a_command);
        if(m+a == 2){
            print_file_info(sub_PATH, path_stats);
        }
    }

    closedir(dirent);
}

int main(int arg_num, char* args[]){
    int i = 2;
    int max_depth = -1;
    char* a = NULL;
    char* m = NULL;
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
    find_function(PATH,0,max_depth,a,m);
}