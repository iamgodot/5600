#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Main process %d\n", (int) getpid());
    int x = 100;
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        x += 1;
        printf("Child process %d: x = %d\n", (int) getpid(), x);
    } else {
        // Parent process
        x -= 1;
        printf("Parent process %d: x = %d\n", (int) getpid(), x);
    }
    return 0;
}
