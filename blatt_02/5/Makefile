CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2 -Wconversion
LFLAGS=-lrt

.PHONY: all clean

all: aufgabe-5.1 matrix

aufgabe-5.1: aufgabe-5.1.o
	$(CC) $(CFLAGS) -o $@ $^

matrix: main.o matrix_mpi.o helpers.o
	$(CC) -o $@ $(CFLAGS) $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf aufgabe-5.1
	rm -rf matrix
	rm -rf *.o
