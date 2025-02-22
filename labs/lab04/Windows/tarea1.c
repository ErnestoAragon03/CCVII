//Crear un proceso hijo

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <windows.h>

int main() {
    pid_t pid = CreateProcess(); //creamos el proceso 

    if (pid < 0) {
        perror("Error en fork");
        return 1;
    } else if (pid == 0) { //proceso hijo
        printf("[Hijo]  PID: %d, PPID: %d\n", getpid(), getppid());
    
    } else {
        sleep(1); //proceso padre 
        printf("[Padre] PID: %d\n", getpid());
    }
    return 0;
}