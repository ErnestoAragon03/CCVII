//Creación de múltiples procesos hijos

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("[Padre] PID: %d\n", getpid());
    
    for (int i = 1; i <= 3; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Error en fork");
            return 1;
        } 
        else if (pid == 0) {  // Proceso hijo
            printf("[Hijo %d] PID: %d, PPID: %d\n", i, getpid(), getppid());
            return 0;  //Evita que los hijos sigan creando más procesos
        }
    }

    // El padre espera a que todos los hijos terminen
    for (int i = 1; i <= 3; i++) {
        wait(NULL);
    }
    
    return 0;
}
