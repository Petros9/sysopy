#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
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
    prinf("FILE TYPE: %s \n", file_type(info->st_mode)); // rodzaj pliku
    printf ("FILE SIZE %ld BYTES\n", info->st_size); //rozmiar w bajtach

    char access_date[24];
    strftime(access_date, sizeof(access_date), "%d %m %Y %H:%M", localtime(&info->st_atime));    
    printf("\t LAST ACCESS DATE: %s\n", access_date); // data ostatniego dostępu

    char modification_date[24];
    strftime(modification_date, sizeof(modification_date), "%d %m %Y %H:%M", localtime(&stats->st_mtime));
    printf("\t LAST MODIFICATION DATE: %s\n", modification_date); //data ostatniej modyfikacji
}
void mtime(char* PATH){ //dane były obliczane przez ostatnie n dób

}

}

void atime(){

} 

void maxdepth(char* PATH, int curr_depth){
 
}




void print_linked_files(char* PATH, char* command, int depth){

    if(strcmp(command, "maxdepth") == 0){
        maxdepth(PATH, depth);
    }
    else if(strcmp(command, "atime") == 0){
        
    }
    else if(strcmp(command. "mtime") == 0){
        
    }
    

}


int main(void){

}