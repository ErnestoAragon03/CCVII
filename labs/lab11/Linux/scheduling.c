/****************************************************************************
 * scheduling_mejorado.c
 * 
 * Implementación mejorada de algoritmos de planificación de CPU en C (Linux)
 * Incluye:
 *   - FIFO  (First-In, First-Out / FCFS)
 *   - RR    (Round Robin) con cuanto fijo = 2
 *   - SJF   (Shortest Job First) no expropiativo
 *   - SRTF  (Shortest Remaining Time First) expropiativo
 *
 * Mejora: Se agregan logs de eventos paso a paso y un diagrama de Gantt
 * más claro en ASCII. Salida traducida al español en su mayoría.
 *
 ****************************************************************************/

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <stdbool.h>
 #include <string.h>
 
 #define QUANTUM 2  // cuanto de tiempo para Round Robin
 
 // Estructura para representar un proceso/hilo
 typedef struct {
     int pid;         // identificador
     int arrival;     // tiempo de llegada
     int burst;       // ráfaga total
     int remaining;   // ráfaga restante (para RR y SRTF)
     int start;       // primer tiempo en que comienza a ejecutarse
     int completion;  // tiempo en que finaliza
     int waiting;     // tiempo total de espera
     int turnaround;  // tiempo total de retorno (completion - arrival)
     bool started;    // indica si ya inició en algún momento
     bool finished;   // indica si terminó
 } Process;
 
 // ----------------------------------------------------------------------
 // Función genérica para imprimir un mensaje de evento (arribo, inicio...)
 // en el formato: [tiempo_simulado] Mensaje
 // ----------------------------------------------------------------------
 void log_event(int current_time, const char *msg) {
     printf("[t=%2d] %s\n", current_time, msg);
 }
 
 // ----------------------------------------------------------------------
 // Copia los procesos de un arreglo original a otro, reiniciando campos
 // usados durante la simulación.
 // ----------------------------------------------------------------------
 void copy_processes(Process *dest, const Process *src, int n) {
     for(int i = 0; i < n; i++){
         dest[i] = src[i];
         dest[i].remaining  = src[i].burst;
         dest[i].start      = -1;
         dest[i].completion = 0;
         dest[i].waiting    = 0;
         dest[i].turnaround = 0;
         dest[i].started    = false;
         dest[i].finished   = false;
     }
 }
 
 // ----------------------------------------------------------------------
 // Imprime una tabla de resultados para un algoritmo dado, calculando
 // tiempo de espera y de retorno. Muestra promedios.
 // ----------------------------------------------------------------------
 void print_results(Process *p, int n, const char *titulo) {
     double total_waiting = 0.0;
     double total_turnaround = 0.0;
 
     printf("\n=== %s ===\n", titulo);
     printf("PID | Llegada | Rafaga | Comienzo |   Fin | Espera | Retorno\n");
     printf("------------------------------------------------------------\n");
     for(int i = 0; i < n; i++){
         p[i].waiting    = p[i].start - p[i].arrival;
         p[i].turnaround = p[i].completion - p[i].arrival;
         total_waiting      += p[i].waiting;
         total_turnaround   += p[i].turnaround;
 
         printf("%3d | %7d | %6d | %8d | %5d | %6d | %7d\n",
                p[i].pid,
                p[i].arrival,
                p[i].burst,
                p[i].start,
                p[i].completion,
                p[i].waiting,
                p[i].turnaround);
     }
 
     printf("------------------------------------------------------------\n");
     printf("Promedio Espera   = %.2f\n", total_waiting / n);
     printf("Promedio Retorno  = %.2f\n", total_turnaround / n);
     printf("============================================================\n\n");
 }
 
 // ----------------------------------------------------------------------
 // Diagrama de Gantt en ASCII. Marca con █ la ejecución de un proceso.
 // ----------------------------------------------------------------------
 void print_gantt_chart(Process *p, int n, const char *titulo) {
     // Hallar el tiempo máximo de finalización
     int max_completion = 0;
     for(int i = 0; i < n; i++) {
         if(p[i].completion > max_completion) {
             max_completion = p[i].completion;
         }
     }
 
     printf("=== Diagrama de Gantt: %s ===\n", titulo);
 
     // Para cada proceso, línea con # (o █) donde se está ejecutando
     for(int i = 0; i < n; i++) {
         printf("P%2d |", p[i].pid);
         for(int t = 0; t < max_completion; t++) {
             if(t >= p[i].start && t < p[i].completion) {
                 printf("█");
             } else {
                 printf("_");
             }
         }
         // Información al final
         printf("| start=%2d, end=%2d\n", p[i].start, p[i].completion);
     }
 
     // Escala de tiempos debajo
     printf("    ");
     for(int t = 0; t < max_completion; t++){
         if(t < 10) {
             printf("%d", t);
         } else if(t < 100) {
             printf("^"); // Para no desalinear
         } else {
             printf("?"); // A partir de 99+
         }
     }
     printf("\n\n");
 }
 
 // ----------------------------------------------------------------------
 // Comparador para ordenar por tiempo de llegada (y luego por PID)
 // ----------------------------------------------------------------------
 int compare_arrival(const void *a, const void *b) {
     Process *p1 = (Process*)a;
     Process *p2 = (Process*)b;
     if (p1->arrival == p2->arrival)
         return p1->pid - p2->pid;
     return p1->arrival - p2->arrival;
 }
 
 // ======================================================================
 // 1) FIFO (FCFS)
 //    - Ordenar por llegada.
 //    - Ejecutar cada proceso cuando la CPU quede libre.
 // ======================================================================
 void fifo_scheduling(Process *p, int n) {
     qsort(p, n, sizeof(Process), compare_arrival);
 
     int current_time = 0;
     printf("***** Eventos FIFO *****\n");
     for(int i=0; i<n; i++){
         if(current_time < p[i].arrival) {
             // CPU está inactiva hasta que llegue este proceso
             current_time = p[i].arrival;
         }
 
         // Log: proceso llega (realmente llega al tiempo p[i].arrival, 
         // pero estamos en current_time ya)
         char msg[100];
         snprintf(msg, sizeof(msg), "P%d ARRIBO (llegada=%d)", p[i].pid, p[i].arrival);
         log_event(current_time, msg);
 
         p[i].start      = current_time;
         snprintf(msg, sizeof(msg), "P%d INICIA su ejecucion (burst=%d)", p[i].pid, p[i].burst);
         log_event(current_time, msg);
 
         p[i].completion = current_time + p[i].burst;
         current_time    = p[i].completion;
 
         snprintf(msg, sizeof(msg), "P%d TERMINA ejecucion", p[i].pid);
         log_event(current_time, msg);
     }
     printf("***** Fin Eventos FIFO *****\n\n");
 }
 
 // ======================================================================
 // 2) Round Robin (quantum = 2)
 //    - Ordenar por llegada, usar cola para procesos listos.
 //    - Ejecutar cada proceso por min(quantum, remaining).
 // ======================================================================
 
 // Estructura de cola sencilla
 typedef struct {
     int *data;
     int front;
     int rear;
     int size;
 } Queue;
 
 void init_queue(Queue *q, int capacity){
     q->data = (int*) malloc(sizeof(int)*capacity);
     q->front = 0;
     q->rear  = 0;
     q->size  = capacity;
 }
 
 bool queue_empty(Queue *q){
     return (q->front == q->rear);
 }
 
 void enqueue(Queue *q, int pid){
     q->data[q->rear++] = pid;
 }
 
 int dequeue(Queue *q){
     return q->data[q->front++];
 }
 
 void round_robin_scheduling(Process *p, int n) {
     qsort(p, n, sizeof(Process), compare_arrival);
 
     Queue q;
     init_queue(&q, n*10); // tamaño un poco mayor que n
     int completed = 0;
     int current_time = 0;
     int index = 0;
 
     printf("***** Eventos Round Robin (q=%d) *****\n", QUANTUM);
 
     // Mientras no se completen todos
     while(completed < n) {
         // Agregamos a la cola todos los que llegan en current_time
         while(index < n && p[index].arrival <= current_time) {
             // Log de llegada
             char msg[100];
             snprintf(msg, sizeof(msg), "P%d ARRIBO (llegada=%d)", p[index].pid, p[index].arrival);
             log_event(current_time, msg);
 
             enqueue(&q, index);
             index++;
         }
 
         if(queue_empty(&q)) {
             // No hay procesos listos, adelantamos al siguiente arrival
             if(index < n) {
                 current_time = p[index].arrival;
                 // Log de llegada en este tiempo
                 char msg[100];
                 snprintf(msg, sizeof(msg), "P%d ARRIBO (llegada=%d)", p[index].pid, p[index].arrival);
                 log_event(current_time, msg);
 
                 enqueue(&q, index);
                 index++;
             }
         } else {
             int i = dequeue(&q);
 
             // Si el proceso nunca había empezado, registramos su start
             if(!p[i].started){
                 p[i].start = current_time;
                 p[i].started = true;
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d INICIA su ejecucion (burst=%d)", p[i].pid, p[i].burst);
                 log_event(current_time, msg);
             } else {
                 // Vuelve a la CPU tras esperar
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d REANUDA ejecucion (restante=%d)", p[i].pid, p[i].remaining);
                 log_event(current_time, msg);
             }
 
             // Ejecutamos min(quantum, remaining)
             int run_time = (p[i].remaining < QUANTUM) ? p[i].remaining : QUANTUM;
             p[i].remaining -= run_time;
             int start_slice = current_time;
             current_time   += run_time;
 
             // Revisa si alguien llegó mientras P[i] corre
             while(index < n && p[index].arrival <= current_time) {
                 // Llega un nuevo proceso
                 char msg[100];
                 snprintf(msg, sizeof(msg), "P%d ARRIBO (llegada=%d)", p[index].pid, p[index].arrival);
                 log_event(current_time, msg);
 
                 enqueue(&q, index);
                 index++;
             }
 
             // Si termina
             if(p[i].remaining == 0) {
                 p[i].completion = current_time;
                 p[i].finished   = true;
                 completed++;
 
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d TERMINA (ejec. desde t=%d hasta t=%d)",
                          p[i].pid, start_slice, current_time);
                 log_event(current_time, msg);
             } else {
                 // Aún falta ráfaga, se mete de nuevo a la cola
                 enqueue(&q, i);
 
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d SE PRE-EMPAQUETA (le quedan %d)", p[i].pid, p[i].remaining);
                 log_event(current_time, msg);
             }
         }
     }
 
     free(q.data);
     printf("***** Fin Eventos Round Robin *****\n\n");
 }
 
 // ======================================================================
 // 3) SJF (no expropiativo)
 //    - Ordenar por llegada.
 //    - En cada iteración, escoger el proceso con menor burst pendiente
 //      entre los que ya hayan llegado y no estén finalizados.
 // ======================================================================
 int find_shortest_job(Process *p, int n, int current_time){
     int idx = -1;
     int min_burst = 999999;
     for(int i=0; i<n; i++){
         if(!p[i].finished && p[i].arrival <= current_time){
             if(p[i].burst < min_burst){
                 min_burst = p[i].burst;
                 idx = i;
             }
         }
     }
     return idx;
 }
 
 void sjf_scheduling(Process *p, int n) {
     qsort(p, n, sizeof(Process), compare_arrival);
 
     int completed = 0;
     int current_time = 0;
 
     printf("***** Eventos SJF (no expropiativo) *****\n");
 
     while(completed < n){
         int idx = find_shortest_job(p, n, current_time);
 
         if(idx == -1) {
             // No hay proceso disponible, avanzamos al arrival del siguiente
             current_time = p[completed].arrival;
             continue;
         }
 
         // Log llegada de todos los que aparecen en este momento
         for(int i = 0; i < n; i++){
             if(!p[i].finished && p[i].arrival == current_time && i != idx){
                 char msg[100];
                 snprintf(msg, sizeof(msg), "P%d ARRIBO (llegada=%d)",
                          p[i].pid, p[i].arrival);
                 log_event(current_time, msg);
             }
         }
 
         if(!p[idx].started) {
             p[idx].start = (current_time < p[idx].arrival) ? p[idx].arrival : current_time;
             p[idx].started = true;
             char msg[100];
             snprintf(msg, sizeof(msg),
                      "P%d INICIA ejecucion (burst=%d)", p[idx].pid, p[idx].burst);
             log_event(p[idx].start, msg);
         }
 
         // Avanzamos tiempo
         current_time       = (current_time < p[idx].arrival) ? p[idx].arrival : current_time;
         p[idx].completion  = current_time + p[idx].burst;
         p[idx].finished    = true;
 
         char msg[100];
         snprintf(msg, sizeof(msg),
                  "P%d TERMINA (ejec. desde t=%d hasta t=%d)",
                  p[idx].pid, current_time, p[idx].completion);
         log_event(p[idx].completion, msg);
 
         current_time       = p[idx].completion;
         completed++;
     }
 
     printf("***** Fin Eventos SJF *****\n\n");
 }
 
 // ======================================================================
 // 4) SRTF (expropiativo)
 //    - En cualquier unidad de tiempo, ejecutar el proceso con menor
 //      tiempo restante entre los que han llegado.
 //    - Avanzamos de 1 en 1 unidad de tiempo en la simulación.
 // ======================================================================
 int find_srtf(Process *p, int n, int current_time){
     int idx = -1;
     int min_remaining = 999999;
     for(int i=0; i<n; i++){
         if(!p[i].finished && p[i].arrival <= current_time){
             if(p[i].remaining < min_remaining){
                 min_remaining = p[i].remaining;
                 idx = i;
             }
         }
     }
     return idx;
 }
 
 void srtf_scheduling(Process *p, int n){
     qsort(p, n, sizeof(Process), compare_arrival);
 
     int completed = 0;
     int current_time = 0;
     int last_pid = -1;  // para detectar cambio de proceso
 
     printf("***** Eventos SRTF (expropiativo) *****\n");
 
     // Avanzamos hasta que todos terminen
     while(completed < n){
         // Log de llegada
         for(int i = 0; i < n; i++){
             if(!p[i].finished && p[i].arrival == current_time){
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d ARRIBO (llegada=%d)", p[i].pid, p[i].arrival);
                 log_event(current_time, msg);
             }
         }
 
         int idx = find_srtf(p, n, current_time);
 
         if(idx == -1){
             // Nadie ha llegado todavía; avanzamos
             current_time++;
             continue;
         }
 
         // Si el proceso aún no ha comenzado
         if(!p[idx].started){
             p[idx].start = current_time;
             p[idx].started = true;
             char msg[100];
             snprintf(msg, sizeof(msg),
                      "P%d INICIA ejecucion (burst=%d)", p[idx].pid, p[idx].burst);
             log_event(current_time, msg);
         } else {
             // Si cambió de proceso con respecto a la última unidad
             if(last_pid != idx && last_pid != -1) {
                 char msg[100];
                 snprintf(msg, sizeof(msg),
                          "P%d ES PREEMPTADO, P%d TOMA CPU (remaining=%d)",
                          last_pid, p[idx].pid, p[idx].remaining);
                 log_event(current_time, msg);
             }
         }
 
         // Ejecutamos 1 unidad de tiempo
         p[idx].remaining--;
         last_pid = idx;
 
         current_time++;
 
         // Si termina
         if(p[idx].remaining == 0){
             p[idx].finished   = true;
             p[idx].completion = current_time;
             completed++;
 
             char msg[100];
             snprintf(msg, sizeof(msg), "P%d TERMINA en t=%d", p[idx].pid, current_time);
             log_event(current_time, msg);
         }
     }
 
     printf("***** Fin Eventos SRTF *****\n\n");
 }
 
 // ======================================================================
 //                              MAIN
 // ======================================================================
 int main(){
     srand(time(NULL));
 
     // 1) Generar número aleatorio de procesos (5..15)
     int n = rand() % 11 + 5;
 
     // 2) Reservar memoria para ellos
     Process *original = (Process*) malloc(sizeof(Process)*n);
 
     // 3) Asignar arrival y burst aleatorios
     printf("\nGenerando %d procesos:\n", n);
     for(int i = 0; i < n; i++){
         original[i].pid       = i;
         original[i].arrival   = rand() % 101;     // [0..100]
         original[i].burst     = (rand() % 10) + 1; // [1..10]
         original[i].remaining = original[i].burst;
         original[i].start     = -1;
         original[i].completion= 0;
         original[i].waiting   = 0;
         original[i].turnaround= 0;
         original[i].started   = false;
         original[i].finished  = false;
 
         printf("Proceso %2d -> Llegada: %3d  |  Ráfaga: %2d\n",
                 original[i].pid, original[i].arrival, original[i].burst);
     }
     printf("-------------------------------------------------\n\n");
 
     // Crear copias para cada algoritmo
     Process *for_fifo = (Process*) malloc(sizeof(Process)*n);
     Process *for_rr   = (Process*) malloc(sizeof(Process)*n);
     Process *for_sjf  = (Process*) malloc(sizeof(Process)*n);
     Process *for_srtf = (Process*) malloc(sizeof(Process)*n);
 
     // ===============================
     // FIFO
     // ===============================
     copy_processes(for_fifo, original, n);
     fifo_scheduling(for_fifo, n);
     print_results(for_fifo, n, "Planificación FIFO");
     print_gantt_chart(for_fifo, n, "FIFO");
 
     // ===============================
     // Round Robin
     // ===============================
     copy_processes(for_rr, original, n);
     round_robin_scheduling(for_rr, n);
     print_results(for_rr, n, "Planificación Round Robin (q=2)");
     print_gantt_chart(for_rr, n, "Round Robin (q=2)");
 
     // ===============================
     // SJF
     // ===============================
     copy_processes(for_sjf, original, n);
     sjf_scheduling(for_sjf, n);
     print_results(for_sjf, n, "Planificación SJF (no expropiativa)");
     print_gantt_chart(for_sjf, n, "SJF");
 
     // ===============================
     // SRTF
     // ===============================
     copy_processes(for_srtf, original, n);
     srtf_scheduling(for_srtf, n);
     print_results(for_srtf, n, "Planificación SRTF (expropiativa)");
     print_gantt_chart(for_srtf, n, "SRTF");
 
     // Liberar memoria
     free(original);
     free(for_fifo);
     free(for_rr);
     free(for_sjf);
     free(for_srtf);
 
     return 0;
 }
 