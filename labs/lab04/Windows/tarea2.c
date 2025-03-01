#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main() {
    // Proceso padre
    printf("Parent Process: PID=%d\n", GetCurrentProcessId());  //Notar que es lo mismo que hacer GetProcessId(GetCurrentProcess())

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Inicializar las estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Crear el proceso hijo
    if (CreateProcess(
        NULL,                   // No se especifica el nombre del módulo (usar línea de comandos)
        _T("./child_process.exe"), // Nombre del programa hijo (debe estar en el mismo directorio)
        NULL,                   // Atributos de seguridad del proceso (por defecto)
        NULL,                   // Atributos de seguridad del hilo (por defecto)
        FALSE,                  // No heredar handles
        0,                      // Sin banderas de creación
        NULL,                   // Usar el entorno del padre
        NULL,                   // Usar el directorio del padre
        &si,                    // Puntero a STARTUPINFO
        &pi                     // Puntero a PROCESS_INFORMATION
    )) {
        printf("Child Process ID: PID=%d      Parent PID=%d\n", pi.dwProcessId, GetCurrentProcessId());
        
        // Esperar a que el proceso hijo termine
        DWORD codigo = WaitForSingleObject(pi.hProcess, INFINITE);  //Si se le quiere poner un tiempo límite reemplazar INFINITE por el tiempo deseado en milisegundos
        if (codigo == WAIT_OBJECT_0){
            printf("El proceso hijo ha terminado correctamente.\n");
        }
        else if(codigo == WAIT_TIMEOUT){
            printf("El tiempo de espera especificado ha expirado.  El proceso hijo no se ha concluido.\n");
        }
        else if(codigo == WAIT_FAILED){
            printf("Ha ocurrido un error en el proceso hijo.\n");
        }

        // Cerrar los handles del proceso y del hilo
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else{
        printf("Error al crear el proceso hijo: %d\n", GetLastError());
        return 1;
    }

    return 0;
}