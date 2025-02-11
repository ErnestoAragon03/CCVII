#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Error en fork");
        return 1;
    } else if (pid == 0) {
        printf("[Hijo]  PID: %d\n", getpid());
    } else {
        waitpid(pid, NULL, 0);
        printf("[Padre] Hijo %d terminado\n", pid);
    }
    return 0;
}