#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
//#include <varargs.h>
#include <iso646.h>

#define MultiArgs(format, ...) printf(format, __VA_ARGS__)
#define PrintArgs(format, args...) printf(format, ##args)

void printargs(int num, ...)
{
    va_list va;
    va_start(va, num);
    int index = 0;
    int value = num;
    while (1) {
        printf("args[%d] = %d\n", index, value);
        if ((value = va_arg(va, int)) == 0) {
            break;
        }
        index++;
    }
    va_end(va);
}

int main(void)
{
    // stddef
    struct s
    {
        int a;
        int b;
    } x;
    size_t size = sizeof(x.a);
    size_t offset = offsetof(struct s, b);
    ptrdiff_t diff = &x.b - &x.a;
    printf("size = %d, offset = %d\n", size, offset); // size = 4, offset = 4
    printf("%p - %p = %d\n", &x.b, &x.a, diff); // 0x7fff931ff6d4 - 0x7fff931ff6d0 = 1

    // errno
    errno = 0;
    int s = sqrt(-1);
    if (errno) {
        printf("errno = %d\n", errno); // errno = 33
        perror("sqrt failed"); // sqrt failed: Numerical argument out of domain
        printf("error: %s\n", strerror(errno)); // error: Numerical argument out of domain
    }

    // stdarg
    printargs(1, 2, 3, 4, 5, 0);
    MultiArgs("%s, %s\n", "hello", "world!");
    PrintArgs("%s, %s\n", "hello", "c language!");
    /*
    args[0] = 1
    args[1] = 2
    args[2] = 3
    args[3] = 4
    args[4] = 5
    hello, world!
    hello, c language!
    */

    return 0;
}

