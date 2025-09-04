# Bitácora de instalación y prueba de **xv6**

## 1. Preparar el repositorio

1. Hice un **fork** de <https://github.com/mit-pdos/xv6-riscv.git> a mi cuenta.
2. Creé la rama **`grupoG-t0`** para la _Tarea 0_.
3. Cloné mi fork en el computador, realizo los comandos para instalar wsl y las dependencias necesarias para xv6-riscv, y por ultimo compilo con qemu.

   ```bash
   git clone https://github.com/benjawiedmaier/xv6-riscv.git
   cd xv6-riscv
  
   # Instalar WSL 2
   wsl --install            # reiniciar cuando lo pida

   # Iniciar la distro
   wsl -d Ubuntu

   # Actualizar paquetes y añadir dependencias
   sudo apt update
   sudo apt install -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-unknown-elf binutils-riscv64-unknown-elf
   # Desde la carpeta clonada
   make qemu
  
  4. ejecute los comandos de prueba y tome los screenshots.
     
       ```bash
       $ ls
       $ echo "Hola xv6"
       $ cat README
