#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("Main process %d\n", (int)getpid());
  int fd = open("output.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    printf("Child process %d writing to file...\n", (int)getpid());
    dprintf(fd, "Child process %d\n", (int)getpid());
  } else {
    // Parent process
    printf("Parent process %d writing to file...\n", (int)getpid());
    dprintf(fd, "Parent process %d\n", (int)getpid());
  }
  return 0;
}
