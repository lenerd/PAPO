CC=gcc
MPICC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -Wconversion

.PHONY: all clean

all: factorial array mpi

factorial: factorial.o
	$(CC) $(CFLAGS) -o $@ $^

array: array.o
	$(CC) $(CFLAGS) -o $@ $^

mpi: mpi.c
	$(MPICC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf factorial
	rm -rf array
	rm -rf mpi
	rm -rf *.o
