#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pidA = fork();

    if (pidA < 0) {
        perror("fork");
        return 1;
    }

    if (pidA == 0) {
        // Child A: execl with explicit args
        execl("/bin/echo", "echo", "one", "two", (char *)NULL);

        // If execl returns, it failed
        perror("execl");
        _exit(1);
    }

    pid_t pidB = fork();

    if (pidB < 0) {
        perror("fork");
        return 1;
    }

    if (pidB == 0) {
        // Child B: execv with argv array
        char *argv[] = { "echo", "one", "two", NULL };
        execv("/bin/echo", argv);

        // If execv returns, it failed
        perror("execv");
        _exit(1);
    }

    // Parent waits for both children
    waitpid(pidA, NULL, 0);
    waitpid(pidB, NULL, 0);

    printf("[parent] both children finished\n");
    return 0;
}
