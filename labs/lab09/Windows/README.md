# Lab06 — Threads (Windows)

## Objetivo
Crear un programa que simule el funcionamiento de un sistema bancario, haciendo el uso de múltiples cuentas, cada una con su respectivo balance; y de multiples ATMs representados por Threads, cada ATM debe de ser capaz de procesar diferentes transacciones (depósitos, retiros y transferencias) de manera aleatoria y hacia cualquier cuenta

# Tareas 
## Tarea 1 — Simulate Car Threads: 
Para completar esta tarea se implemento una struct de c llamada CarData que contaba con 3 elementos:
- int carId: Es el ID del vehiculo
- int priority: Es el nivel de prioridad que tiene (es para la sección extra de autos prioritarios), debe de estar entre 0 y 3, donde 0 es el nivel de prioridad más alta y 3 el de más baja
- int parked: 1 si está parqueado, 0 si no.  (es para la sección extra de la Interfaz Gráfica)

## Tarea 2 — Semaphore for Parking Spots:
Se creó un semaforo que tomaba el valor de la cantidad de parqueos disponibles (MAX_PARKING_SPOTS) para indicar si un auto se puede parquear o no.  En la práctica se complico esta sección debido a los extras, ya que los semaforos no están pensados para manejar prioridades, así que decidí añadir un sistema de queues de espera (una por cada nivel de prioridad).  Por lo que para que un Thread llegara al Semaforo primero debía de checar si no había un vehículo prioritario en la cola.  Además, en caso de llegar al semaforo tiene tiempo limitado (yo seleccione 2 segundos como este tiempo) para que el semaforo lo deje pasar, de caso contrario tendrá que regresar a la cola una vez más.

## Tarea 3 — Thread-Safe Logging:
Se creó un sistema de logs compartido (usando una sección de crítica) donde se mandaban a un archivo .log los logs en el formato indicado cada que un auto realizaba una acción.  Se usó un archivo auxiliar ya que se añadió la sección extra de una GUI o Dashboard en tiempo real que usaba la consola, por lo que la mejor forma de presentar los logs de manera ordenada era con un archivo .log como ya se ha hecho en otros laboratorios.

## Tarea 4 — Car Behavior
- Arrival: Se asigna un tiempo aleatorio para que el vehículo llegue al parqueo (esto ya con el thread creado, solo se está fingiendo cuanto tiempo le lleva para llegar a la cola).  Inmediatamente se manda un log indicando que se arrivó junto al Timestamp
- Wait: Primero era necesario checar que el Thread fuese el primero en su propia cola, de serlo entonces debía de checar que todas las colas de mayor prioridad estuviesen vacías (esto simplemente revisando si el primer elemento de la queue es NULL).  Si esto se cumple entonces por fin se llega al semaforo, en el que permanece durante un tiempo limitado hasta que sea expulsado y retorne a su fila (esto para evitar problemas con vehículos prioritarios).

## Tarea 5 — Statistics Tracking
Cada que se calculaba el tiempo de espera para cada vehículo se sumaba en una variable global "averageWaitingTime", luego, cuando se terminaban todos los Threads, simplemente se dividia dentro del número total de autos.  En ese momento ya se puede mandar ambas estadisticas al log compartido.

### Link al video del lab:
[Ver video](https://youtu.be/clZnqxERbAk)