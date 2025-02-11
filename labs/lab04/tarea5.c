#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // <- Cabecera añadida
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    int shmid = shmget(IPC_PRIVATE, SHM_SIZE, 0666|IPC_CREAT);
    char *shm = shmat(shmid, NULL, 0);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        sleep(1);
        printf("[Hijo]  Memoria compartida: %s\n", shm);
        shmdt(shm);
    } else {
        strcpy(shm, "Datos compartidos!");
        printf("[Padre] Escribiendo en memoria\n");
        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}