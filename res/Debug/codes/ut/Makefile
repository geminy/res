CFLAGS += -fprofile-arcs -ftest-coverage
LFLAGS += -fprofile-arcs -ftest-coverage

.PHONY: all
all:

test: test.o libdemo
	gcc -o $@ test.o -L. -ldemo

%.o: %.c
	gcc -c $<

libdemo: demo.h demo.c
	gcc -shared -fpic -o libdemo.so $^

before_run: libdemo_cov

libdemo_cov: demo.h demo.c
	-rm libdemo.so
	gcc $(CFLAGS) -shared -fpic -o libdemo.so $^

after_run: cov_gcov cov_lcov cov_genhtml

cov_gcov: demo.h demo.c
	-gcov demo.h
	-gcov demo.c

cov_lcov:
	lcov -c -d . -o demo.info

cov_genhtml:
	genhtml demo.info -o out

clean:
	-rm *.o *.so
	-rm *.gcno *.gcda *.info
	-rm test

clean-out:
	-rm -r out
