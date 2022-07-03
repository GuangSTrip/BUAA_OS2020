#include "lib.h"

void *barber(void *arg) {
	writef("barber begin\n");
	sem_t *cus = (sem_t *)((u_int *)arg)[0];
	sem_t *bar = (sem_t *)((u_int *)arg)[1];
	sem_t *mutex = (sem_t *)((u_int *)arg)[2];
	u_int *empty = ((u_int*)arg)[3];
	while (1) {
		sem_wait(cus);
		sem_wait(mutex);
		(*empty)++;
		writef("a customer leave, empty is %d\n", *empty);
		sem_post(bar);
		sem_post(mutex);
		syscall_yield();
	}
}

void *customer(void *arg) {
	writef("customer begin\n");
	sem_t *cus = (sem_t *)((u_int *)arg)[0];
        sem_t *bar = (sem_t *)((u_int *)arg)[1];
        sem_t *mutex = (sem_t *)((u_int *)arg)[2];
	u_int *empty = ((u_int*)arg)[3];
	//writef("------------empty is %d", *empty);
	int i;
	for (i = 1; i < 1000; i++) {
		writef("customer %d come\n", i);
		sem_wait(mutex);
		if ((*empty) > 0) {
			(*empty)--;
			writef("customer %d sit down\n",i);
			//writef("emtpy is %d\n", *empty);
			sem_post(cus);
			sem_post(mutex);
			sem_wait(bar);
		} else {
			sem_post(mutex);
		}
		syscall_yield();
	}
}

void umain() {
	writef("-----barbertest begin-----");
	sem_t cus, bar;
	sem_t mutex;
	sem_init(&cus, 0, 0);
	sem_init(&bar, 0, 0);
	sem_init(&mutex, 0, 1);
	u_int empty = 5;
	u_int a[4];
	u_int b[4];
	a[0] = &cus;
	a[1] = &bar;
	a[2] = &mutex;
	a[3] = &empty;
	b[0] = &cus;
	b[1] = &bar;
	b[2] = &mutex;
	b[3] = &empty;
	pthread_t p1, p2;
	pthread_create(&p1, NULL, barber, (void *)a);
	pthread_create(&p2, NULL, customer, (void *)b);
	while(1) {
		syscall_yield();
	}
}
