#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(void) {
    pid_t kids[3];
    int sleeps[3] = {1, 2, 3};

    // Spawn 3 children
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // Child: sleep i+1 seconds then exit
            printf("[child %d] PID=%d sleeping %d sec\n", i, getpid(), sleeps[i]);
            sleep(sleeps[i]);
            _exit(0);
        }

        // Parent stores child PID
        kids[i] = pid;
        printf("[parent] created child %d with PID=%d\n", i, pid);
    }

    // Parent: non-blocking wait loop
    int finished = 0;
    while (finished < 3) {
        int status;
        pid_t w = waitpid(-1, &status, WNOHANG);

        if (w > 0) {
            // A child finished
            if (WIFEXITED(status)) {
                printf("[parent] child PID=%d finished (exit=%d)\n", w, WEXITSTATUS(status));
            } else {
                printf("[parent] child PID=%d finished (not normal exit)\n", w);
            }
            finished++;
        } else if (w == 0) {
            // No child finished yet (do NOT block)
            printf("[parent] polling... no child finished yet\n");
            usleep(200000); // 0.2 sec sleep to avoid busy-waiting
        } else {
            // w == -1 => error
            if (errno == EINTR) {
                continue; // interrupted, try again
            }
            perror("waitpid");
            return 1;
        }
    }

    printf("[parent] all children finished\n");
    return 0;
}
