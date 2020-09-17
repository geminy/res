#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t value_lock = PTHREAD_MUTEX_INITIALIZER;
int value = 0;

void* thread_func1(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 1;
		printf("thread 1: value = %d\n", value);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_exit((void*)1);
}

void* thread_func2(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 10;
		printf("thread 2: value = %d\n", value);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_exit((void*)2);
}

void* thread_func3(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 100;
		printf("thread 3: value = %d\n", value);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_exit((void*)3);
}

void* thread_func4(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 1000;
		printf("thread 4: value = %d\n", value);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_exit((void*)4);
}

void* thread_func5(void *arg)
{
	pthread_mutex_lock(&value_lock);
	int count = 1;
	while (count++ <= 5) {
		value += 10000;
		printf("thread 5: value = %d\n", value);
	}
	pthread_mutex_unlock(&value_lock);
	pthread_exit((void*)5);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2, tid3, tid4, tid5;

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

	err = pthread_create(&tid3, NULL, thread_func3, NULL);
	if (0 != err) {
		printf("can't create thread 3: %s\n", strerror(err));
		abort();		
	}

	err = pthread_create(&tid4, NULL, thread_func4, NULL);
	if (0 != err) {
		printf("can't create thread 4: %s\n", strerror(err));
		abort();		
	}

	err = pthread_create(&tid5, NULL, thread_func5, NULL);
	if (0 != err) {
		printf("can't create thread : %s\n", strerror(err));
		abort();		
	}

	sleep(1);
	printf("main thread end\n");

	exit(0);
}
