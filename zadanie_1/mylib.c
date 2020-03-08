#include <stdio.h>
#include "mylib.h"
#include <stdlib.h>
#include <string.h>


const int SIZE = 10000;

array_of_blocks *initialise_array(int size)
{
    array_of_blocks *main_array = (array_of_blocks *)calloc(1, sizeof(array_of_blocks));
    main_array->blocks_number = 0;
    main_array->blocks = (block **)calloc(size, sizeof(block *));
    return main_array;
}





pairs *make_pairs(char sequence[])
{    
    char sequence_copy[strlen(sequence)];
    strcpy(sequence_copy, sequence); // copy string so we will use that later

                                 
    int number_of_files = 1;
    for(int i = 0; i<strlen(sequence);i++)
    {
        if(sequence[i] == ' ' || sequence[i] == ':')
        {
          number_of_files++;
        }
    }

    number_of_files /=2; // tu bardziej sprawdzamy liczbę par plików, które będziemy porównywać

    pairs *pair_seq = calloc(1, sizeof(pairs));
    pair_seq->files_number = number_of_files;
    pair_seq->files = calloc(number_of_files, sizeof(char **));
    char *file_name = strtok(sequence_copy, " :");
    int i = 0;
    
    while (file_name != NULL)
    { // init space for row
        pair_seq->files[i] = calloc(2, sizeof(char *));
        // enter first name

        pair_seq->files[i][0] = calloc(1, strlen(file_name));
        strcpy(pair_seq->files[i][0], file_name);
        file_name = strtok(NULL, " :");

        // enter second name
        pair_seq->files[i][1] = calloc(1, strlen(file_name));
        strcpy(pair_seq->files[i][1], file_name);
        file_name = strtok(NULL, " :");
        i++;
    }
    return pair_seq;


}

void do_diff(pairs *files)
{
    char ***files_pairs = files->files;
    for (int i = 0; i < files->files_number; i++)
    {
        char **curr_pair = files_pairs[i];


        char *tmp_file_name = calloc(1, strlen(curr_pair[0]) + strlen(curr_pair[1]) + 2);

        strcpy(tmp_file_name, curr_pair[0]);
        strcat(tmp_file_name, ":");
        strcat(tmp_file_name, curr_pair[1]);

        //sekcja tworzenia nowego pliku z wynikami
        char *touch_command = calloc(1, strlen(tmp_file_name) + 6);
        strcpy(touch_command, "touch ");
        strcat(touch_command, tmp_file_name);      
        system(touch_command); 

        //sekcja wykonywania komendydiff
        char diff_command[12 + strlen(curr_pair[0]) + strlen(curr_pair[1])];
        strcpy(diff_command, "diff ");
        strcat(diff_command, curr_pair[0]);
        strcat(diff_command, " ");
        strcat(diff_command, curr_pair[1]);
        strcat(diff_command, " >> ");
        strcat(diff_command, tmp_file_name);

        system(diff_command);
        free(tmp_file_name);
        free(touch_command);
    }
}


void create_block(char *file_name, array_of_blocks *main_array)
{

    FILE *result = fopen(file_name, "r");

    if (result == NULL)
    {
        printf("%s", "There is no file with that name");
        return;
    }

    int line_number = 0;
    int operations_quant = 0;
    char *line = NULL;
    char **line_collector = (char **)calloc(SIZE, sizeof(char *)); // file text
    
    size_t _ = 0; 
    while (getline(&line, &_, result) != -1)
    {
        line_collector[line_number] = calloc(SIZE, sizeof(char));
        strcpy(line_collector[line_number], "");
        strcat(line_collector[line_number], line);
        line_number++;
    }


    //zamyka i usuwa plik tymczasowy
    fclose(result);
    char *rm_command = calloc(1, strlen(file_name) + 3);
    strcpy(rm_command, "rm ");
    strcat(rm_command, file_name);
    system(rm_command);
    free(line);


    block *new_block = calloc(1, sizeof(block));
    char **operations = calloc(line_number, sizeof(char *));

    int j = 0;
    while (j < line_number)
    {
        char *curr_line = line_collector[j];

        if (curr_line[0] >= '0' && curr_line[0] <= '9') // nowe pole dla bloku, z tego, co zauważyłem, to każdy blok bloku zaczyna się od cyfry
        {
            operations_quant++;
            operations[operations_quant-1] = calloc(SIZE, sizeof(char));
            strcpy(operations[operations_quant-1], "");
        }
        if (operations_quant > 0)
        {
            strcat(operations[operations_quant - 1], curr_line);
        }

        free(line_collector[j]);
        line_collector[j] = NULL;
        j++;
    }

    free(line_collector);

    new_block->operations_number = operations_quant;
    new_block->diff_result = operations;


    int index = 0;
    while (main_array->blocks[index] != NULL && index < main_array->blocks_number)
    {
        index++;
    }
    main_array->blocks[index] = new_block;
    main_array->blocks_number++;
    return;
}

int operationss_number(array_of_blocks *main_array, int index)
{
    if (main_array->blocks_number <= index || main_array->blocks[index] == NULL)
    {
        printf("%s","Block with such index does not exists");
        return 0;
    }
    return main_array->blocks[index]->operations_number;
}


void delete_block(array_of_blocks *main_array, int index)
{
    if (main_array->blocks_number <= index || main_array->blocks[index] == NULL)
    {
        printf("%s", "Block with such index does not exists");
    }
    else
    {
        block *spec_block = main_array->blocks[index];
        for (int i = 0; i < spec_block->operations_number; i++)
        {
           if(spec_block->diff_result[i] != NULL) // bo mogliśmy operację wcześniej usunąć
            {
              delete_operation(spec_block, i);
            }
        }
        free(main_array->blocks[index]);
        main_array->blocks[index] = NULL;
        main_array->blocks_number--;
    }
}

void delete_operation(block *spec_block, int index)
{
    if (spec_block->operations_number <= index || spec_block->diff_result[index] == NULL)
    {
        printf("%s","Operation with such index does not exists");
    }
    else
    {
        free(spec_block->diff_result[index]);
        spec_block->diff_result[index] = NULL;
        spec_block->operations_number--;
    }
    return;
}