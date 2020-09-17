#include <stdio.h>
#include "demo.h"

void demo_foo()
{
	printf("%s\n", __func__);
}

void demo_foo2(int i)
{
	if (i > 0) {
		printf("%s pos\n", __func__);
		if (0 == i % 2) {
			printf("%s even\n", __func__);
		}
		else {
			printf("%s odd\n", __func__);
		}
	}
	else if (i < 0) {
		printf("%s neg\n", __func__);
	}
	else {
		printf("%s zero\n", __func__);
	}
}
