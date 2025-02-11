#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    char mensaje[] = "Mensaje secreto del padre";
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("Pipe error");
        return 1;
    }

    pid_t pid = fork();
    
    if (pid == 0) {
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("[Hijo]  Recibido: %s\n", buffer);
        close(fd[0]);
    } else {
        close(fd[0]);
        write(fd[1], mensaje, strlen(mensaje)+1);
        printf("[Padre] Mensaje enviado\n");
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}