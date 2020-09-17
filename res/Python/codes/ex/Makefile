.PHONY: all
all: libhello libex

libhello: helloworld.c
	gcc -fPIC -shared -o libhello.so helloworld.c

libex: PythonEx.c
	gcc -fPIC -shared -o libex.so PythonEx.c -I/usr/include/python2.7
clean:
	-rm *.so
