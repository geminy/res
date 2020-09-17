#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct foo
{
	int a, b, c, d;
};

void printFoo(const char *s, const struct foo *fp)
{
	printf(s);
	printf(" structure at 0x%x\n", (unsigned)fp);
	printf(" foo.a = %d\n", fp->a);
	printf(" foo.b = %d\n", fp->b);
	printf(" foo.c = %d\n", fp->c);
	printf(" foo.d = %d\n", fp->d);
}

void* threadFunc1(void *arg)
{
	struct foo foo = {1, 2, 3, 4};
	printFoo("thread 1:\n", &foo);
	pthread_exit((void*)&foo);
}

void* threadFunc2(void *arg)
{
	printf("thread 2: ID is %u\n", (unsigned int)pthread_self());
	pthread_exit((void*)0);
}

int main(void)
{
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;

	err = pthread_create(&tid1, NULL, threadFunc1, NULL);
	if (0 != err) {
		printf("can't create thread 1: %s\n", strerror(err));
		abort();		
	}
	err = pthread_join(tid1, (void*)&fp);
	if (0 != err) {
		printf("can't join with thread 1: %s\n", strerror(err));
		abort();		
	}
	sleep(1);
	printf("parent starting second thread\n");

	err = pthread_create(&tid2, NULL, threadFunc2, NULL);
	if (0 != err) {
		printf("can't create thread 2: %s\n", strerror(err));
		abort();		
	}
	sleep(1);
	printFoo("parent: \n", fp);

	exit(0);
}
