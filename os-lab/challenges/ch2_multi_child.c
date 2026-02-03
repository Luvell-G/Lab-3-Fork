#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\nExample: %s 5\n", argv[0], argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Error: N must be a positive integer.\n");
        return 1;
    }

    // Create N children
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // Child process
            printf("[child %d] PID=%d PPID=%d\n", i, getpid(), getppid());
            _exit(i + 1); // exit code = index + 1
        }
        // Parent continues loop to create next child
    }

    // Parent waits for all children and prints completion order
    printf("[parent] waiting for %d children...\n", N);

    for (int count = 0; count < N; count++) {
        int status;
        pid_t w = wait(&status);  // waits for ANY child

        if (w == -1) {
            perror("wait");
            return 1;
        }

        if (WIFEXITED(status)) {
            printf("[parent] child PID %d finished, exit status %d\n",
                   w, WEXITSTATUS(status));
        } else {
            printf("[parent] child PID %d did not exit normally\n", w);
        }
    }

    return 0;
}
