# Sistema Bancario Multihilo

## Video de YouTube
[Ver video](|https://youtu.be/THtn_GYYbR0|)
---

---
## **Colaboradores**
- [Poncho Ajmv](https://github.com/poncho-ajmv)
---

## Resumen
Este proyecto simula un sistema bancario multihilo donde múltiples cajeros automáticos (ATMs) realizan transacciones simultáneamente. Se implementa sincronización de hilos mediante mutexes y joins para garantizar la consistencia de los datos.

## Descripción
Se desarrolla un sistema en C que maneja varias cuentas bancarias, permitiendo depósitos, retiros y transferencias entre ellas. Se usa la biblioteca `pthread` para manejar los hilos de los ATMs y mutexes para evitar condiciones de carrera.

## Objetivos
- Implementar sincronización de hilos con mutexes y joins.
- Prevenir condiciones de carrera en un entorno bancario multihilo.
- Registrar de forma segura las transacciones en un archivo de log.
- Simular operaciones bancarias concurrentes de manera eficiente.

## Estructura del Proyecto

Lab_08
│
├─ Linux/
│   ├─ main.c              # Procesador de el artchivo transactions.log
│   ├─ Makefile            # Script de compilación
│   └─ run.sh              # Script para compilar y ejecutar el programa
│   └─ transactions.log    # Se genera toda la informacion necesaria
│
└─ Instructions/
    └─ Lab_Instructions.pdf # Documento con instrucciones del laboratorio

   - Inicializa cuentas, crea hilos y espera su finalización.

## Funcionamiento
1. Se inicializan las cuentas bancarias con un saldo inicial de $1000.
2. Se crean múltiples hilos representando ATMs.
3. Cada ATM ejecuta transacciones aleatorias (retiros, depósitos y transferencias).
4. Se utilizan mutexes para evitar modificaciones inconsistentes en los saldos.
5. Se registra cada transacción en un archivo `transactions.log`.
6. Una vez finalizadas todas las transacciones, se imprime el saldo final de cada cuenta.

### Impresión de Resultados
Ejemplo de salida en consola:
```
Saldo Inicial:
Cuenta 0: $1000
Cuenta 1: $1000
Cuenta 2: $1000

Saldo Final:
Cuenta 0: $950
Cuenta 1: $1050
Cuenta 2: $1100
```

## Ejemplo de Salida
Ejemplo de archivo `transactions.log`:
```
[Vie Mar 14 10:40:19 2025] Retiró $47.00 de la Cuenta 0 | Nuevo Saldo: $1051.00  
[Vie Mar 14 10:40:19 2025] Retiró $51.00 de la Cuenta 2 | Nuevo Saldo: $1047.00  
[Vie Mar 14 10:40:19 2025] Depositó $21.00 en la Cuenta 1 | Nuevo Saldo: $1051.00  
[Vie Mar 14 10:40:19 2025] Transferencia de $14.00 de la Cuenta 1 a la Cuenta 0  
[Vie Mar 14 10:40:19 2025] Depositó $72.00 en la Cuenta 2 | Nuevo Saldo: $1119.00  
[Vie Mar 14 10:40:20 2025] Transacción Fallida: Fondos Insuficientes | Cuenta 2  
```

