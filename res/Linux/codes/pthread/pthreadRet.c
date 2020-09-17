#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* threadFunc1(void *arg)
{
	printf("thread 1 returning\n");
	return (void*)1;
}

void* threadFunc2(void *arg)
{
	printf("thread 2 exiting\n");
	pthread_exit((void*)2);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	void *ret;

	err = pthread_create(&tid1, NULL, threadFunc1, NULL);
	if (0 != err) {
		printf("can't create thread 1: %s\n", strerror(err));
		abort();		
	}

	err = pthread_create(&tid2, NULL, threadFunc2, NULL);
	if (0 != err) {
		printf("can't create thread 2: %s\n", strerror(err));
		abort();		
	}

	err = pthread_join(tid1, &ret);
	if (0 != err) {
		printf("can't join with thread 1: %s\n", strerror(err));
		abort();		
	}
	printf("thread 1 exit code %x\n", (int)ret);

	err = pthread_join(tid2, &ret);
	if (0 != err) {
		printf("can't join with thread 2: %s\n", strerror(err));
		abort();		
	}
	printf("thread 2 exit code %d\n", (int)ret);

	exit(0);
}
