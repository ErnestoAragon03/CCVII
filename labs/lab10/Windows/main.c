#include "student.h"

// Función principal
int main() {
    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    int num_estudiantes = 5; // Número de estudiantes
    Estudiante* estudiantes[num_estudiantes];

    // Crear los estudiantes
    for (int i = 0; i < num_estudiantes; i++) {
        estudiantes[i] = crear_estudiante(i + 1);
    }

    // Destruir los estudiantes
    for (int i = 0; i < num_estudiantes; i++) {
        destruir_estudiante(estudiantes[i]);
    }

    printf("Todos los estudiantes han cruzado.\n");
    return 0;
}