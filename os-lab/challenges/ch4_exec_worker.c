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
        // Child: exec worker with custom environment
        char *argv[] = { "./worker", "alpha", "beta", NULL };
        char *envp[] = { "MYVAR=hello", NULL };

        execve("./worker", argv, envp);

        // If execve returns, it failed
        perror("execve");
        _exit(1);
    } else {
        // Parent: wait for child to finish
        int status;
        waitpid(pid, &status, 0);
        printf("[parent] worker finished\n");
    }

    return 0;
}
