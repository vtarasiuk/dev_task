#include "dev_task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <search.h>
#include <malloc.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(STDERR, "Usage: %s <filepath>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_file_path = argv[INPUT_FILE_ARG_INDEX];
    FILE *input_file_stream = fopen(input_file_path, "r");
    if (input_file_stream == NULL)
    {
        fprintf(STDERR, "fopen - error opening a file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    const clock_t start_time = clock();

    const size_t input_file_size = fileSize(input_file_stream);
    if (input_file_size == EOF)
    {
        fprintf(STDERR, "Can't calculate file size\n");
        return EXIT_FAILURE;
    }

    size_t alloc_size = 1;
    while (alloc_size < input_file_size / sizeof(NumericType) / CHAR_BIT)
    {
        alloc_size *= 2;
    }
    printf("alloc_size: %zu\n", alloc_size);

    NumericType *numbers = malloc(sizeof(NumericType) * alloc_size);
    if (numbers == NULL)
    {
        fclose(input_file_stream);
        return EXIT_FAILURE;
    }
    NumericType number_read_from_file = 0;
    fNumericType average = 0;

    int status_code = 0;
    size_t arr_size = 0;
    for (size_t i = 0; ; ++i)
    {
        status_code = fscanf(input_file_stream, "%" NUMERIC_FORMAT, &number_read_from_file);
        if (status_code != 1)
        {
            if (status_code == 0)
            {
                fprintf(STDERR, "Couldn't read a number from a file\n");
                free(numbers);
                fclose(input_file_stream);
                return EXIT_FAILURE;
            }
            fprintf(STDERR, "End of file reached\n");
            arr_size = i;
            break;
        }
        if (i == alloc_size)
        {
            alloc_size *= 2;
            NumericType *temp = realloc(numbers, alloc_size * sizeof(NumericType));
            if (temp == NULL)
            {
                fprintf(STDERR, "realloc - memory reallocation failed: %s\n", strerror(errno));
                free(numbers);
                fclose(input_file_stream);
                return EXIT_FAILURE;
            }
            numbers = temp;
        }
        numbers[i] = number_read_from_file;
        average += (number_read_from_file - average) / (i + 1);
    }

    if (arr_size != 1)
    {
        qsort(numbers, arr_size, sizeof(NumericType), compareNumericTypes);
    }

    printf("alloc_size: %zu\n", alloc_size);
    printf("arr_size = %zu\nmin: %lld\n", arr_size, numbers[0]);
    printf("max: %lld\n", numbers[arr_size - 1]);
    printf("average: %Lf\n", average);
    
    if (arr_size % 2 == 0)
    {
        printf("median: %Lf\n", (fNumericType) (numbers[arr_size / 2] + numbers[arr_size / 2 + 1]) / 2);
    }
    else
    {
        printf("median: %lld\n", numbers[arr_size / 2]);
    }

    free(numbers);

    const clock_t end_time = clock();
    const double time_spent = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(STDERR, "Time taken: %f seconds\n", time_spent);

    if (fclose(input_file_stream) == EOF)
    {
        fprintf(STDERR, "fclose - error closing a file: %s", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int compareNumericTypes(const void *arg1, const void *arg2)
{
    NumericType diff = *((NumericType*)arg1) - *((NumericType*)arg2);
    if (diff > 0)
    {
        return 1;
    }
    else if (diff < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

long fileSize(FILE* stream)
{
    if (fseek(stream, 0, SEEK_END) != 0)
    {
        return EOF;
    }

    long file_size = ftell(stream);
    if (file_size == -1L)
    {
        return EOF;
    }

    if (fseek(stream, 0, SEEK_SET) != 0)
    {
        return EOF;
    }

    return file_size;
}
