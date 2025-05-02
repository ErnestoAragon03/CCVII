#include "stdio.h"

// Declaraciones de las funciones de los procesos
extern void process1_main(void);
extern void process2_main(void);

void main() {

    PRINT("Bienvenido al sistema\n");


    //Aqui se tiene un bug lo primero que se manda el read 
    // no lo enteinde hasta el segundo algun /n puede ser revisar
    char nombre[100];
    int edad;

    PRINT("Ingresa tu nombre: ");
    READ("%s", nombre);
    PRINT("Hola, %s\n", nombre);

    PRINT("Ingresa tu edad: ");
    READ("%d", &edad);
    PRINT("Tienes %d años\n", edad);



    PRINT("Ejecutando Proceso 1:\n");
    //process1_main();

    PRINT("Ejecutando Proceso 2:\n");
    //process2_main();

    while (1);  // No retornar nunca
}
    