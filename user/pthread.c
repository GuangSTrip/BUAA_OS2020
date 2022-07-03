#include "lib.h"
#include <error.h>
#include <mmu.h>

int pthread_create(pthread_t *thread, const pthread, void *(*start_rountine)(void *), void *arg) {
	int newpth = syscall_thread_alloc();
	//writef("num is %d\n", newpth);	
	struct Pth *p = &env->env_pth[newpth];
	p->pth_tf.regs[29] = USTACKTOP - 4 * BY2PG * newpth;
	p->pth_tf.pc = start_rountine;
	p->pth_tf.regs[29] -= 4;
	p->pth_tf.regs[4] = arg;
	p->pth_tf.regs[31] = exit;

	syscall_set_thread_status(p->pth_id, ENV_RUNNABLE);
	*thread = p->pth_id;
	return 0;
}

void pthread_exit(void *retval) {
	u_int threadid = syscall_getthreadid();
	struct Pth *p = &env->env_pth[threadid & 0x7];
	p->pth_exit_ptr = retval;
	syscall_thread_destroy(threadid);
}

int pthread_join(pthread_t th, void **thread_return) {
	return syscall_thread_join(th, thread_return);
}

int pthread_setcancelstate(int state, int *oldvalue) {
	u_int threadid = syscall_getthreadid();
	struct Pth *p = &env->env_pth[threadid & 0x7];
	if (p->pth_id != threadid) {
		return -1;
	}
	if (oldvalue != 0) {
		*oldvalue = p->pth_cancelstate;
	}
	p->pth_cancelstate = state;
}

int pthread_setcanceltype(int type, int *oldvalue) {
	u_int threadid = syscall_getthreadid();
	struct Pth *p = &env->env_pth[threadid & 0x7];
	if (p->pth_id != threadid) {
		return -1;
	}
	if (oldvalue != 0) {
		*oldvalue = p->pth_canceltype;
	}
	p->pth_canceltype = type;
	return 0;
}

int pthread_testcancel() {
	pthread_t thread = syscall_getthreadid();
	struct Pth *p = &env->env_pth[thread & 0x7];
	if (p->pth_id != thread || p->pth_status == ENV_FREE) {
		return -1;
	}
	if (p->pth_canceled && p->pth_cancelstate == THREAD_CANCEL_ENABLE) {
		p->pth_exit_value = -1;
		syscall_thread_destroy(p->pth_id);
	}
}

int pthread_cancel(pthread_t thread) {
	struct Pth *p = &env->env_pth[thread & 0x7];
	//writef("cancelstate is %d\n", p->pth_cancelstate);
	if (p->pth_cancelstate == THREAD_CANCEL_DISABLE) {
		writef("cancel failed !!\n");
		return -1;
	}
	p->pth_exit_value = -1;
	if (p->pth_canceltype == THREAD_CANCEL_ASYNCHRONOUS) {
		syscall_thread_destroy(thread);
	} else {
		p->pth_canceled = 1;
	}
	return 0;
}
