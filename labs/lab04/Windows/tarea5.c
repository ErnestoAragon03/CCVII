#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main() {
    SECURITY_ATTRIBUTES sa;
    
    // Configurar SECURITY_ATTRIBUTES para permitir herencia
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;


    //Crear Archivo de mapeo de memoria
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,               //Archivo Base (NULL para solo RAM)
        NULL,               //Seguridad (NULL para seguridad default)
        PAGE_READWRITE,     //Protección
        0,                  //Tamaño máximo para parte alta
        1024,               //Tamaño máximo para parte baja
        "MyMemoryShared"   //Nombre del objeto de memoria compartida
    );
    
    if (hMapFile == NULL){
        printf("Error al crear memoria compartida.\n");
        return 1;
    }

    //Mapear la memoria compartida al espacio de direcciones del proceso padre
    LPVOID pBuf = MapViewOfFile(
        hMapFile,               //Handler de memoria compartida
        FILE_MAP_ALL_ACCESS,    //Acceso (LECTURA/ESCRITURA)
        0,                      //Offset Alto
        0,                      //Offset Bajo
        0                       //Tamaño del mapeo (0 = todo)
    );

    if(pBuf==NULL){
        printf("Error al mapear memoria compartida.\n");
        return 1;
    }
    char* mensaje = "No gods, no masters";
    // Escribir en la memoria compartida
    strcpy((char*)pBuf, mensaje);
    printf("Parent Process: Writing: \"%s\" \n",mensaje);

    //Crear proceso hijo
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    if (CreateProcess(
        NULL,                   // No se especifica el nombre del módulo (usar línea de comandos)
        _T("./child_process_5.exe"), // Nombre del programa hijo (debe estar en el mismo directorio)
        NULL,                   // Atributos de seguridad del proceso (por defecto)
        NULL,                   // Atributos de seguridad del hilo (por defecto)
        FALSE,                  // No heredar handles
        0,                      // Sin banderas de creación
        NULL,                   // Usar el entorno del padre
        NULL,                   // Usar el directorio del padre
        &si,                    // Puntero a STARTUPINFO
        &pi                     // Puntero a PROCESS_INFORMATION
    )) {

        WaitForSingleObject(pi.hProcess, INFINITE);
        
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);

        // Cerrar los handles del proceso y del hilo
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else{
        printf("Error al crear el proceso hijo: %d\n", GetLastError());
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    return 0;
}