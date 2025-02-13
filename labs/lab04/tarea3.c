//Comunicación entre procesos usando tuberías

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2]; // Descriptores de la tubería
    char mensaje[] = "Mensaje secreto del padre";
    char buffer[100];

    //creamos la tuberia
    if (pipe(fd) == -1) {
        perror("Pipe error");
        return 1;
    }

    pid_t pid = fork();
    
    //hijo
    if (pid == 0) {
        close(fd[1]); // Cierra el extremo dfe  write
        read(fd[0], buffer, sizeof(buffer)); //leemos
        printf("[Hijo] Recibido: %s\n", buffer);
        close(fd[0]);
    } 
    //padrfe
    else {
        close(fd[0]); // Cierra el extremo dfe  read
        write(fd[1], mensaje, strlen(mensaje) + 1); //escribimos
        printf("[Padre] Mensaje enviado\n");
        fflush(stdout);
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}