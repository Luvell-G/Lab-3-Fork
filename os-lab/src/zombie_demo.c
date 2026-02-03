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
        // Child exits immediately
        printf("[child] PID=%d exiting now\n", getpid());
        _exit(0);
    } else {
        // Parent sleeps so child becomes a zombie briefly
        printf("[parent] PID=%d created child PID=%d\n", getpid(), pid);
        printf("[parent] sleeping 10 seconds... (child is zombie during this)\n");
        sleep(10);

        printf("[parent] now calling waitpid()\n");
        waitpid(pid, NULL, 0);
        printf("[parent] child reaped, zombie is gone\n");
    }

    return 0;
}
