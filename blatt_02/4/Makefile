CC=gcc
MPICC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -O0 -Wconversion

.PHONY: all clean

all: 4-mpi 4-omp

4-mpi: 4-mpi.c
	$(MPICC) $(CFLAGS) -o $@ $^

4-omp: 4-omp.c
	$(CC) $(CFLAGS) -fopenmp -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf 4-mpi 4-omp
	rm -rf *.o
