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
        // Child: replace process with "ls -la"
        execlp("ls", "ls", "-la", (char *)NULL);

        // If execlp returns, it failed
        perror("execlp");
        _exit(1);
    } else {
        // Parent: wait for child
        waitpid(pid, NULL, 0);
        printf("[parent] child finished running ls -la\n");
    }

    return 0;
}
