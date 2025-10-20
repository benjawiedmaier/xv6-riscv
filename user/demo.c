#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_process(int id) {
    int tickets = 50 * (id + 1);
    settickets(tickets);
    
    // Mensaje de inicio
    printf("Inicio: proceso %d, tickets: %d\n", id, tickets);
    
    // Trabajo intensivo
    for(int i = 0; i < 1000000; i++) {
        asm volatile("nop");
    }
    
    exit(0);
}

int main(void) {
    printf("=== DEMO LOTTERY SCHEDULING ===\n");
    printf("10 procesos - Tickets: 50, 100, 150...500\n\n");
    
    // Crear y esperar cada proceso secuencialmente
    for(int i = 0; i < 10; i++) {
        int pid = fork();
        if(pid == 0) {
            child_process(i);
        }
        // Esperar a que este proceso termine antes de crear el siguiente
        wait(0);
    }
    
    printf("\n=== DEMO COMPLETADO ===\n");
    exit(0);
}