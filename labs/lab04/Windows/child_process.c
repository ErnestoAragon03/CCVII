#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h> // Para usar la API de ToolHelp

// Función para obtener el PID del proceso padre
DWORD GetParentProcessId(DWORD pid) {
    DWORD ppid = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Crear un snapshot de los procesos
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        // Recorrer la lista de procesos
        if (Process32First(hSnapshot, &pe32)) {
            do {
                if (pe32.th32ProcessID == pid) { // Encontrar el proceso hijo
                    ppid = pe32.th32ParentProcessID; // Obtener el PID del padre
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot); // Cerrar el snapshot
    }
    return ppid;
}

int main() {
    // Obtener el PID del proceso actual
    DWORD pid = GetCurrentProcessId();

    // Obtener el PID del proceso padre
    DWORD ppid = GetParentProcessId(pid);

    // Imprimir el PID del proceso hijo y el PID del padre
    printf("Child Process: PID=%d, Parent PID=%d\n", pid, ppid);

    return 0;
}