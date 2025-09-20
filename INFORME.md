# INFORME - Tarea 1: Implementación de Llamadas al Sistema en xv6

## 📋 Descripción
Implementación de las syscalls `getppid()` y `getancestor()` en xv6-riscv para la asignatura de Sistemas Operativos.

## 🛠️ Pasos de Implementación

### Paso 1: Configuración inicial mit-pdos/xv6-riscv

se crea una rama para la tarea 1 a partir del repositorio original 

### Paso 2: Agregar números de syscall
**Archivo: `kernel/syscall.h`**
```c
#define SYS_getppid    22
#define SYS_getancestor 23
```

### Paso 3: Registrar syscalls en la tabla
**Archivo: `kernel/syscall.c`**
```c
extern uint64 sys_getppid(void);
extern uint64 sys_getancestor(void);

[SYS_getppid]    sys_getppid,
[SYS_getancestor] sys_getancestor,
```

### Paso 4: Implementar funciones del kernel
**Archivo: `kernel/sysproc.c`**
```c
uint64
sys_getppid(void)
{
  struct proc *p = myproc();
  
  if(p->parent == 0)
    return 1;
  
  return p->parent->pid;
}

uint64
sys_getancestor(void)
{
  int level;
  struct proc *p;
  
  if(argint(0, &level) < 0)
    return -1;
  
  if(level < 0)
    return -1;
  
  p = myproc();
  
  for(int i = 0; i < level; i++) {
    if(p->parent == 0) {
      return -1;
    }
    p = p->parent;
  }
  
  return p->pid;
}
```

### Paso 5: Agregar prototipos de usuario
**Archivo: `user/user.h`**
```c
int getppid(void);
int getancestor(int level);
```

### Paso 6: Generar stubs de assembler
**Archivo: `user/usys.pl`**
```perl
entry("getppid");
entry("getancestor");
```

### Paso 7: Crear programa de prueba
**Archivo: `user/yosoytupadre.c`**
```c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void test_basic_relationships(void);
void test_ancestor_chain(void);
void test_error_cases(void);

int main(void)
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
```

### Paso 8: Actualizar Makefile
**Archivo: `Makefile`**
```makefile
UPROGS=\
	$U/_cat\
	$U/_echo\
	$U/_forktest\
	$U/_grep\
	$U/_init\
	$U/_kill\
	$U/_ln\
	$U/_ls\
	$U/_mkdir\
	$U/_rm\
	$U/_sh\
	$U/_stressfs\
	$U/_usertests\
	$U/_grind\
	$U/_wc\
	$U/_zombie\
	$U/_logstress\
	$U/_forphan\
	$U/_dorphan\
	$U/_yosoytupadre\
```

## ⚠️ Errores y Soluciones

| Error | Solución |
|-------|----------|
| `void value not ignored as it ought to be` | Agregar `#include "syscall.h"` en `kernel/sysproc.c` |
| `implicit declaration of function` | Agregar prototipos en `user/user.h` |
| Dudas sobre ubicación de cambios | Confirmar edición en archivos fuente desde WSL/Ubuntu |

## 🧪 Resultados de las Pruebas

**Output de `yosoytupadre`:**
```
=== Iniciando pruebas de getppid() y getancestor() ===

--- Pruebas básicas de relaciones ---
Mi PID: 4
PID de mi padre (getppid()): 2
Ancestro nivel 0 (debería ser 4): 4
Ancestro nivel 1 (debería ser 2): 2
[HIJO] Mi PID: 5
[HIJO] PID de mi padre: 4
[HIJO] Ancestro nivel 0: 5
[HIJO] Ancestro nivel 1: 4
[HIJO] Ancestro nivel 2: 2
[HIJO] ✓ Verificación exitosa: Mi padre es el proceso correcto
[PADRE] Mi hijo 5 ha terminado

--- Pruebas de cadena de ancestros ---
Mi PID: 4
Ancestro nivel 0 (yo): 4
Ancestro nivel 1 (padre): 2
Ancestro nivel 2 (abuelo): 1
Ancestro nivel 3 (bisabuelo): -1
[HIJO] Mi PID: 6
[HIJO] Mi padre: 4
[NIETO] Mi PID: 7
[NIETO] Mi padre: 6
[NIETO] Ancestro nivel 0: 7
[NIETO] Ancestro nivel 1: 6
[NIETO] Ancestro nivel 2: 4
[NIETO] Ancestro nivel 3: 2

--- Pruebas de casos de error ---
Nivel negativo (-1): -1
Nivel negativo (-5): -1
Nivel muy alto (10): -1
Nivel muy alto (100): -1

=== Todas las pruebas completadas ===
```

## ✅ Conclusiones

Las syscalls implementadas funcionan correctamente:
- `getppid()` retorna el PID del padre.
- `getancestor()` recorre correctamente la jerarquía de ancestros.
- Se validan correctamente los casos de error (niveles negativos o muy altos).
- Las pruebas confirmaron que las relaciones padre-hijo-abuelo se mantienen en procesos creados mediante `fork()`.

## 📦 Archivos Modificados
- `kernel/syscall.h`
- `kernel/syscall.c`
- `kernel/sysproc.c`
- `user/user.h`
- `user/usys.pl`
- `user/yosoytupadre.c`
- `Makefile`
