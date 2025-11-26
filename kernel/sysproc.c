#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// ------------------------------
// SYSCALLS SEGÚN TU VERSIÓN XV6
// ------------------------------

uint64
sys_fork(void)
{
  return kfork();     // tu kernel SI tiene kfork()
}

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);           // tu kernel SI tiene kexit()
  return 0;
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);    // tu kernel SI tiene kwait()
}

uint64
sys_sbrk(void)
{
  int n;
  argint(0, &n);

  struct proc *p = myproc();
  uint64 addr = p->sz;

  if(growproc(n) < 0)
    return -1;

  return addr;
}

uint64
sys_kill(void)
{
  int pid;
  argint(0, &pid);
  return kkill(pid);  // tu kernel SI tiene kkill()
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

// Tu kernel NO implementa kpause() → devolvemos 0
uint64
sys_pause(void)
{
  return 0;
}

uint64
sys_uptime(void)
{
  uint xticks = 0;
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sleep(void)
{
  int n;
  argint(0, &n);

  acquire(&tickslock);
  uint64 ticks0 = ticks;

  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);  // esta es la función REAL de tu kernel
  }

  release(&tickslock);
  return 0;
}

uint64
sys_mrdprotect(void)
{
  uint64 addr;
  int len;

  argaddr(0, &addr);
  argint(1, &len);

  if (len <= 0) return -1;

  return mrdprotect(addr, len);
}

uint64
sys_munrdprotect(void)
{
  uint64 addr;
  int len;

  argaddr(0, &addr);
  argint(1, &len);

  if (len <= 0) return -1;

  return munrdprotect(addr, len);
}
