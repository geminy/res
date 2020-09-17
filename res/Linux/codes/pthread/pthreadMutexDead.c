#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t value_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t value2_lock = PTHREAD_MUTEX_INITIALIZER;

int value = 0;
int value2 = 0;

void* thread_func1(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 1;
		printf("thread 1: value = %d\n", value);
	}
	pthread_mutex_lock(&value2_lock);
	count = 1;
	while (count++ <= 5) {
		value2 += 1;
		printf("thread 1: value2= %d\n", value2);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_mutex_unlock(&value2_lock);
	pthread_exit((void*)1);
}

void* thread_func2(void *arg)
{
	pthread_mutex_lock(&value2_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 10;
		printf("thread 2: value = %d\n", value);
	}
	pthread_mutex_lock(&value_lock);
	count = 1;
	while (count++ <= 5) {
		value2 += 10;
		printf("thread 2: value2= %d\n", value2);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_mutex_unlock(&value2_lock);
	pthread_exit((void*)1);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;

	err = pthread_create(&tid1, NULL, thread_func1, NULL);
	if (0 != err) {
		printf("can't create thread 1: %s\n", strerror(err));
		abort();		
	}

	err = pthread_create(&tid2, NULL, thread_func2, NULL);
	if (0 != err) {
		printf("can't create thread 2: %s\n", strerror(err));
		abort();		
	}

	sleep(1);
	printf("main thread end\n");

	exit(0);
}
