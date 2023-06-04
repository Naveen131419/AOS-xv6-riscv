#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "stddef.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

//syscall sysinfo to print number of active processess, number of system calls after bootup and number of free pages
uint64
sys_info(void)
{
  int n;
	argint(0,&n);
	if (n == 0){
		return  total_process_count();
	}
	else if (n == 1){
    return syscount;
	}
  else if (n == 2){
    int count = kfreepages();
    return count;
  }
	return -1;
}

//syscall procinfo to print ppid, number of system calls made by current process and current process's memory size
uint64
sys_procinfo(void)
{
  uint64 ptr;
  argaddr(0, &ptr);
  struct proc *p = myproc();
  if(copyout(p->pagetable, ptr, (char*)&(p->parent->pid), sizeof(p->parent->pid)) < 0)
    return -1;
  if(copyout(p->pagetable, ptr+4, (char*)&(p->syscallCount), sizeof(p->syscallCount)) < 0)
    return -1;
  int temp;
  if((p->sz)%PGSIZE == 0)
    temp = p->sz/PGSIZE;
  else
    temp = p->sz/PGSIZE + 1;
  if(copyout(p->pagetable, ptr+8, (char*)&temp, sizeof(p->sz)) < 0)
    return -1;
  return 0;
}

//syscall sched_tickets to set the caller process’s ticket value to the given parameter
//return value of this funciton is int (0)
uint64
sys_sched_tickets(void)
{
  int n;
  argint(0,&n);         //n is the inout parameter from user level, here it is number of tickets for that process
  return set_tickets_to_proc(n);
}

//syscall sched_statistics to print pid, name, tickets and ticks of the process
//this function returns int 0
uint64
sys_sched_statistics(void)
{
  print_sched_statistics();
  return 0;
}

//syscall sys_clone to ceate a child thread
uint64
sys_clone(void)
{
  uint64 temp;
  argaddr(0,&temp);
  return make_clone((void*)temp);   //returns PID of the child to the parent
}                                   //returns 0 to the child thread

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
uint64 sys_hello(void)
{
  int n;
  argint(0,&n);
  print_hello(n);
  return 0;
}