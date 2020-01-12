CFLAGS=-std=c11 -g -static

compiler: compiler.c

test: compiler
	./test.sh

clean:
		rm -f 9cc *.o *~ tmp*

.PHONY: test clean