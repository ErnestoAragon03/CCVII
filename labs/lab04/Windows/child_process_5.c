#include <windows.h>
#include <stdio.h>
// Dummie process para tener algo que ejecutar
int main() {
    //Abrir el mapeo de memoria
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,        //Permisos (LECTURA/ESCRITURA)
        FALSE,                       //Permitir herencia
        "MyMemoryShared"            //Nombre del mapeo (Debe coincidir con el del padre)
    );
    if(hMapFile==NULL){
        printf("Error al abrir memoria compartida.\n");
        return 1;
    }
    //Mapear en el espacio de direcciones del proceso hijo
    LPVOID pBuf = MapViewOfFile(
        hMapFile,               //Handler de memoria compartida
        FILE_MAP_ALL_ACCESS,    //Acceso (LECTURA/ESCRITURA)
        0,                      //Offset Alto
        0,                      //Offset Bajo
        0                       //Tamaño del mapeo (0 = todo)
    );

    if(pBuf==NULL){
        printf("Error al mapear memoria compartida en el Child process.\n");
        return 1;
    }

    //Leer lo que escribió en memoria el padre
    printf("Child Process: Read \"%s\"", (char*)pBuf);

    return 0;
}