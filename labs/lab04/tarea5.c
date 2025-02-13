//Memoria compartida
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666 | IPC_CREAT); // Crea un segmento de memoria compartida
    //IPC_PRIVATE segmento no se compartirá con otros procesos
    //HM_SIZE es el tamaño de la memoria
    //IPC_CREAT indica que se debe crear el segmento si no existe
    //0666  cualquier pueda leer y escribir en el segmento de memoria compartida
    
    char *shm = shmat(shmid, NULL, 0); // Adjunta la memoria compartida
    
    pid_t pid = fork();
    
    if (pid == 0) { // Código del hijo
        sleep(1); // Asegura que el padre escriba primero
        printf("[Hijo] Memoria compartida: %s\n", shm);
        shmdt(shm); // Desvincula la memoria compartida
    } else {
        strcpy(shm, "Datos compartidos!"); // Escribe en la memoria compartida
        printf("[Padre] Escribiendo en memoria\n");
        fflush(stdout);
        wait(NULL); // Espera al hijo
        shmdt(shm); // Desvincula la memoria compartida
        shmctl(shmid, IPC_RMID, NULL); // Elimina la memoria compartida
    }
    return 0;
}