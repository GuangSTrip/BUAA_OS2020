#include "lib.h"
void *test1(void *arg) {
	int count = 0;
	int oldvalue;
	pthread_setcancelstate(THREAD_CANCEL_DISABLE, &oldvalue);
	syscall_yield();
	int i;
	for (i = 0; i < 1000000; i++);
}

void *test2(void *arg) {
	int count = 0;
	int oldvalue;
	pthread_setcanceltype(THREAD_CANCEL_ASYNCHRONOUS, &oldvalue);
	syscall_yield();
	//writef("asynchronous fail!\n");
}

void *test3(void *arg) {
	int oldvalue;
	pthread_setcanceltype(THREAD_CANCEL_DEFERRED, &oldvalue);
	syscall_yield();
	writef("test3 wait for cancel point\n");
	while(1) pthread_testcancel();
	writef("deferred fail!\n");
}

void umain() {
	pthread_t p1, p2, p3;
	int i;
	int ret = pthread_create(&p1, NULL, test1, NULL);
	ret = pthread_create(&p2, NULL, test2, NULL);
	ret = pthread_create(&p3, NULL, test3, NULL);
	syscall_yield();
	pthread_cancel(p1);
	pthread_cancel(p2);
	pthread_cancel(p3);
	for (i = 0;i < 100000;i++);
	*(int *)KERNBASE = 0;
}
