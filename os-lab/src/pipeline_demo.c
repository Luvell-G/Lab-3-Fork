#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];
    if (pipe(fd) == -1) { perror("pipe"); exit(1); }

    pid_t pid1 = fork();
    if (pid1 < 0) { perror("fork"); exit(1); }

    if (pid1 == 0) {
        // Child 1: ls -> pipe write end
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls");
        _exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) { perror("fork"); exit(1); }

    if (pid2 == 0) {
        // Child 2: grep -> reads from pipe read end
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        // Change this pattern to something you know exists in your folder
        execlp("grep", "grep", "demo", (char *)NULL);
        perror("execlp grep");
        _exit(1);
    }

    // Parent: close both ends and wait
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
