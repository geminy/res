#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int value = 0;

pthread_cond_t value_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t value_mutex = PTHREAD_MUTEX_INITIALIZER;


void* thread_read(void *arg)
{
	pthread_mutex_lock(&value_mutex);
	pthread_cond_wait(&value_cond, &value_mutex);
	printf("thread %d: value = %d\n", (int)arg, value);
	pthread_mutex_unlock(&value_mutex);
	pthread_exit(arg);
}

void* thread_write(void *arg)
{
	pthread_mutex_lock(&value_mutex);
	printf("thread %d: mutex\n", (int)arg);
	value += 100;
	pthread_cond_signal(&value_cond);
	pthread_mutex_unlock(&value_mutex);
	pthread_exit(arg);
}

int main(void)
{
	pthread_t tid;

	pthread_create(&tid, NULL, thread_read, (void*)1);
	usleep(500 * 1000);
	pthread_create(&tid, NULL, thread_write, (void*)2);

	sleep(1);
	printf("main thread end\n");
	exit(0);
}
