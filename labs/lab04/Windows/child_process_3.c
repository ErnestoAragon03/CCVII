#include <windows.h>
#include <stdio.h>
// Dummie process para tener algo que ejecutar
int main() {
    char buffer[128];
    DWORD bytesLeidos;

    //Leer desde la entrada estandar, (pipe heredado del padre)
    HANDLE hReadPipe = GetStdHandle(STD_INPUT_HANDLE);

    if(ReadFile(
        hReadPipe,              // Handle de lectura del pipe
        buffer,                 // Buffer donde se guardaran los datos leídos
        sizeof(buffer) - 1,     // Numero de bytes a leer
        &bytesLeidos,           // Puntero a variable que recibe los bytes realmente leidos
        NULL                    // Para operaciones asincronas (NULL indica sincrono)
    )){
        buffer[bytesLeidos] == '\0';
        printf("Child Process: Received: %s \n",buffer);
    } else {
        printf("Error durante la comunicacion de parte de Child");
    }
    return 0;
}