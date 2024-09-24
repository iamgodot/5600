#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  int pid1, pid2;

  if (pipe(fd) == -1) {
    perror("Create fd failed\n");
    exit(EXIT_FAILURE);
  }

  pid1 = fork();
  if (pid1 < 0) {
    perror("Fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid1 == 0) {
    // Child process 1
    printf("Child process: %d\n", (int)getpid());

    close(fd[0]);               // Close unused read end
    dup2(fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the fd
    close(fd[1]);

    execlp("ls", "ls", "-l", NULL);
    perror("Execute ls failed\n");
    exit(EXIT_FAILURE);
  }

  pid2 = fork();
  if (pid2 < 0) {
    perror("Fork failed\n");
    exit(EXIT_FAILURE);
  } else if (pid2 == 0) {
    // Child process 2
    printf("Child process: %d\n", (int)getpid());

    close(fd[1]);              // Close unused write end
    dup2(fd[0], STDIN_FILENO); // Redirect stdin to the read end of the fd
    close(fd[0]);

    execlp("wc", "wc", "-l", NULL);
    perror("Execute wc failed\n");
    exit(EXIT_FAILURE);
  }

  close(fd[0]);
  close(fd[1]);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}
