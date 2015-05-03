CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -O0 -Wconversion

.PHONY: all clean

all: aufgabe-5.1

aufgabe-5.1: aufgabe-5.1.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf aufgabe-5.1
	rm -rf *.o
