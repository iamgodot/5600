#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd[2];
  int pid;
  char buf[2];

  if (pipe(fd) == -1) {
    perror("Create pipe failed\n");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < 0) {
    perror("Fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Child process
    close(fd[0]);
    printf("hello\n");
    write(fd[1], "x", 1); // Signal to parent
    close(fd[1]);
    exit(EXIT_SUCCESS);
  } else {
    // Parent process
    close(fd[1]);
    while (read(fd[0], buf, 1) <= 0) {
    }
    printf("goodbye\n");
    close(fd[0]);
  }

  return 0;
}
