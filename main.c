#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <search.h>
#include <malloc.h>

#define STDERR stderr
#define INPUT_FILE_ARG_INDEX 1

#define NUMERIC_FORMAT "lld"
typedef long long int NumericType;
typedef long double   fNumericType;

inline NumericType calculateMaxNumber(NumericType max, NumericType value);
int compareNumericTypes(const void *arg1, const void *arg2);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(STDERR, "Usage: %s <filepath>\n", argv[0]);
        return 1;
    }

    char *input_file_path = argv[INPUT_FILE_ARG_INDEX];
    FILE *input_file_stream = fopen(input_file_path, "r");
    if (input_file_stream == NULL)
    {
        fprintf(STDERR, "fopen - error opening a file: %s\n", strerror(errno));
        return 2;
    }

    const clock_t start_time = clock();

    if (fseek(input_file_stream, 0, SEEK_END) != 0)
    {
        return 3;
    }
    const size_t input_file_size = ftell(input_file_stream);
    if (input_file_size == EOF)
    {
        return 4;
    }
    if (fseek(input_file_stream, 0, SEEK_SET) != 0)
    {
        return 5;
    }

    size_t alloc_size = 1;
    while (alloc_size < input_file_size / sizeof(NumericType) / 8)
    {
        alloc_size *= 2;
    }
    printf("alloc_size: %zu\n", alloc_size);

    NumericType *numbers = malloc(sizeof(NumericType) * alloc_size);
    if (numbers == NULL)
    {
        // other error handling
        return 6;
    }
    NumericType number_read_from_file = 0;
    if (fscanf(input_file_stream, "%" NUMERIC_FORMAT, &number_read_from_file) != 1)
    {
        return 7;
    }

    numbers[0] = number_read_from_file;
    fNumericType average = (fNumericType) number_read_from_file;

    int status_code = 0;
    size_t i;
    for (i = 1;; i++)
    {
        status_code = fscanf(input_file_stream, "%" NUMERIC_FORMAT, &number_read_from_file);
        if (status_code != 1)
        {
            if (status_code == 0)
            {
                fprintf(STDERR, "Error during fscanf call\n");
                break; // exit failure
            }
            fprintf(STDERR, "End of file reached\n");
            break; // exit failure
        }
        if (i == alloc_size)
        {
            alloc_size *= 2;
            NumericType *temp = realloc(numbers, alloc_size * sizeof(NumericType));
            if (temp == NULL)
            {
                fprintf(STDERR, "realloc - memory reallocation failed: %s\n", strerror(errno));
                free(numbers);
                // fclose ???
                return 8;
            }
            numbers = temp;
        }
        numbers[i] = number_read_from_file;
        average += (number_read_from_file - average) / (i + 1);
    }

    if (i != 1)
    {
        qsort(numbers, i, sizeof(NumericType), compareNumericTypes);
    }

    printf("alloc_size: %zu\n", alloc_size);
    printf("i = %zu\nmin: %lld\n", i, numbers[0]);
    printf("max: %lld\n", numbers[i - 1]);
    printf("average: %Lf\n", average);
    
    if (i % 2 == 0)
    {
        printf("median: %Lf\n", (fNumericType) (numbers[i / 2] + numbers[i / 2 + 1]) / 2);
    }
    else
    {
        printf("median: %lld\n", numbers[i / 2]);
    }

    free(numbers);

    const clock_t end_time = clock();
    const double time_spent = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    fprintf(STDERR, "Time taken: %f seconds\n", time_spent);

    if (fclose(input_file_stream) == EOF)
    {
        fprintf(STDERR, "fclose - error closing a file: %s", strerror(errno));
        return 9;
    }

    return 0;
}

NumericType calculateMaxNumber(NumericType max, NumericType value)
{
    return (value > max) ? value : max;
}

int compareNumericTypes(const void *arg1, const void *arg2)
{
    NumericType num1 = *(NumericType *)arg1;
    NumericType num2 = *(NumericType *)arg2;

    if (num1 < num2) return -1;
    if (num1 > num2) return 1;
    return 0;
}