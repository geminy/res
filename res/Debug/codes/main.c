// main.c
#include <stdio.h>

int sum(int n)
{
    int ret = 0;
    int i = 0;
    for (i = 1; i <= n; ++i) {
        ret += i;
    }
    return ret;
}

int main(void)
{
    int n = 10;
    int result = sum(n);
    printf("result = %d\n", result);

    return 0;
}

