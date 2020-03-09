#ifndef mylib_h
#define myib_h

typedef struct pairs
{
    int files_number;
    char ***files;
}pairs;

typedef struct block
{
    int operations_number;
    char **diff_result;
}block;


typedef struct array_of_blocks
{
    int blocks_number;
    block **blocks;
}array_of_blocks;

array_of_blocks *initialise_array(int size);

pairs *make_pairs(char *sequence);

void diff_result(pairs *files);

void create_block(char *file_name, array_of_blocks *main_array);

int operationss_number(array_of_blocks *main_array, int index);

void delete_block(array_of_blocks *main_array, int index);

void delete_operation(block *spec_block, int index);

#endif