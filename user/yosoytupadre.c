// user/yosoytupadre.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_basic_relationships(void);
void test_ancestor_chain(void);
void test_error_cases(void);

int
main(void)
{
  printf("=== Iniciando pruebas de getppid() y getancestor() ===\n\n");
  
  test_basic_relationships();
  printf("\n");
  
  test_ancestor_chain();
  printf("\n");
  
  test_error_cases();
  printf("\n");
  
  printf("=== Todas las pruebas completadas ===\n");
  exit(0);
}

void test_basic_relationships(void)
{
  int pid, my_pid, parent_pid;
  
  printf("--- Pruebas básicas de relaciones ---\n");
  
  my_pid = getpid();
  parent_pid = getppid();
  
  printf("Mi PID: %d\n", my_pid);
  printf("PID de mi padre (getppid()): %d\n", parent_pid);
  printf("Ancestro nivel 0 (debería ser %d): %d\n", my_pid, getancestor(0));
  printf("Ancestro nivel 1 (debería ser %d): %d\n", parent_pid, getancestor(1));
  
  // Crear un proceso hijo
  pid = fork();
  if(pid < 0) {
    printf("Error en fork\n");
    exit(1);
  } else if(pid == 0) {
    // Proceso hijo
    int child_pid = getpid();
    int child_ppid = getppid();
    
    printf("[HIJO] Mi PID: %d\n", child_pid);
    printf("[HIJO] PID de mi padre: %d\n", child_ppid);
    printf("[HIJO] Ancestro nivel 0: %d\n", getancestor(0));
    printf("[HIJO] Ancestro nivel 1: %d\n", getancestor(1));
    printf("[HIJO] Ancestro nivel 2: %d\n", getancestor(2));
    
    // Verificar que el padre del hijo soy yo (el proceso que hizo fork)
    if(child_ppid == my_pid) {
      printf("[HIJO] ✓ Verificación exitosa: Mi padre es el proceso correcto\n");
    } else {
      printf("[HIJO] ✗ Error: Mi padre debería ser %d pero es %d\n", my_pid, child_ppid);
    }
    
    exit(0);
  } else {
    // Proceso padre
    wait(0);
    printf("[PADRE] Mi hijo %d ha terminado\n", pid);
  }
}

void test_ancestor_chain(void)
{
  int pid;
  
  printf("--- Pruebas de cadena de ancestros ---\n");
  
  printf("Mi PID: %d\n", getpid());
  printf("Ancestro nivel 0 (yo): %d\n", getancestor(0));
  printf("Ancestro nivel 1 (padre): %d\n", getancestor(1));
  printf("Ancestro nivel 2 (abuelo): %d\n", getancestor(2));
  printf("Ancestro nivel 3 (bisabuelo): %d\n", getancestor(3));
  
  // Crear un proceso nieto
  pid = fork();
  if(pid < 0) {
    printf("Error en fork\n");
    exit(1);
  } else if(pid == 0) {
    // Proceso hijo
    int child_pid = getpid();
    printf("[HIJO] Mi PID: %d\n", child_pid);
    printf("[HIJO] Mi padre: %d\n", getppid());
    
    // El hijo crea un nieto
    int grandchild_pid = fork();
    if(grandchild_pid < 0) {
      printf("[HIJO] Error en fork\n");
      exit(1);
    } else if(grandchild_pid == 0) {
      // Proceso nieto
      printf("[NIETO] Mi PID: %d\n", getpid());
      printf("[NIETO] Mi padre: %d\n", getppid());
      printf("[NIETO] Ancestro nivel 0: %d\n", getancestor(0));
      printf("[NIETO] Ancestro nivel 1: %d\n", getancestor(1));
      printf("[NIETO] Ancestro nivel 2: %d\n", getancestor(2));
      printf("[NIETO] Ancestro nivel 3: %d\n", getancestor(3));
      exit(0);
    } else {
      wait(0);
      exit(0);
    }
  } else {
    wait(0);
  }
}

void test_error_cases(void)
{
  printf("--- Pruebas de casos de error ---\n");
  
  printf("Nivel negativo (-1): %d\n", getancestor(-1));
  printf("Nivel negativo (-5): %d\n", getancestor(-5));
  printf("Nivel muy alto (10): %d\n", getancestor(10));
  printf("Nivel muy alto (100): %d\n", getancestor(100));
}