#include "stdio.h"
#include "stdio.c"

int contains_decimal_point(const char *s) {
    while (*s) {
        if (*s == '.') {
            return 1;
        }
        s++;
    }
    return 0;
}

void main() {
    int int1 = 0, int2 = 0, int_result = 0;
    float float1 = 0, float2 = 0, float_result = 0;
    char input_buffer[100];
    char result_buffer[32];
    char operation;

    PRINT("Inicia programa...\n");

    while (1) {
        PRINT("Ingrese un dato (puede ser entero, flotante o texto): ");
        uart_gets_input(input_buffer, sizeof(input_buffer));

        if ((input_buffer[0] >= '0' && input_buffer[0] <= '9') || input_buffer[0] == '-') {
            if (contains_decimal_point(input_buffer)) {
                float1 = uart_atof(input_buffer);
            } else {
                int1 = uart_atoi(input_buffer);
            }

            PRINT("Seleccione operacion (+, -, *, /): ");
            uart_gets_input(&operation, 2);

            if (contains_decimal_point(input_buffer)) {
                PRINT("Primer numero flotante detectado: ");
                uart_ftoa(float1, result_buffer);
                PRINT("%s\n", result_buffer);

                PRINT("Ingrese el segundo numero flotante: ");
                uart_gets_input(input_buffer, sizeof(input_buffer));
                float2 = uart_atof(input_buffer);
                uart_ftoa(float2, result_buffer);
                PRINT("Segundo numero flotante detectado: %s\n", result_buffer);

                switch (operation) {
                    case '+': float_result = float1 + float2; break;
                    case '-': float_result = float1 - float2; break;
                    case '*': float_result = float1 * float2; break;
                    case '/': float_result = (float2 != 0) ? float1 / float2 : 0; break;
                    default: PRINT("Operacion no valida.\n"); continue;
                }
                
                PRINT("Resultado: "); uart_ftoa(float_result, result_buffer); PRINT("%s\n", result_buffer);
            } else {
               

                PRINT("\nIngrese el segundo numero entero: ");
                uart_gets_input(input_buffer, sizeof(input_buffer));
                int2 = uart_atoi(input_buffer);
                

                switch (operation) {
                    case '+': int_result = int1 + int2; break;
                    case '-': int_result = int1 - int2; break;
                    case '*': int_result = int1 * int2; break;
                    case '/': int_result = (int2 != 0) ? int1 / int2 : 0; break;
                    default: PRINT("Operacionn no valida.\n"); continue;
                }
                
                PRINT("Resultado: %d\n", int_result);
            }
        } else {
            PRINT("Cadena ingresada: %s\n", input_buffer);
        }

        PRINT("------------------------------------------------\n");
    }
}
