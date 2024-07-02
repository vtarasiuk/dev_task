#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE_ARG_INDEX 1

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
    return 1;
  }

  char *input_file_path = argv[INPUT_FILE_ARG_INDEX];
  FILE *input_file_stream = fopen(input_file_path, "r");
  if (input_file_stream == NULL) {
    perror("fopen - error opening file");
    return 2;
  }

  const clock_t start_time = clock();



  const clock_t end_time = clock();
  double time_spent = (double) (end_time - start_time) / CLOCKS_PER_SEC;
  fprintf(stderr, "Time taken: %f seconds\n", time_spent);

  if (fclose(input_file_stream) != 0) {
    perror("fclose - error closing file");
    return 3;
  }

  return 0;
}