#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Inicializar las estructuras
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Crear el proceso hijo
    if (!CreateProcess(
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
        printf("Error al crear el proceso hijo: %d\n", GetLastError());
        return 1;
    }

    // Proceso padre
    printf("Parent Process: PID=%d\n", GetCurrentProcessId());
    printf("Probando GetProcessId; PID=%d\n", GetProcessId(&pi));

    // Esperar a que el proceso hijo termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Cerrar los handles del proceso y del hilo
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}