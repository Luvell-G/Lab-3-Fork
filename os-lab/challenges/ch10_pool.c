#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

// Simple "task": open a file, count lines + bytes, print result.
// If file can't be opened, child exits with 1.
static int process_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 1;

    long lines = 0;
    long bytes = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        bytes++;
        if (c == '\n') lines++;
    }
    fclose(f);

    printf("[worker %d] processed '%s' -> lines=%ld bytes=%ld\n",
           getpid(), path, lines, bytes);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr,
                "Usage: %s M task1 [task2 ...]\n"
                "Example: %s 3 file1 file2 file3\n",
                argv[0], argv[0]);
        return 1;
    }

    int M = atoi(argv[1]);
    if (M <= 0) {
        fprintf(stderr, "Error: M must be a positive integer.\n");
        return 1;
    }

    char **tasks = &argv[2];
    int num_tasks = argc - 2;

    int next_task = 0;   // index into tasks[]
    int running = 0;     // how many children currently running
    int finished = 0;

    // Spawn initial workers up to M
    while (running < M && next_task < num_tasks) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // Child worker
            const char *t = tasks[next_task];
            printf("[worker %d] START task '%s'\n", getpid(), t);

            int rc = process_file(t);

            printf("[worker %d] FINISH task '%s' (exit=%d)\n", getpid(), t, rc);
            _exit(rc);
        }

        // Parent
        printf("[parent %d] spawned worker PID=%d for task '%s'\n",
               getpid(), pid, tasks[next_task]);
        running++;
        next_task++;
    }

    // As workers finish, spawn new ones until all tasks are processed
    while (finished < num_tasks) {
        int status;
        pid_t w = waitpid(-1, &status, 0); // block until ANY child finishes
        if (w == -1) {
            if (errno == EINTR) continue;
            perror("waitpid");
            return 1;
        }

        running--;
        finished++;

        if (WIFEXITED(status)) {
            printf("[parent %d] worker PID=%d done (exit status=%d)\n",
                   getpid(), w, WEXITSTATUS(status));
        } else {
            printf("[parent %d] worker PID=%d ended abnormally\n", getpid(), w);
        }

        // Spawn next task if any remain, keeping running <= M
        if (next_task < num_tasks) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                return 1;
            }

            if (pid == 0) {
                const char *t = tasks[next_task];
                printf("[worker %d] START task '%s'\n", getpid(), t);

                int rc = process_file(t);

                printf("[worker %d] FINISH task '%s' (exit=%d)\n", getpid(), t, rc);
                _exit(rc);
            }

            printf("[parent %d] spawned worker PID=%d for task '%s'\n",
                   getpid(), pid, tasks[next_task]);
            running++;
            next_task++;
        }
    }

    printf("[parent %d] ALL TASKS COMPLETE (processed=%d, max_concurrency=%d)\n",
           getpid(), num_tasks, M);
    return 0;
}
