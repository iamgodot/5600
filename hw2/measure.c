#define cpu_set_t
#define CPU_ZERO
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    //Measure system call
    int fd = open("output.txt", O_WRONLY|O_APPEND|O_CREAT, 0644);
    int loops = 1000000;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i=0;i<loops;i++) {
        read(fd, NULL, 0);
    }
    gettimeofday(&end, NULL);
    printf("Average time of system call read(): %f microseconds\n", (float) (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / loops);
    close(fd);

    //Measure context switch
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
        for (int i=0;i<loops;i++) {
            if (read(fd_p1[0], 'x', 1) == -1) {
                perror("Child read failed\n");
                exit(EXIT_FAILURE);
            }
            if (write(fd_p2[1], 'x', 1) == -1) {
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
        for (int i=0;i<loops;i++) {
            if (write(fd_p1[1], 'x', 1) == -1) {
                perror("Parent write failed\n");
                exit(EXIT_FAILURE);
            }
            if (read(fd_p2[0], 'x', 1) == -1) {
                perror("Parent read failed\n");
                exit(EXIT_FAILURE);
            }
        }
        wait(NULL);
        close(fd_p1[1]);
        close(fd_p2[0]);
    }
    gettimeofday(&end, NULL);
    printf("Average time of context switch: %f microseconds\n", (float) (end.tv_set * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / loops / 2);
    exit(EXIT_SUCCESS);
}
