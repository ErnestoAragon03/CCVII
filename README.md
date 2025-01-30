# Laboratorio 001: Calculadora en ARM Bare-Metal

## Colaboradores
- [Poncho Ajmv](https://github.com/poncho-ajmv)
- [Ernesto Aragon](https://github.com/ErnestoAragon03)
- [Christian Sosa](https://github.com/ChristianSosa22)



## Estructura del Proyecto
El proyecto está organizado en tres capas principales:

1. **Capa de Usuario (User Level):**
   - Contiene la lógica principal de la aplicación en `main.c`.
   - Se encarga de la interacción con el usuario, procesamiento de entrada y salida de datos.

2. **Capa de Biblioteca de Lenguaje (Language Library Level):**
   - Implementa funciones de utilidad como manipulación de cadenas y conversión de datos.
   - Incluye `stdio.c` y `stdio.h` para funciones similares a `printf` y `scanf`.

3. **Capa de Sistema Operativo (OS Level):**
   - Interfaz directa con el hardware ARM.
   - Gestiona la comunicación UART y la inicialización del sistema.

## Archivos Principales
- **main.c**: Lógica principal de la calculadora.
- **root.s**: Inicialización del stack y llamada a la función `main`.
- **string.c & string.h**: Funciones básicas de manipulación de cadenas.
- **stdio.c & stdio.h**: Implementación de funciones de entrada/salida.
- **build_and_run.sh**: Script para compilar y ejecutar la aplicación en QEMU.

## Tareas Principales
1. Analizar y entender el código proporcionado.
2. Implementar las funciones `PRINT` y `READ` en `stdio.c`.
3. Modificar `main.c` para utilizar las nuevas funciones de entrada/salida.
4. Validar la comunicación UART y asegurar el correcto funcionamiento de la calculadora.
