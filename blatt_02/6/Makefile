CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -Wconversion -pedantic -O3
LFLAGS=-lrt

.PHONY: all clean

all: matrix

matrix: main.o matrix_mpi.o helpers.o
	$(CC) -o $@ $(CFLAGS) $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf matrix
	rm -rf *.o
