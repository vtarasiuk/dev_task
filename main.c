#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define STDERR stderr
#define INPUT_FILE_ARG_INDEX 1

#define NUMERIC_FORMAT "lld"
typedef long long int NumericType;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(STDERR, "Usage: %s <filepath>\n", argv[0]);
    return 1;
  }

  char *input_file_path = argv[INPUT_FILE_ARG_INDEX];
  FILE *input_file_stream = fopen(input_file_path, "r");
  if (input_file_stream == NULL) {
    fprintf(STDERR, "fopen - error opening a file: %s", strerror(errno));
    return 2;
  }

  const clock_t start_time = clock();

  NumericType number_read_from_file = 0;
  int return_value = 0;
  size_t i = 0;
  for (i = 0; ; i++) {
    return_value = fscanf(input_file_stream, "%" NUMERIC_FORMAT, &number_read_from_file);
    if (return_value == EOF || return_value == 0) {
      fprintf(STDERR, "End of file reached\n");
      break;
    }
  }

  const clock_t end_time = clock();
  const double time_spent = (double) (end_time - start_time) / CLOCKS_PER_SEC;
  fprintf(STDERR, "Time taken: %f seconds\n", time_spent);

  if (fclose(input_file_stream) != 0) {
    fprintf(STDERR, "fclose - error closing a file: %s", strerror(errno));
    return 3;
  }

  return 0;
}