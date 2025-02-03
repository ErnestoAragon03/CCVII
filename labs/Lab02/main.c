#include "stdio.h"
#include "os.h"

#define TIMER_BASE      0x101E2000
#define TIMER_LOAD      (*(volatile int *)(TIMER_BASE + 0x00))
#define TIMER_VALUE     (*(volatile int *)(TIMER_BASE + 0x04))
#define TIMER_CONTROL   (*(volatile int *)(TIMER_BASE + 0x08))
#define TIMER_INTCLR    (*(volatile int *)(TIMER_BASE + 0x0C))
#define TIMER_RIS       (*(volatile int *)(TIMER_BASE + 0x10))

#define VIC_BASE 0x10140000
#define VIC_IRQ_STATUS (*(volatile int *) (VIC_BASE + 0x00))
#define PIC_VECT_ADDR (*(volatile int *) (VIC_BASE + 0x30))

void main() {
    int int1 = 0, int2 = 0, int_result = 0;
    float float1 = 0, float2 = 0, float_result = 0;
    char input_buffer[100];
    char result_buffer[32];
    char operation[5];

    PRINT("Iniciando programa...\n");
    timer_setup(2000);     //Configurar timer
    PRINT("Habilitando IRQs...\n");
    enable_timer_irq();    //Habilitar interrupciones de timer
    PRINT("Se han habilitado las IRQs\n");

    PRINT("Status de banderas: %d\n", VIC_IRQ_STATUS);

    PRINT("Tiempo restante: %d\n", TIMER_VALUE);
    timer_isr();

    
    PRINT("Status de banderas: %d\n", VIC_IRQ_STATUS);

    while (1) {
        if(VIC_IRQ_STATUS == 16){
            timer_isr();
        }
        PRINT("Tiempo restante: %d\n", get_timer_value());
        for (volatile int i = 0; i < 99999999; i++);  // Pequeña pausa
        //PRINT("Status de banderas: %d\n", VIC_IRQ_STATUS);
        //PRINT("Estado de PIC_VECT_ADDR: %d\n", PIC_VECT_ADDR);
        
        //for (volatile int i = 0; i < 500000; i++);  // Pequeña pausa
        /*
        PRINT("Tiempo restante: %d\n", TIMER_VALUE);
        PRINT("Ingrese que tipo de datos va a ingresar: \n");
        PRINT("1. Enteros\n");
        PRINT("2. Flotantes\n");
        PRINT("3. String\n");
        READ("%d", input_buffer);

        switch (*input_buffer)
        {
            //Operaciones con enteros
        case 1:
            PRINT("Ingrese primer numero (entero): \n");
            READ("%d", &int1);
            PRINT("Seleccione operacion (+, -, *, /): \n");
            READ("%s", &operation);
            PRINT("Ingrese segundo numero (entero): \n");
            READ("%d", &int2);
            switch (*operation) {
                    case '+': int_result = int1 + int2; break;
                    case '-': int_result = int1 - int2; break;
                    case '*': int_result = int1 * int2; break;
                    case '/': int_result = (int2 != 0) ? int1 / int2 : 0; break;
                    default: PRINT("Operacion invalida.\n"); continue;
                }
            PRINT("Resultado: %d\n", int_result);
            break;
            //Operaciones con flotantes
        case 2:
            PRINT("Ingrese primer numero (flotante): \n");
            READ("%f", &float1);
            PRINT("Seleccione operacion (+, -, *, /): \n");
            READ("%s", &operation);
            PRINT("Ingrese segundo numero (flotante): \n");
            READ("%f", &float2);
            switch (*operation) {
                    case '+': float_result = float1 + float2; break;
                    case '-': float_result = float1 - float2; break;
                    case '*': float_result = float1 * float2; break;
                    case '/': float_result = (float2 != 0) ? float1 / float2 : 0; break;
                    default: PRINT("Operacion invalida.\n"); continue;
                }
            PRINT("Resultado: %f\n", float_result);
            break;
            //Strings
        case 3:
            PRINT("Ingrese un string: ");
            READ("%s", input_buffer);
            PRINT("\nCadena ingresada: %s\n", input_buffer);
            break;
        default:
            PRINT("Tipo de dato invalido\n");
            break;
        }

        PRINT("------------------------------------------------\n");*/
    }
}
