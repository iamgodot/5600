#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s pages trials\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  long PAGESIZE = sysconf(_SC_PAGESIZE);
  long jump = PAGESIZE / sizeof(int);
  int pages = atoi(argv[1]);
  int trials = atoi(argv[2]);
  int *a = calloc(pages, PAGESIZE);

  struct timespec start, end;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
  for (int j = 0; j < trials; j++) {
    for (int i = 0; i < pages * jump; i += jump)
      a[i] += 1;
  }
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

  printf("%f\n",
         ((end.tv_sec - start.tv_sec) * 1E9 + end.tv_nsec - start.tv_nsec) /
             (trials * pages));
  free(a);
  return 0;
}
