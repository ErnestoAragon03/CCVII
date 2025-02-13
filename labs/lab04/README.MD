
# **Documentación: Lab004 - Creación y Gestión de Procesos**  
**Sistemas Operativos (Windows/Linux)**  

## Video de YouTube
[Ver video](||)
---

---
## **Colaboradores**
- [Poncho Ajmv](https://github.com/poncho-ajmv)
- [Ernesto Aragon](https://github.com/ErnestoAragon03)
- [Christian Sosa](https://github.com/ChristianSosa22)
---
# Administración de Procesos en Linux y Windows

## 🔹 Linux - Funciones para Procesos

- **`fork()`**: Crea un nuevo proceso hijo duplicando el proceso padre.
- **`exec()`**: Reemplaza el código del proceso actual por otro programa.
- **`wait()` / `waitpid()`**: Hace que el proceso padre espere la finalización de un hijo.
- **`pipe()`**: Crea un canal de comunicación unidireccional entre procesos.
- **`shmget()` / `shmat()`**: Permiten la creación y uso de memoria compartida.


## 🔹 Windows - Funciones para Procesos

- **`CreateProcess()`**: Crea un nuevo proceso hijo y ejecuta otro programa.
- **`WaitForSingleObject()`**: Hace que el proceso padre espere a un proceso hijo.
- **`CreatePipe()`**: Crea un canal de comunicación entre procesos.
- **`CreateFileMapping()` / `MapViewOfFile()`**: Permiten compartir memoria entre procesos.
---

## 🔹 Bibliotecas
- **`stdio.h:`**: Para funciones de entrada/salida estándar como printf.
- **`stdlib.h:`**: Para funciones generales, como la gestión de memoria.
- **`string.h:`**: Para funciones de manipulación de cadenas, como strcpy.
- **`unistd.h: `**: Para funciones de POSIX como fork y sleep.
- **`sys/shm.h:`**: Para trabajar con memoria compartida.
- **`sys/wait.h:`**: Para manejar la espera de procesos, como wait.
---

## Tareas Principales  

### 1. Creación de un Proceso Hijo  
**Implementación:**  
- **Linux:** Usar `fork()` para duplicar el proceso. Diferenciar padre/hijo mediante el valor retornado.  
- **Windows:** Emplear `CreateProcess()` para generar un proceso independiente.  

**Resultado Esperado:**  
- Padre muestra su PID.  
- Hijo muestra su PID y el PPID del padre.  

### 2. Sincronización Padre-Hijo  
**Implementación:**  
- **Linux:** Invocar `waitpid()` para bloquear al padre hasta que el hijo finalice.  
- **Windows:** Usar `WaitForSingleObject()` con el manejador del proceso hijo.  

**Resultado Esperado:**  
- Padre confirma la finalización del hijo después de su ejecución.  

### 3. Comunicación mediante Tuberías  
**Implementación:**  
- Crear tubería con `pipe()` (Linux) o `CreatePipe()` (Windows).  
- Padre escribe en la tubería, hijo lee desde ella.  

**Resultado Esperado:**  
- Hijo imprime el mensaje enviado por el padre.  

### 4. Múltiples Procesos Hijos  
**Implementación:**  
- Generar tres hijos mediante un bucle con `fork()` (Linux) o `CreateProcess()` (Windows).  
- Asegurar que cada hijo muestre su información antes de terminar.  

**Resultado Esperado:**  
- Tres procesos hijos con PIDs únicos y el mismo PPID (PID del padre).  

### 5. Memoria Compartida (Avanzado)  
**Implementación:**  
- **Linux:** Usar `shmget()` para reservar memoria y `shmat()` para mapearla.  
- **Windows:** Emplear `CreateFileMapping()` y `MapViewOfFile()`.  

**Resultado Esperado:**  
- Hijo accede y muestra los datos escritos por el padre en la memoria compartida.  

---

## Consideraciones Técnicas  
- **Diferencias entre Sistemas:**  
  - En Windows, los procesos son independientes (no heredan memoria), mientras que en Linux, `fork()` crea una copia inicial.  
  - Las tuberías en Windows requieren manejadores (`HANDLE`), a diferencia de los descriptores de archivo en Linux.  
- **Gestión de Recursos:**  
  - Liberar memoria compartida con `shmdt()` (Linux) o `UnmapViewOfFile()` (Windows).  
  - Cerrar descriptores de archivo/manejadores después de su uso.  


