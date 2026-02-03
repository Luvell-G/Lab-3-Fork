#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child: read numbers from pipe, sum them
        close(fd[1]); // close write end

        int sum = 0;
        int x;

        // Read integers until EOF (parent closes write end)
        while (read(fd[0], &x, sizeof(x)) == sizeof(x)) {
            sum += x;
        }

        close(fd[0]);
        printf("Sum = %d\n", sum);
        _exit(0);
    } else {
        // Parent: write numbers 1..10 into pipe
        close(fd[0]); // close read end

        for (int i = 1; i <= 10; i++) {
            if (write(fd[1], &i, sizeof(i)) != sizeof(i)) {
                perror("write");
                close(fd[1]);
                return 1;
            }
        }

        close(fd[1]);           // IMPORTANT: signals EOF to child
        waitpid(pid, NULL, 0);  // wait for child to finish
    }

    return 0;
}
