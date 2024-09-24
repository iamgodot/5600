#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  // Measure system call
  int fd[2];
  if (pipe(fd) == -1) {
    perror("Create pipe failed\n");
    exit(EXIT_FAILURE);
  }
  char buf[1];
  int loops = 1000000;
  int factor = 1000000;
  struct timeval start, end;

  // Measure io time
  gettimeofday(&start, NULL);
  for (int i = 0; i < loops; i++) {
    if (write(fd[1], buf, sizeof(buf)) != 1) {
      perror("Write failed\n");
    }
    if (read(fd[0], buf, sizeof(buf)) != 1) {
      perror("Read failed\n");
    }
  }
  gettimeofday(&end, NULL);
  float io_time = (float)(end.tv_sec * factor + end.tv_usec -
                          start.tv_sec * factor - start.tv_usec) /
                  loops;
  printf("Average time of system call 1 read and 1 write: %f microseconds\n",
         io_time);

  close(fd[0]);
  close(fd[1]);

  // Measure context switch
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(sched_getcpu(), &set);
  if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
    perror("Set affinity failed\n");
    exit(EXIT_FAILURE);
  }

  int fd_p1[2], fd_p2[2];
  if (pipe(fd_p1) == -1) {
    perror("Create pipe failed\n");
    exit(EXIT_FAILURE);
  }
  if (pipe(fd_p2) == -1) {
    perror("Create pipe failed\n");
    exit(EXIT_FAILURE);
  }

  int pid = fork();
  gettimeofday(&start, NULL);
  if (pid == -1) {
    perror("Fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Child process
    close(fd_p1[1]);
    close(fd_p2[0]);
    for (int i = 0; i < loops; i++) {
      if (read(fd_p1[0], buf, sizeof(buf)) == -1) {
        perror("Child read failed\n");
        exit(EXIT_FAILURE);
      }
      if (write(fd_p2[1], buf, sizeof(buf)) == -1) {
        perror("Child write failed\n");
        exit(EXIT_FAILURE);
      }
    }
    close(fd_p1[0]);
    close(fd_p2[1]);
    exit(EXIT_SUCCESS);
  } else {
    // Parent process
    close(fd_p1[0]);
    close(fd_p2[1]);
    for (int i = 0; i < loops; i++) {
      if (write(fd_p1[1], buf, sizeof(buf)) == -1) {
        perror("Parent write failed\n");
        exit(EXIT_FAILURE);
      }
      if (read(fd_p2[0], buf, sizeof(buf)) == -1) {
        perror("Parent read failed\n");
        exit(EXIT_FAILURE);
      }
    }
    wait(NULL);
    close(fd_p1[1]);
    close(fd_p2[0]);
  }
  gettimeofday(&end, NULL);
  // Here the io time needs to be deducted
  float context_switch_time =
      (float)(end.tv_sec * factor + end.tv_usec - start.tv_sec * factor -
              start.tv_usec - io_time * loops * 2) /
      loops;
  printf("Average time of context switch: %f microseconds\n",
         context_switch_time);
  exit(EXIT_SUCCESS);
}
