#include "lib.h"

void *test(void *arg) {
	u_int *ta = ((u_int *)arg)[0];
	writef("a get in test thread is %d\n", *ta);
}

void umain() {
	writef("-----sharetest begin-----\n");
	u_int a = 114514;
	writef("a get in umain thread is %d\n", a);
	u_int arg[3];
	arg[0] = &a;
	pthread_t p;
	int r  = pthread_create(&p, NULL, test, (void *)arg);
	int i;
	for (i = 0; i <= 100000; i++);
	writef("test end!\n");
}
