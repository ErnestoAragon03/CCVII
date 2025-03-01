#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main() {
    SECURITY_ATTRIBUTES sa;
    
    // Configurar SECURITY_ATTRIBUTES para permitir herencia
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    HANDLE hReadPipe, hWritePipe;
    //Creación de Pipe para IPC
    if(CreatePipe(
        &hReadPipe,                 //Handler para lectura
        &hWritePipe,                //Handler para escritura
        &sa,                       //Opciones de seguridad
        0                           //Tamaño del buffer (0 para usar tamaño default del sistema)
    )){
        printf("Canal IPC entre Parent y Child Processes establecido con exito\n");
    } else{
        printf("No se ha podido establecer un canal IPC entre Parent y Child Processes\n");
        return 1;
    }
    
    //Configurar STARTUPINFO del Child process para acoplarlo al Pipe
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESTDHANDLES; 
    si.hStdInput = hReadPipe;       //Leer del Pipe
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);


    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Crear el proceso hijo
    if (CreateProcess(
        NULL,                   // No se especifica el nombre del módulo (usar línea de comandos)
        _T("./child_process_3.exe"), // Nombre del programa hijo (debe estar en el mismo directorio)
        NULL,                   // Atributos de seguridad del proceso (por defecto)
        NULL,                   // Atributos de seguridad del hilo (por defecto)
        TRUE,                   // Heredar handles (ESTO ES IMPORTANTE EN LA TAREA 3)
        0,                      // Sin banderas de creación
        NULL,                   // Usar el entorno del padre
        NULL,                   // Usar el directorio del padre
        &si,                    // Puntero a STARTUPINFO
        &pi                     // Puntero a PROCESS_INFORMATION
    )) {        
        //Cerrar extremo de lectura del padre
        //CloseHandle(hReadPipe);

        const char* mensaje = "I forgot to remember to forget\n";
        //Escribir en el Pipe
        printf("Parent Process: %s", mensaje);
        DWORD bytesEscritos;
        WriteFile(hWritePipe,           // Handle de escritura del pipe
            mensaje,                    // Buffer con los datos de escritura
            strlen(mensaje),            // Numero de bytes a escribir
            &bytesEscritos,             // Numero de bytes efectivamente escritos
            NULL                        // Opcional, Estructura para escrituras asincronas (NULL para sincrono)
        );

        //Cerrar extremo de escirtura (esto indica que ya no hay más datos)
        CloseHandle(hWritePipe);
        
        // Esperar a que el proceso hijo termine
        WaitForSingleObject(pi.hProcess, INFINITE);  //Si se le quiere poner un tiempo límite reemplazar INFINITE por el tiempo deseado en milisegundos

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