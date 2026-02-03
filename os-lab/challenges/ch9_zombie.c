#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("[child] PID=%d exiting now\n", getpid());
        _exit(0);
    } else {
        printf("[parent] PID=%d created child PID=%d\n", getpid(), pid);
        printf("[parent] waiting immediately (no zombie should persist)\n");
        waitpid(pid, NULL, 0);
        printf("[parent] child reaped immediately\n");
    }

    return 0;
}
