CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -O0 -Wconversion

.PHONY: all clean

all: 4-mpi

4-mpi: 4-mpi.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf 4-mpi
	rm -rf *.o
