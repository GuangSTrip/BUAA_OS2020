#include "lib.h"

int sem_init(sem_t *sem, int shared, u_int value) {
	return syscall_sem_init(sem, shared, value, env->env_id);
}

int sem_destroy(sem_t *sem) {
	return syscall_sem_destroy(sem);
}

int sem_wait(sem_t *sem) {
	return syscall_sem_wait(sem);
}

int sem_trywait(sem_t *sem) {
	return syscall_sem_trywait(sem);
}

int sem_post(sem_t *sem) {
	return syscall_sem_post(sem);
}

int sem_getvalue(sem_t *sem, u_int *value) {
	return syscall_sem_getvalue(sem, value);
} 
