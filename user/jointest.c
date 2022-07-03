#include "lib.h"
void *test1(void *arg) {
	syscall_yield();
	writef("test1 end!\n");
}

void *test2(void *arg) {
	pthread_t p1 = ((u_int *)arg)[0];
	pthread_join(p1, NULL);
	writef("test2 end!\n");
}

void *test3(void *arg) {
	pthread_t p2 = ((u_int *)arg)[1];
	pthread_join(p2,NULL);
	writef("test3 end!\n");
}

void umain() {
	writef("-----jointest begin-----\n");
	u_int a[3];
	pthread_t p1, p2, p3;
	int r;
	r = pthread_create(&p1, NULL, test1, (void *)a);
	a[0] = p1;
	r = pthread_create(&p2, NULL, test2, (void *)a);
	a[1] = p2;
	r = pthread_create(&p3, NULL, test3, (void *)a);
	pthread_join(p3, NULL);
	syscall_yield();
	int i;
	writef("umain end!\n");
	for (i = 0;i < 100000; i++);
}
