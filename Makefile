CC=gcc
CFLAGS= -Wall -std=c99
SOURCES=./qstring/qstring.c
TESTS=./qstring/test.c

test:
	$(CC) $(CFLAGS) -o test.out $(SOURCES) $(TESTS)
	./test.out
	rm test.out
