// pthreadClean.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void cleanup(void *arg)
{
	printf("cleanup: %s\n", (char*)arg);
}

void* threadFunc1(void *arg)
{
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	printf("thread 1 push complete\n");
	if (arg) {
		return (void*)1;
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	return (void*)1;
}

void* threadFunc2(void *arg)
{
	printf("thread 2 start\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	printf("thread 2 push complete\n");
	if (arg) {
		pthread_exit((void*)2);
	}
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
	pthread_exit((void*)2);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	void *ret;

	err = pthread_create(&tid1, NULL, threadFunc1, (void*)1);
	if (0 != err) {
		printf("can't create thread 1: %s\n", strerror(err));
		abort();		
	}

	err = pthread_create(&tid2, NULL, threadFunc2, (void*)1);
	if (0 != err) {
		printf("can't create thread 2: %s\n", strerror(err));
		abort();		
	}

	err = pthread_join(tid1, &ret);
	if (0 != err) {
		printf("can't join with thread 1: %s\n", strerror(err));
		abort();		
	}	
	printf("thread 1 exit code %d\n", (int)ret);


	err = pthread_join(tid2, &ret);
	if (0 != err) {
		printf("can't join with thread 2: %s\n", strerror(err));
		abort();		
	}
	printf("thread 2 exit code %d\n", (int)ret);

	exit(0);
}
