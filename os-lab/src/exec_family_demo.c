#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void run_execlp() {
    printf("\n--- execlp demo (ls -l) ---\n");
    execlp("ls", "ls", "-l", (char *)NULL);
    perror("execlp");
    _exit(1);
}

void run_execv() {
    printf("\n--- execv demo (/bin/echo) ---\n");
    char *args[] = {"/bin/echo", "Hello from execv", NULL};
    execv("/bin/echo", args);
    perror("execv");
    _exit(1);
}

void run_execvp() {
    printf("\n--- execvp demo (echo) ---\n");
    char *args[] = {"echo", "Hello from execvp", NULL};
    execvp("echo", args);
    perror("execvp");
    _exit(1);
}

int main(void) {
    pid_t pid;

    // execlp
    pid = fork();
    if (pid == 0) run_execlp();
    waitpid(pid, NULL, 0);

    // execv
    pid = fork();
    if (pid == 0) run_execv();
    waitpid(pid, NULL, 0);

    // execvp
    pid = fork();
    if (pid == 0) run_execvp();
    waitpid(pid, NULL, 0);

    printf("\n[parent] all exec demos finished\n");
    return 0;
}
