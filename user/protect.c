#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main()
{
    printf("Solicitando 1 pagina...\n");
    char *p = sbrk(4096);

    printf("Escribiendo OK...\n");
    p[0] = 'A';

    printf("Protegiendo memoria...\n");
    if(mrdprotect(p, 4096) < 0){
        printf("Error en mrdprotect\n");
        exit(1);
    }

    printf("Intentando escribir (debería fallar)...\n");
    p[0] = 'B';   // DEBE provocar un page fault y matar el proceso

    printf("ERROR: no deberías ver este mensaje\n");
    exit(0);
}
