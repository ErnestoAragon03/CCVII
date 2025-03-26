# Lab06 — Threads (Windows)

## Objetivo
Crear un programa que simule el funcionamiento de un sistema bancario, haciendo el uso de múltiples cuentas, cada una con su respectivo balance; y de multiples ATMs representados por Threads, cada ATM debe de ser capaz de procesar diferentes transacciones (depósitos, retiros y transferencias) de manera aleatoria y hacia cualquier cuenta

# Tareas 
## Tarea 1 — Implementar una Cuenta Bancaria Compartida
Para completar esta tarea se implemento una struct de c llamada BankAccount que ocntaba con 3 elementos:
- int balance: Es el dinero que posee la cuenta
- CRITICAL_SECTION lock: El lock para proteger el recurso compartido balance
- int accountID: ID que posee la cuenta para poder identificarla

## Tarea 2 — Simular los ATMs como Threads
Se creó una función que simulaba el comportamiento de los ATMs, donde durante un determinado número de transacciones (se puede editar) seleccionará un ID al azar, una cantidad de dinero aleatoria y finalmente una de las 3 posibles transacciones disponibles. Una vez ya se ha computado estos 3 elementos simplemente mandamos a llamar a la función que creamos para cada tipo de transacciones.  El único caso especial es el de transferencia, donde hay que seleccionar un ID aleatorio más, el de la cuenta destino.

## Tarea 3 — Implementar Sincronización de Threads
Cada función de transacción tenía su respectivo lock para evitar los race conditions, y para la sincronización de threads por medio de joins se usó un WaitForMultipleObjects al igual que ya se ha hecho en otros labs.

## Tarea 4 — Sistema de Logs y manejo de errores
Dentro de la función de ATMs se crea un mensaje en el formato correcto para crear logs en el archivo .log (que también es creado durante la ejecución del programa).  En caso de que una transacción resulte fallida (normalmente por insuficiencia de fondos) las funciones para editar el balance se encargarán de informar que la transacción no se completó y que se debe de crear un log de error en lugar de uno normal.

## Tarea 5 — Reporte Final
Finalmente, despúes de que todos los threads han terminado y se juntaron con join, se inserta el balance final de cada una de las cuentas en el archivo .log.

### Link al video del lab:
[Ver video](https://youtu.be/qLxQWhyXgxc)