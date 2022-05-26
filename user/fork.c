// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>


/* ----------------- help functions ---------------- */

/* Overview:
 * 	Copy `len` bytes from `src` to `dst`.
 *
 * Pre-Condition:
 * 	`src` and `dst` can't be NULL. Also, the `src` area
 * 	 shouldn't overlap the `dest`, otherwise the behavior of this
 * 	 function is undefined.
 */
static u_int f[3][1024];
void user_bcopy(const void *src, void *dst, size_t len)
{
	void *max;

	//	writef("~~~~~~~~~~~~~~~~ src:%x dst:%x len:%x\n",(int)src,(int)dst,len);
	max = dst + len;

	// copy machine words while possible
	if (((int)src % 4 == 0) && ((int)dst % 4 == 0)) {
		while (dst + 3 < max) {
			*(int *)dst = *(int *)src;
			dst += 4;
			src += 4;
		}
	}

	// finish remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst = *(char *)src;
		dst += 1;
		src += 1;
	}

	//for(;;);
}

/* Overview:
 * 	Sets the first n bytes of the block of memory
 * pointed by `v` to zero.
 *
 * Pre-Condition:
 * 	`v` must be valid.
 *
 * Post-Condition:
 * 	the content of the space(from `v` to `v`+ n)
 * will be set to zero.
 */
void user_bzero(void *v, u_int n)
{
	char *p;
	int m;

	p = v;
	m = n;

	while (--m >= 0) {
		*p++ = 0;
	}
}
/*--------------------------------------------------------------*/

/* Overview:
 * 	Custom page fault handler - if faulting page is copy-on-write,
 * map in our own private writable copy.
 *
 * Pre-Condition:
 * 	`va` is the address which leads to a TLBS exception.
 *
 * Post-Condition:
 *  Launch a user_panic if `va` is not a copy-on-write page.
 * Otherwise, this handler should map a private writable copy of
 * the faulting page at correct address.
 */
/*** exercise 4.13 ***/
static void
pgfault(u_int va)
{
	u_int *tmp;
	//	writef("fork.c:pgfault():\t va:%x\n",va);
	u_int perm;
	tmp = USTACKTOP;
	va = ROUNDDOWN(va, BY2PG);
	perm = (((Pte *)(*vpt))[VPN(va)] & 0xfff);
	if ((perm & PTE_COW) == 0) {
		user_panic("panic at pgfault : NOT COW");
	}
	if (syscall_mem_alloc(0, tmp, (PTE_V | PTE_R)) < 0) {
		user_panic("panic at pgfault : page alloc failed");
	}
	//map the new page at a temporary place
	//copy the content
	user_bcopy(va, tmp, BY2PG);
	if (syscall_mem_map(0, tmp, 0, va, PTE_V | PTE_R) < 0) {
		user_panic("panic at pgfault : map failed");
	}
	if (syscall_mem_unmap(0, tmp) < 0) {
		user_panic("panic at pgfault : unmap failed");
	}
	//map the page on the appropriate place
	//unmap the temporary place

}

/* Overview:
 * 	Map our virtual page `pn` (address pn*BY2PG) into the target `envid`
 * at the same virtual address.
 *
 * Post-Condition:
 *  if the page is writable or copy-on-write, the new mapping must be
 * created copy on write and then our mapping must be marked
 * copy on write as well. In another word, both of the new mapping and
 * our mapping should be copy-on-write if the page is writable or
 * copy-on-write.
 *
 * Hint:
 * 	PTE_LIBRARY indicates that the page is shared between processes.
 * A page with PTE_LIBRARY may have PTE_R at the same time. You
 * should process it correctly.
 */
/*** exercise 4.10 ***/
static void
duppage(u_int envid, u_int pn)
{
	u_int addr;
	u_int perm;
	addr = (pn << PGSHIFT);
	//writef("%x\n",addr);
	perm = ((Pte *)(*vpt))[pn] & 0xfff;
	if ((perm & PTE_R) == 0) {
		if (syscall_mem_map(0, addr, envid, addr, perm) < 0) {
			user_panic("panic at duppage : !PTE_R");
		}
	} else if ((perm & PTE_COW) != 0) {
		if (syscall_mem_map(0, addr, envid, addr, perm) < 0) {
			user_panic("panic at duppage : PTE_COW");
		}
	} else if ((perm & PTE_LIBRARY) != 0) {
		if (syscall_mem_map(0, addr, envid, addr, perm) < 0) {
			user_panic("panic at duppage : PTE_LIBRARY");
		}
	} else {
		if (syscall_mem_map(0, addr, envid, addr, perm | PTE_COW) < 0) {
			user_panic("panic at duppage : normal for child");
		}
		if (syscall_mem_map(0, addr, 0, addr, perm | PTE_COW) < 0) {
			user_panic("panic at duppage : normal for father");
		}
	}
	//	user_panic("duppage not implemented");
}

/* Overview:
 * 	User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: use vpd, vpt, and duppage.
 * Hint: remember to fix "env" in the child process!
 * Note: `set_pgfault_handler`(user/pgfault.c) is different from
 *       `syscall_set_pgfault_handler`.
 */
/*** exercise 4.9 4.15***/
extern void __asm_pgfault_handler(void);
int
fork(void)
{
	// Your code here.
	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i;


	//The parent installs pgfault using set_pgfault_handler
	set_pgfault_handler(pgfault);
	//alloc a new alloc
	newenvid = syscall_env_alloc();
	if (newenvid == 0) {
		env = &envs[ENVX(syscall_getenvid())];
		return 0;	
	}
	u_int curenv = syscall_getenvid();
	f[0][ENVX(newenvid)] = f[0][ENVX(curenv)];
	f[1][ENVX(newenvid)] = f[1][ENVX(curenv)];
	f[2][ENVX(newenvid)] = f[2][ENVX(curenv)];
	for (i = 0;i < USTACKTOP;i += BY2PG) {
		if (((((Pde *)(*vpd))[i >> PDSHIFT]) & PTE_V) && ((((Pte *)(*vpt))[i >> PGSHIFT]) & PTE_V)) {	
			//writef("%x\n",(*vpt)[VPN(i)]);
			duppage(newenvid, VPN(i));
		} 
	}
	if (syscall_mem_alloc(newenvid, UXSTACKTOP - BY2PG, PTE_V | PTE_R) < 0) {
		user_panic("panic at fork : alloc failed");
	}
	if (syscall_set_pgfault_handler(newenvid, __asm_pgfault_handler, UXSTACKTOP) < 0) {
		user_panic("panic at fork : set pgfault handler failed");
	}
	if (syscall_set_env_status(newenvid , ENV_RUNNABLE) < 0) {
		user_panic("panic at fork : status set failed");
	}
	return newenvid;
}
//static u_int f[3][1024];
void signal(int sig, void (*handler)(int)) {
	int t;
	if (sig == 15) {
		t = 0;
	}
	if (sig == 11) {
		t = 1;
	}	
	if (sig == 18) {
		t = 2;
	}
	int envid = syscall_getenvid();
	if (handler == NULL) {
		f[t][ENVX(envid)] = 0;	
	} else {
		f[t][ENVX(envid)] = (u_int)handler;
	}
}


// Challenge!
int
sfork(void)
{
	user_panic("sfork not implemented");
	return -E_INVAL;
}
