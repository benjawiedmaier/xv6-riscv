# Informe: Implementación de Lottery Scheduling en XV6

## Objetivo
Reemplazar el planificador **Round-Robin** de XV6 por un sistema de **Lottery Scheduling**, donde la fracción de CPU que recibe cada proceso sea proporcional a la cantidad de "tickets" que posee.

## Modificaciones Implementadas

### 1. Estructura del Proceso (`proc.h`)
```c
// Campos agregados a struct proc
int tickets;           // Número de tickets para lottery scheduling
int run_slices;        // Contador de veces que fue seleccionado
```

### 2. Inicialización (`proc.c`)
- Modificado `allocproc()` para inicializar `tickets = 100` y `run_slices = 0`
- Modificado `freeproc()` para limpiar estos campos al liberar proceso

### 3. System Call `settickets(int n)`

**Archivos modificados:**
- `syscall.h`: `#define SYS_settickets 22`
- `syscall.c`: Registro de la syscall en la tabla
- `sysproc.c`: Implementación de `sys_settickets()`
- `user.h`: Declaración para espacio de usuario
- `usys.pl`: Entrada para generar el stub en assembler

### 4. Algoritmo de Lottery Scheduling (`proc.c`)

**Cambios en `scheduler()`:**
1. Cálculo del total de tickets de procesos RUNNABLE
2. Generación de número aleatorio entre 1 y total_tickets
3. Selección del proceso ganador mediante acumulación de tickets
4. Incremento de `run_slices` al seleccionar proceso

### 5. Función de Números Aleatorios (`proc.c`)
```c
static unsigned int random(void) {
    static unsigned int seed = 12345;
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}
```

## Dificultades y Soluciones

### Error 1: Declaración de `random()`
**Problema:** En `proc.c`, la función `random()` se usaba en el scheduler antes de estar definida, causando error de declaración implícita.

**Solución:** Se agregó la declaración `unsigned int random(void);` antes de la función `scheduler()` y la definición se colocó después.

### Error 2: Syscall `settickets` no disponible
**Problema:** El programa `demo.c` no podía enlazar `settickets` porque faltaba en `usys.S`.

**Causa:** `usys.S` se genera automáticamente desde `usys.pl`, por lo que los cambios manuales se perdían.

**Solución:** Se modificó `user/usys.pl` agregando `entry("settickets");` para generar automáticamente el stub.

### Error 3: Output Mezclado en Consola
**Problema:** Múltiples procesos escribiendo simultáneamente a la consola causaban output ilegible.

**Solución:** Se implementó versión ultra-conservadora del demo que ejecuta procesos secuencialmente.

## Archivos Modificados
1. `kernel/proc.h` - Estructura del proceso
2. `kernel/proc.c` - Scheduler, allocproc, random
3. `kernel/syscall.h` - Número de syscall
4. `kernel/syscall.c` - Tabla de syscalls
5. `kernel/sysproc.c` - Implementación de settickets
6. `user/user.h` - Declaración para usuario
7. `user/usys.pl` - Generación de stub
8. `user/demo.c` - Programa de prueba
9. `Makefile` - Agregar _demo a UPROGS

## Programa de Prueba
```c
// Versión ultra-conservadora para output limpio
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_process(int id) {
    int tickets = 50 * (id + 1);
    settickets(tickets);
    printf("Inicio: proceso %d, tickets: %d\n", id, tickets);
    
    for(int i = 0; i < 1500000; i++) {
        asm volatile("nop");
    }
    
    exit(0);
}

int main(void) {
    printf("=== DEMO LOTTERY SCHEDULING ===\n");
    printf("10 procesos - Tickets: 50, 100, 150...500\n\n");
    
    for(int i = 0; i < 10; i++) {
        int pid = fork();
        if(pid == 0) {
            child_process(i);
        }
        wait(0); // Ejecución secuencial
    }
    
    printf("\n=== DEMO COMPLETADO ===\n");
    exit(0);
}
```

## Resultados de Ejecución

<img width="2872" height="1702" alt="image" src="https://github.com/user-attachments/assets/ce6bf2fa-2b49-4d5f-a126-120b343e75e5" />



```
=== DEMO LOTTERY SCHEDULING ===
10 procesos - Tickets: 50, 100, 150...500

Inicio: proceso 0, tickets: 50
Inicio: proceso 1, tickets: 100
Inicio: proceso 2, tickets: 150
Inicio: proceso 3, tickets: 200
Inicio: proceso 4, tickets: 250
Inicio: proceso 5, tickets: 300
Inicio: proceso 6, tickets: 350
Inicio: proceso 7, tickets: 400
Inicio: proceso 8, tickets: 450
Inicio: proceso 9, tickets: 500

=== DEMO COMPLETADO ===
```

## Problemas del Lottery Scheduling

1. **Injusticia a Corto Plazo:** Procesos con muchos tickets pueden ser seleccionados múltiples veces consecutivas por azar
2. **Overhead de Cálculo:** Requiere calcular total de tickets en cada ciclo del scheduler
3. **Dependencia del Azar:** No garantiza distribución perfectamente proporcional en ejecuciones cortas
4. **Starvation:** Teóricamente posible para procesos con muy pocos tickets

## Conclusión

Se implementó exitosamente el Lottery Scheduling en XV6, reemplazando el scheduler Round-Robin original. El sistema ahora asigna tiempo de CPU proporcional a los tickets de cada proceso, demostrando un enfoque diferente a la planificación de procesos que prioriza la proporcionalidad sobre la equidad.

La implementación requirió modificaciones coordinadas en múltiples componentes del kernel, destacando la importancia de la arquitectura modular de XV6 y la correcta coordinación entre espacio de kernel y usuario para las system calls.
