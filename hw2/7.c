#include <stdio.h>
#include <unistd.h>

int main() {
  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    close(STDOUT_FILENO); // Close standard output
    printf("Child process: %d\n", (int)getpid());
  } else {
    // Parent process
    printf("Parent of %d: %d\n", pid, (int)getpid());
  }
  return 0;
}
