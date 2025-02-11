#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("[Padre] PID: %d\n", getpid());
    
    for (int i = 1; i <= 3; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            printf("[Hijo %d] PID: %d, PPID: %d\n", i, getpid(), getppid());
            break;
        } else {
            wait(NULL);
        }
    }
    return 0;
}