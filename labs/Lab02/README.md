# **Laboratorio 003: Interrupciones y Manejo de Excepciones**

## **Video de YouTube**

[Ver video](|https://www.youtube.com/watch?v=0z6VicMW958|)
## **Colaboradores**
- [Ernesto Aragon](https://github.com/ErnestoAragon03)
- [Christian Sosa](https://github.com/ChristianSosa22)
- [Poncho Ajmv](https://github.com/poncho-ajmv)

---

## **Objetivo**

Este laboratorio introduce la programación basada en interrupciones en arquitecturas ARM. Se implementa una interrupción de temporizador para sustituir retardos manuales con bucles activos, mejorando la eficiencia del sistema. Los objetivos principales incluyen:

- Configuración del temporizador para ejecución periódica.
- Manejo de la Tabla de Vectores de Interrupción (IVT).
- Implementación de una Rutina de Servicio de Interrupción (ISR).
- Validación del funcionamiento en QEMU.

## **Resumen**

Se amplió el framework bare-metal existente para manejar interrupciones de temporizador cada 2 segundos. Se realizaron las siguientes modificaciones:

1. **os.c**: Implementación del manejo de interrupciones del temporizador y configuración del Vector Interrupt Controller (VIC).
2. **main.c**: Inicialización del temporizador y habilitación de interrupciones.
3. **root.s**: Ajuste de la Tabla de Vectores de Excepción para redirigir IRQs correctamente.


## **Estructura del Proyecto**

### **1. Nivel de Usuario (main.c)**
- Configura y habilita la interrupción del temporizador.
- Imprime mensajes de depuración.
- Monitorea el valor del temporizador para verificar su funcionamiento.

### **2. Nivel de Sistema Operativo (os.c)**
- Configura el periférico del temporizador.
- Habilita la IRQ del temporizador en el VIC.
- Implementa el manejador de interrupciones asegurando la limpieza de banderas.

### **3. Interfaz de Hardware (root.s)**
- Define la Tabla de Vectores de Excepción.
- Redirige las interrupciones al manejador correspondiente.
- Preserva registros de CPU dentro de la ISR.

## **Implementación**

### **1. Inicialización del Temporizador (os.c)**
- Se carga un valor de cuenta regresiva (2s).
- Se configura el temporizador en modo periódico.
- Se habilita la generación de interrupciones.

### **2. Configuración del VIC (os.c)**
- Habilita la IRQ #4 (Timer IRQ).
- Configura la redirección de interrupciones a la CPU.

### **3. Manejador de IRQ (root.s)**
- Modifica la tabla de vectores.
- Implementa una rutina de servicio que detecta la fuente de la interrupción, ejecuta el manejador y limpia las banderas.

### **4. Pruebas y Validación (main.c)**
- Se ejecuta el sistema y se monitorean mensajes de depuración.
- Se verifica que el temporizador dispare la interrupción cada 2 segundos.


### Descripción de funciones modificadas
- `enable_timer_irq()`: Habilita las interrupciones del temporizador en el VIC.
- `timer_isr()`: Atiende la interrupción del temporizador y limpia la bandera de interrupción.
- `get_timer_value()`: Devuelve el valor restante del temporizador.

### Registro de ejecución y validación de interrupciones
- Se imprime el estado de las banderas IRQ (`VIC_IRQ_STATUS`).
- Se valida la ejecución de `timer_isr()` al detectar una interrupción activa.
- `uart_puts()` permite visualizar los mensajes de ejecución en la UART.

### Definiciones de registros:
- Se definen direcciones base para el temporizador (TIMER_BASE) y el controlador de interrupciones (VIC_BASE).

Se mapean registros específicos del temporizador, como:
- TIMER_LOAD: Para cargar un valor inicial en el temporizador.
- TIMER_VALUE: Para leer el valor actual del temporizador.
- TIMER_CONTROL: Para controlar el comportamiento del temporizador.
- TIMER_INTCLR: Para limpiar (desactivar) interrupciones del temporizador.
- TIMER_RIS: Para leer el estado de la interrupción del temporizador.

También se define un registro del controlador de interrupciones (VIC_IRQ_STATUS) para verificar el estado de las interrupciones.

## **Problemas y Soluciones**

| Problema | Causa Probable | Solución |
|----------|---------------|-----------|
| La interrupción no se activa | VIC mal configurado | Verificar registro VIC_INTENABLE |
| El sistema se bloquea | CPU no reconoce la interrupción | Escribir en VIC_VADDR en ISR |
| El temporizador no se reinicia | Modo periódico deshabilitado | Habilitar en TIMER_CONTROL |
| Múltiples activaciones inesperadas | Bandera de interrupción no limpiada | Escribir TIMER_INTCLR = 1 tras atender IRQ |


## **Conclusiones**

- Implementar temporizadores en ARM de manera eficiente.
- Manejar interrupciones de hardware en sistemas embebidos.
- Profundizar en el manejo de excepciones en bajo nivel.



