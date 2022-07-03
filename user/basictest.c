#include "lib.h"

void *fun1(void *arg) {
	int r = 0;
	writef("create success!\n");
	writef("arg[0] is %d\n", ((int *)arg)[0]);
	pthread_exit(&r);
	writef("arg[1] is %d\n", ((int *)arg)[1]);
	writef("arg[2] is %d\n", ((int *)arg)[2]);
	writef("fun1 out\n");
}

void umain() {
	writef("\n--basictest start---\n");	
	pthread_t th1;
	u_int trans[3];
	trans[0] = 114514;
	trans[1] = 19260817;
	trans[2] = 20373025;
	int r = pthread_create(&th1, NULL, fun1, (void *)trans);
	int i;
	for (i = 0;i < 1000000;i++) {

	}
	//for(;;);
	writef("test end!\n");
	*(int *) KERNBASE = 0;
}
