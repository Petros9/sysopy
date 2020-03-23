#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/times.h>
#include <wait.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <time.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>


typedef struct{
    FILE *file;
    unsigned int rows;
    unsigned int columns;
} matrix;

matrix* initialise_matrix(char* PATH, unsigned int rows, unsigned columns, int offset){

    FILE* file = fopen(PATH, "w+");
    if(file == NULL){
        return NULL;
    }

    matrix* result_matrix = (matrix*) malloc(sizeof(matrix));
    result_matrix->file = file;
    result_matrix->rows = rows;
    result_matrix->columns = columns;
    if(offset == 2)
    {
        fwrite(&result_matrix->rows, sizeof(unsigned int), 1, file);
        fwrite(&result_matrix->columns, sizeof(unsigned int), 1, file);
    }
    fseek(file, 0, SEEK_SET);
    return result_matrix;
}

void set_matrix_value(matrix* set_matrix, int value,unsigned int column, unsigned int row){

    fseek(set_matrix->file, (set_matrix->columns*row+column)*sizeof(int) + 2, SEEK_SET);
    fwrite(&value, sizeof(int), 1, set_matrix->file);
}


void set_tmp_matrix_value(matrix* set_matrix, int value,unsigned int column, unsigned int row){

    fseek(set_matrix->file, (set_matrix->columns*row+column)*sizeof(int), SEEK_SET);
    fwrite(&value, sizeof(int), 1, set_matrix->file);
}

int get_matrix_value(matrix* get_matrix, unsigned int column, unsigned int row, int offset){

    fseek(get_matrix->file, (get_matrix->columns*row+column)*sizeof(int) + offset, SEEK_SET);
    int value;
    fread(&value,sizeof(int),1,get_matrix->file);
    return value;
}


void print_matrix(matrix* print_matrix, int offset){
    int value;
    for(unsigned int r = 0; r < print_matrix->rows; r++){
        for(unsigned int c = 0; c< print_matrix->columns; c++){
            value = get_matrix_value(print_matrix, r, c, offset);
            printf(" %d ", value);
        }
        printf("\n");
    }
}



void free_matrix(matrix* free_matrix){
    fclose(free_matrix->file);
    free(free_matrix);
}


