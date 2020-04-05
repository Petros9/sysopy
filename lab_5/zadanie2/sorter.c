#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int arg_num, char** args){
    if(arg_num != 2){
        printf("%s", "Zla liczba argumentow!");
        exit(1);
    }
    char* PATH = args[1];

    char* sort_command = (char*)calloc(strlen(PATH)+5,sizeof(char));
    strcpy(sort_command, "sort ");
    strcat(sort_command, PATH);
    FILE* file = popen(sort_command, "w");
    pclose(file);
    return 0;
}