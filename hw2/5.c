#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    printf("Child process: %d\n", (int)getpid());
  } else {
    // Parent process
    int rc_wait = wait(NULL);
    printf("Parent of %d (rc_wait:%d): %d\n", pid, rc_wait, (int)getpid());
  }
  return 0;
}