void multiply_column(matrix* first_matrix, matrix* second_matrix, matrix** result_matrix,int column_number, int child_processes){
    
    int fd = fileno((*result_matrix)->file);
    struct flock lock;
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    for(int i = column_number; i<(*result_matrix)->columns; i+=child_processes){
        for (int f_row = 0; f_row < (*result_matrix)->rows; f_row++)
        {
            int value = 0;
            for (int s_column = 0; s_column < (*result_matrix)->columns; s_column++)
            {
                value += get_matrix_value(first_matrix, f_row, s_column,2) * get_matrix_value(second_matrix, s_column, i,2);
            }
            set_matrix_value(*result_matrix, value, f_row, i);
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
}

void multiply_column_separate(matrix* first_matrix, matrix* second_matrix, matrix** result_matrix, int column_number, int child_processes, int number, int inner_column){

    for(int i = column_number; i<inner_column; i+=child_processes){
        for (int f_row = 0; f_row < first_matrix->rows; f_row++)
        {
            int value = 0;
            for (int s_column = 0; s_column < second_matrix->columns; s_column++)
            {
                value += get_matrix_value(first_matrix, f_row, s_column,2) * get_matrix_value(second_matrix, s_column, i,2);
            }

            set_tmp_matrix_value(*result_matrix, value, f_row, number);
        }
    }

}

matrix* multiply_matrices(matrix* first_matrix, matrix* second_matrix, char* result_file, unsigned int processes, double seconds, FILE* raport,char* separate){
    
    if(first_matrix->rows != second_matrix->columns){
        printf("%s", "Wrong declarations of size!");
        exit(1);
    }
    matrix* result_matrix = initialise_matrix(result_file, first_matrix->rows, second_matrix->columns,2);
    int columns_per_process = second_matrix->columns/processes;
    if(columns_per_process < 1){
        processes = first_matrix->rows;
    }
    if(!separate){
        for (int i = 0; i < processes; i++)
        {
            pid_t pid = fork();
            if (pid == 0){
                int number = 1;
                time_t start = time(NULL);
                for(int j = 0; j<columns_per_process; j++){
                    multiply_column(first_matrix, second_matrix, &result_matrix, i+j, processes);
                    time_t potential_end = time(NULL);
                
                    if(seconds<(difftime(potential_end,start))){
                        number = 0;
                        break;
                    }
                }



                fprintf(raport,"Proces PID: %i, wykonał %d mnożeń macierzy\n", getpid(), number);
                exit(0);
            }

            for (int j = 0; j<columns_per_process; j++)
            {
                wait(NULL);
            }
        }
    }
    else{

        for (int i = 0; i < processes; i++)
        {
            pid_t pid = fork();
            if (pid == 0){
                char* i_file = (char*)calloc(100, sizeof(char));
                strcpy(i_file, result_file);
                sprintf(i_file, "%d%s",i,result_file);
                matrix* tmp_matrix = initialise_matrix(i_file,first_matrix->rows, columns_per_process,0);
                free(i_file);

                int number = 1;
                time_t start = time(NULL);

                for(int j = 0; j<columns_per_process; j++){
                    multiply_column_separate(first_matrix, second_matrix, &tmp_matrix, i+j, processes,j, result_matrix->columns);
                    time_t potential_end = time(NULL);

                    print_matrix (tmp_matrix,0);
                    free(tmp_matrix);
                    if(seconds<(difftime(potential_end,start))){
                        number = 0;
                        break;
                    }
                }
                fprintf(raport,"Proces PID: %i, wykonał %d mnożeń macierzy\n", getpid(), number);
                exit(0);
            }

            for (int j = 0; j<columns_per_process; j++)
            {
                wait(NULL);
            }
        }


        pid_t pid = fork();

        if(pid == 0){
            char** args = (char**)calloc(processes + 2, sizeof(char*));
            args[0] = (char*)calloc(20, sizeof(char));
            strcpy(args[0], "paste");

            for(int i = 0; i<processes; i++){
                args[i+1] = (char*)calloc(100, sizeof(char));
                char* i_file = (char*)calloc(100, sizeof(char));
                strcpy(i_file, result_file);
                sprintf(i_file, "%d%s",i,result_file);
                args[i+1] = i_file;
            }
            args[processes+1] = NULL;

            int fd = open(result_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            dup2(fd, 1);
            close(fd);

            execvp(args[0], args); // trzy razy przekleja ten sam plik, zamiast trzy razy inny problemy z kopiowaniem pliku są oprócz tego wrzuca taby, nie pomaga to w niczym
        }
        else{
            wait(NULL);
        }
    }
    return result_matrix;
}

matrix* read_matrix_from_file(char* PATH,unsigned int rows, unsigned columns){
    FILE* file = fopen(PATH, "r+");

    if (file == NULL){
        printf("%s", "There is no file with such name!");
        exit(1);
    }

    matrix* result_matrix = malloc(sizeof(matrix));

    result_matrix->rows = rows;
    result_matrix->columns = columns;

    result_matrix->file = file;
    return result_matrix;   
}


gsl_matrix* generate_matrices(char* PATH, unsigned int rows, unsigned int columns){

    gsl_matrix* test_matrix = gsl_matrix_alloc(rows, columns);
    matrix* result_matrix = initialise_matrix(PATH, rows, columns,2);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            int value = rand() % 200;
            value -=100;
            set_matrix_value(result_matrix, value, r, c);
            value = get_matrix_value(result_matrix, r, c,2);
            gsl_matrix_set(test_matrix, r, c, value);
        }
    }
    return test_matrix;
}

void test_matrix_print(gsl_matrix* matrix_to_print, int rows, int columns){

    int value;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c<columns; c++){
            value = gsl_matrix_get(matrix_to_print, r, c);
            printf(" %d ", value);
        }
        printf("\n");
    }
    printf("\n\n");
}

int main(int arg_num, char** args)
{

    char* separate = NULL;
    char* first_arg = args[1];
    int processes = atoi(args[2]);
    double seconds = atof(args[3]);
    int type = atoi(args[4]);
    int MIN = atoi(args[5]);
    int MAX = atoi(args[6]);
    if(type == 1){
        separate = "a";
    }

    FILE* file = fopen(first_arg, "r");
    if(file == NULL){
        printf("%s","There is no such file!");
        exit(1);
    }
    char* first_file = malloc(20*sizeof(char));
    char* second_file = malloc(20*sizeof(char));
    char* result_file = malloc(20*sizeof(char));
    if(MIN <1){
        MIN = 1;
    }
    int r_n;
    int c_n; 
    srand((unsigned int)times(NULL));
    if(MAX == MIN){
        r_n =  MAX;
        c_n =  MAX;    
    }
    else{
        r_n =  rand()%(MAX-MIN);
        c_n =  rand()%(MAX-MIN);
        r_n += MIN;
        c_n +=MIN;
    }

    fscanf(file, "%s %s %s\n", first_file, second_file, result_file);

    FILE* raport = fopen("raport.txt", "w+");

    gsl_matrix* first_test = generate_matrices(first_file, r_n, c_n);
    gsl_matrix* second_test = generate_matrices(second_file, c_n, r_n); 


    matrix* first_proper = read_matrix_from_file(first_file,r_n, c_n);
    matrix* second_proper = read_matrix_from_file(second_file, c_n, r_n);


    print_matrix(first_proper,2);
    printf("\n\n");

    print_matrix(second_proper,2);
    printf("\n\n");

    matrix* proper_result = multiply_matrices(first_proper, second_proper, result_file, processes, seconds, raport, separate);

    print_matrix(proper_result,2);

    printf("\n\n");

    gsl_matrix* test_result =  gsl_matrix_alloc(r_n, r_n);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, first_test, second_test, 0.0, test_result); 

    test_matrix_print(first_test,r_n, c_n);

    test_matrix_print(second_test,c_n, r_n);

    test_matrix_print(test_result,r_n, r_n);

    fclose(raport);
    return 0;
}
