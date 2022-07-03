/* See COPYRIGHT for copyright information. */

#ifndef _ENV_H_
#define _ENV_H_

#include "types.h"
#include "queue.h"
#include "trap.h"
#include "mmu.h" 

#define LOG2NENV	10
#define NENV		(1<<LOG2NENV)
#define ENVX(envid)	((envid) & (NENV - 1))
#define GET_ENV_ASID(envid) (((envid)>> 11)<<6)

// Values of env_status in struct Env
#define ENV_FREE	0
#define ENV_RUNNABLE		1
#define ENV_NOT_RUNNABLE	2

//lab4-challenge
#define NPTH 8
#define THREAD_CANCEL_ENABLE		0
#define THREAD_CANCEL_DISABLE		1
#define THREAD_CANCEL_DEFERRED		0
#define THREAD_CANCEL_ASYNCHRONOUS	1
#define	THREAD_CANCEL_EXIT	9
#define SEM_VALID	1
#define SEM_FREE	0
#define NWAITSEM	8

struct Sem {
	u_int sem_value;
	u_int sem_status;
	u_int sem_shared;
	u_int sem_envid;
	u_int sem_wait_list[8];
	u_int sem_wait_num;
	u_int sem_wait_i;
};

struct Pth {
	//environment
	struct Trapframe pth_tf;
	LIST_ENTRY(Pth) pth_link;
	//message
	u_int pth_id;
	u_int pth_status;
	u_int pth_pri;
	u_int pth_in_ienv;
	u_int pth_envid;
	LIST_ENTRY(Pth) pth_sched_link;
	//join
	LIST_ENTRY(Pth) pth_joined_link;
	LIST_HEAD(Pth_joined_list, Pth);
	struct Pth_joined_list pth_joined_list;
	void **pth_join_value_ptr;
	u_int pth_detach;
	//exit 
	void *pth_exit_ptr;
	int pth_exit_value;
	//cancel
	int pth_cancelstate;
	int pth_canceltype;
	u_int pth_canceled;
};

struct Env {
	struct Trapframe env_tf;        // Saved registers
	LIST_ENTRY(Env) env_link;       // Free list
	u_int env_id;                   // Unique environment identifier
	u_int env_parent_id;            // env_id of this env's parent
	u_int env_status;               // Status of the environment
	Pde  *env_pgdir;                // Kernel virtual address of page dir
	u_int env_cr3;
	LIST_ENTRY(Env) env_sched_link;
        u_int env_pri;
	// Lab 4 IPC
	u_int env_ipc_value;            // data value sent to us 
	u_int env_ipc_from;             // envid of the sender  
	u_int env_ipc_recving;          // env is blocked receiving
	u_int env_ipc_dstva;		// va at which to map received page
	u_int env_ipc_perm;		// perm of page mapping received

	// Lab 4 fault handling
	u_int env_pgfault_handler;      // page fault state
	u_int env_xstacktop;            // top of exception stack

	// Lab 6 scheduler counts
	u_int env_runs;			// number of times been env_run'ed
	u_int env_nop;                  // align to avoid mul instruction
	
	//lab4-challenge-Pth
	struct Pth env_pth[8];
	u_int env_pth_num;
};

LIST_HEAD(Env_list, Env);
extern struct Env *envs;		// All environments
extern struct Env *curenv;	        // the current env
extern struct Env_list env_sched_list[2]; // runnable env list

void env_init(void);
int env_alloc(struct Env **e, u_int parent_id);
void env_free(struct Env *);
void env_create_priority(u_char *binary, int size, int priority);
void env_create(u_char *binary, int size);
void env_destroy(struct Env *e);

int envid2env(u_int envid, struct Env **penv, int checkperm);
void env_run(struct Env *e);
//lab4-chllenge-pthread
LIST_HEAD(Pth_list, Pth);
extern struct Pth *curpth;
extern struct Pth_list pth_sched_list[2];
int thread_alloc(struct Env *e, struct Pth **newp);
void pth_run(struct Pth *p);
int pthid2pth(u_int threadid, struct Pth **p);
void thread_free(struct Pth *p);
void thread_destroy(struct Pth *p);

// for the grading script
#define ENV_CREATE2(x, y) \
{ \
	extern u_char x[], y[]; \
	env_create(x, (int)y); \
}
#define ENV_CREATE_PRIORITY(x, y) \
{\
        extern u_char binary_##x##_start[]; \
        extern u_int binary_##x##_size;\
        env_create_priority(binary_##x##_start, \
                (u_int)binary_##x##_size, y);\
}
#define ENV_CREATE(x) \
{ \
	extern u_char binary_##x##_start[];\
	extern u_int binary_##x##_size; \
	env_create(binary_##x##_start, \
		(u_int)binary_##x##_size); \
}

#endif // !_ENV_H_
