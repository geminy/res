#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_rwlock_t value_lock = PTHREAD_RWLOCK_INITIALIZER;

int value = 0;

void* thread_read(void *arg)
{
	pthread_rwlock_rdlock(&value_lock);
	printf("thread %d: value = %d\n", (int)arg, value);
	pthread_rwlock_unlock(&value_lock);
	pthread_exit(arg);
}

void* thread_write(void *arg)
{
	pthread_rwlock_wrlock(&value_lock);
	printf("thread %d: wrlock\n", (int)arg);
	int count = 1;
	while (count++ <= 10) {
		value += 1;
	}
	usleep(1000);
	pthread_rwlock_unlock(&value_lock);
	pthread_exit(arg);
}

int main(void)
{
	pthread_t tid;

	pthread_create(&tid, NULL, thread_read, (void*)1);
	pthread_create(&tid, NULL, thread_read, (void*)2);
	pthread_create(&tid, NULL, thread_write, (void*)3);
	pthread_create(&tid, NULL, thread_write, (void*)4);
	pthread_create(&tid, NULL, thread_read, (void*)5);
	pthread_create(&tid, NULL, thread_read, (void*)6);
	pthread_create(&tid, NULL, thread_write, (void*)7);
	pthread_create(&tid, NULL, thread_write, (void*)8);
	pthread_create(&tid, NULL, thread_read, (void*)9);
	pthread_create(&tid, NULL, thread_read, (void*)10);

	sleep(1);
	printf("main thread end\n");
	exit(0);
}
