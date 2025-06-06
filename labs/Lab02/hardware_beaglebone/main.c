#include "stdio.h"
#include "os.h"

// Declaración explícita para el linker y root.s
void main(void);

void main(void) {
    int int1 = 0, int2 = 0, int_result = 0;
    float float1 = 0, float2 = 0, float_result = 0;
    char input_buffer[100];
    char result_buffer[32];
    char operation[5];

    PRINT("Iniciando programa...\n");

    while (1) {
        PRINT("Ingrese que tipo de datos va a ingresar: \n");
        PRINT("1. Enteros\n");
        PRINT("2. Flotantes\n");
        PRINT("3. String\n");
        READ("%d", input_buffer);

        switch (*input_buffer) {
            case '1':
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

            case '2':
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

            case '3':
                PRINT("Ingrese un string: ");
                READ("%s", input_buffer);
                PRINT("\nCadena ingresada: %s\n", input_buffer);
                break;

            default:
                PRINT("Tipo de dato invalido\n");
                break;
        }

        PRINT("------------------------------------------------\n");
    }
}
