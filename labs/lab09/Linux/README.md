# Lab08 - Sistema de Estacionamiento Inteligente

## Video Demostración
[Ver video explicativo](|https://youtu.be/KZO1tBYt868 |)

## Colaboradores
- [Poncho Ajmv](https://github.com/poncho-ajmv)

---

## Descripción
Este proyecto implementa un sistema de estacionamiento inteligente en Linux utilizando semáforos y mutex para gestionar el acceso concurrente de los vehículos. El sistema simula el comportamiento de múltiples autos (hilos) intentando estacionarse en un espacio con un número limitado de plazas (N). Además, cuenta con una interfaz gráfica 3D desarrollada con GTK/SDL.

---

## Objetivos
- Simular el comportamiento de autos (hilos) compitiendo por plazas de estacionamiento limitadas.
- Utilizar semáforos para gestionar los espacios disponibles.
- Implementar mecanismos de sincronización (mutex) para el registro seguro de eventos.
- Mostrar estadísticas sobre el uso del estacionamiento.
- Implementar una interfaz gráfica 3D (GTK/SDL) para visualización.

---

## Estructura del Proyecto

```
Lab_08/
│
└─ Linux/
    ├─ LabBase/
    │   ├─ main.c              # Lógica principal del sistema de estacionamiento
    │   ├─ Makefile            # Script de compilación
    │   └─ run.sh              # Script para compilar y ejecutar el programa
    │
    └─ LabExtra/ (Ambulancia y Gráfico)
        ├─ main.c              # Lógica principal del sistema de estacionamiento con gráficos
        ├─ Makefile            # Script de compilación
        └─ run.sh              # Script para compilar y ejecutar el programa
```

---

## Componentes Clave

### Semáforos (`parking_semaphore`)
- Semáforo contador inicializado con `N` (número de plazas disponibles).
- Controla el acceso concurrente a los espacios de estacionamiento.
- `sem_wait()` decrementa el contador cuando un auto ocupa una plaza.
- `sem_post()` incrementa el contador cuando un auto libera una plaza.

### Mutex
#### `log_mutex`
- Protege el registro de eventos para evitar condiciones de carrera.
- Garantiza que los mensajes de log se impriman de forma ordenada.

#### `stats_mutex`
- Protege las variables compartidas `total_cars_parked` y `total_wait_time`.
- Permite el cálculo seguro de estadísticas.

---

## Comportamiento de los Autos (Hilos)
- **Llegada:** Registra su llegada con timestamp.
- **Espera:** Espera a que haya una plaza disponible (`sem_wait`).
- **Estacionamiento:** Ocupa la plaza por un tiempo aleatorio (1-5 segundos).
- **Salida:** Libera la plaza (`sem_post`) y registra su salida.

---

## Instalación y Dependencias
Este proyecto está diseñado para ejecutarse en sistemas **Linux**. Asegúrate de tener instaladas las siguientes dependencias antes de compilar:

```
sudo apt-get install libsdl2-dev freeglut3-dev libglu1-mesa-dev
```

El programa requiere las siguientes bibliotecas para compilar correctamente:
- `-lSDL2`
- `-lGL`
- `-lGLU`
- `-lpthread`

---

## Compilación y Ejecución
Para compilar y ejecutar el programa, usa los siguientes comandos:

```
cd Linux/

cd LabBase/
./run.sh

||

cd LabVisualTerminal/
./run.sh
```

---

## Ejemplo de Salida en Consola labBase/
```
[Thu Mar 25 14:30:00 2023] Auto 1: Llegó al estacionamiento (esperó 0.00 segundos)
[Thu Mar 25 14:30:01 2023] Auto 1: Se estacionó exitosamente (esperó 1.00 segundos)
[Thu Mar 25 14:30:03 2023] Auto 1: Salió del estacionamiento (esperó 0.00 segundos)
...
Total de autos estacionados: 10
Tiempo promedio de espera: 2.34 segundos

```


---

## Notas Adicionales
- Este version solo está disponible para entornos **Linux**.
- Asegúrate de instalar todas las dependencias antes de ejecutar el programa.

---

### ¡Listo para probarlo! 🚗💨

