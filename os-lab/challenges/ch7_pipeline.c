#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pattern>\nExample: %s \"\\.c$\"\n", argv[0], argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Child 1: runs ls, writes to pipe
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        // stdout -> pipe write end
        close(fd[0]);                    // close read end
        dup2(fd[1], STDOUT_FILENO);      // redirect stdout to pipe
        close(fd[1]);                    // close original write fd

        execlp("ls", "ls", (char *)NULL);
        perror("execlp ls");
        _exit(1);
    }

    // Child 2: runs grep pattern, reads from pipe
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        // stdin <- pipe read end
        close(fd[1]);                    // close write end
        dup2(fd[0], STDIN_FILENO);       // redirect stdin from pipe
        close(fd[0]);                    // close original read fd

        execlp("grep", "grep", pattern, (char *)NULL);
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
