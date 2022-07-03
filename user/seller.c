/*#include "lib.h"
void *consumer(void *args) {
    struct Sem *mutex = (struct sem *)((u_int *)args)[0];
    struct Sem *empty = (struct sem *)((u_int *)args)[1];
    struct Sem *full = (struct sem *)((u_int *)args)[2];
    int count = 0;
    while (1) {
        sem_wait(full);
        sem_wait(mutex);
        writef("consumer got one ! total got %d\n",  ++count);
        sem_post(mutex);
        sem_post(empty);
        if (count == 5) break;
    }
    pthread_exit(0);
}

void *producer(void *args) {
    struct Sem *mutex = (struct sem *)((u_int *)args)[0];
    struct Sem *empty = (struct sem *)((u_int *)args)[1];
    struct Sem *full = (struct sem *)((u_int *)args)[2];
    int count = 0;
    while (1) {
        sem_wait(empty);
        sem_wait(mutex);
        writef("producer  send one ! total send %d\n",  ++count);
        sem_post(mutex);
        sem_post(full);
        if (count == 5) break;
    }
    pthread_exit(0);
}      
void umain() {
    struct Sem mutex, empty, full;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    u_int arg1[3] = {&mutex, &empty, &full};
    u_int arg2[3] = {&mutex, &empty, &full};
    u_int thread1, thread2;
    pthread_create(&thread1, NULL, consumer, (void *)arg1);
    writef("consumer : 0x%x has been created !\n", thread1);
    
    pthread_create(&thread2, NULL, consumer, (void *)arg2);
    writef("consumer : 0x%x has been created !\n", thread2);
    
    pthread_create(&thread2, NULL, producer, (void *)arg2);
    writef("producer : 0x%x has been created !\n", thread2);
}   */
#include "lib.h"

void *consumer(void *args) {
    struct Sem *mutex = (struct Sem *)((u_int *)args)[0];
    struct Sem *empty = (struct Sem *)((u_int *)args)[1];
    struct Sem *full = (struct Sem *)((u_int *)args)[2];
    int count = 0;
    while (1) {
        sem_wait(full);
        sem_wait(mutex);
        writef("consumer got one ! total got %d\n", ++count);
        sem_post(mutex);
        sem_post(empty);
        if (count == 5) break;
    }
	//writef("con out\n");
    //ipthread_exit(0);
}

void *producer(void *args) {
    struct Sem *mutex = (struct Sem *)((u_int *)args)[0];
    struct Sem *empty = (struct Sem *)((u_int *)args)[1];
    struct Sem *full = (struct Sem *)((u_int *)args)[2];
    int count = 0;
    while (1) {
        sem_wait(empty);
        sem_wait(mutex);
        writef("producer send one ! total send %d\n", ++count);
        sem_post(mutex);
        sem_post(full);
        if (count == 5) break;
    }
	//writef("pro out\n");
    //pthread_exit(0);
}

void umain() {
    struct Sem mutex, empty, full;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    u_int arg1[3] = {&mutex, &empty, &full};
    u_int arg2[3] = {&mutex, &empty, &full};
    u_int thread1, thread2;
    pthread_create(&thread2, NULL, producer, (void *)arg2);
    writef("producer : 0x%x has been created !\n", thread2);
    pthread_create(&thread1, NULL, consumer, (void *)arg1);
    writef("consumer : 0x%x has been created !\n", thread1);
    int i;
	for (i = 0; i < 100000000; i++);
	writef("seller test end!\n");
}                                                                            
