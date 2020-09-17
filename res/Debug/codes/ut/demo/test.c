#include <stdio.h>
#include "demo.h"

int main(int argc, char* argv[])
{
	printf("%s unit test begin\n", __func__);

	demo_foo();
	demo_foo2(2);
	demo_foo2(1);
	demo_foo2(0);
	demo_foo2(-1);

	printf("%s unit test end\n", __func__);

	return 0;
}
