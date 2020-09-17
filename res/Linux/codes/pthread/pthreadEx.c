#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_t g_tid;

void printIds(const char *s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
		(unsigned int)tid, (unsigned int)tid);
}

void* threadFunc(void *arg)
{
	printIds("new thread: ");
	return ((void*)0);
}

int main(void)
{
	int err;
	err = pthread_create(&g_tid, NULL, threadFunc, NULL);
	if (0 != err) {
		printf("can't create thread: %s\n", strerror(err));
		abort();		
	}
	printIds("main thread: ");
	sleep(1);
	exit(0);
}
