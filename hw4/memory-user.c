#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MB (1024 * 1024)

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s <memory_in_MB> [duration_in_seconds]\n",
            argv[0]);
    return 1;
  }

  long memory_size = strtol(argv[1], NULL, 10) * MB;
  long duration = (argc == 3) ? strtol(argv[2], NULL, 10) : -1;

  char *buffer = (char *)malloc(memory_size);
  if (buffer == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }

  printf("Allocated %ld MB of memory\n", memory_size / MB);

  time_t start_time = time(NULL);
  long index = 0;

  while (1) {
    buffer[index] = 'A';
    index = (index + 1) % memory_size;

    if (index == 0) {
      printf("Completed one full cycle through the allocated memory\n");
    }

    if (duration > 0 && difftime(time(NULL), start_time) >= duration) {
      printf("Duration reached. Exiting.\n");
      break;
    }
  }

  free(buffer);
  return 0;
}
