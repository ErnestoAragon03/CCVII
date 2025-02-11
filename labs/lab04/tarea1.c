#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        return 1;
    } else if (pid == 0) {
        printf("[Hijo]  PID: %d, PPID: %d\n", getpid(), getppid());
    } else {
        sleep(1);
        printf("[Padre] PID: %d\n", getpid());
    }
    return 0;
}