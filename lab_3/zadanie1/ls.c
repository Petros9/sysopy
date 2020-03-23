 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

char* make_new_path(char* path1, char* path2) {
  char* path = malloc(sizeof(char) * (strlen(path1) + strlen(path2)) + 2);
  sprintf(path, "%s/%s", path1, path2);
  return path;
}

void ls_function(char* PATH) {
  if (PATH == NULL) {
      return;
    }

  DIR* dirent = opendir(PATH);
  if (dirent == NULL){
     return; 
  }

  struct dirent* sub_files;
  struct stat dir_stats;
  while ((sub_files = readdir(dirent))) {

    if (strcmp(sub_files->d_name, "..") == 0 ||strcmp(sub_files->d_name, ".") == 0) continue;

    char* new_PATH = make_new_path(PATH, sub_files->d_name);
    lstat(new_PATH, &dir_stats);

    if (S_ISDIR(dir_stats.st_mode) && fork() == 0) {

        printf("\nPID: %i, PATH: %s\n", getpid(), new_PATH);
        char command[] = "ls -l %s";
        char* ls = calloc(sizeof(command), sizeof(char));
        sprintf(ls, command, new_PATH);
        system(ls);
        free(ls);
        exit(0);

    } else wait(NULL);

    ls_function(new_PATH);
    free(new_PATH);
  }
  closedir(dirent);
}

int main(int arg_num, char** args) {
  char* PATH = args[1];
  ls_function(PATH);
  return 0;
}