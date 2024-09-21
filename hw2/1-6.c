#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: %d\n", (int) getpid());
    } else {
        // Parent process
        waitpid(pid, NULL, 0);
        printf("Parent of %d: %d\n", pid, (int) getpid());
    }
    return 0;
}
