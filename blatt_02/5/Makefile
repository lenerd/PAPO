CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -O0 -Wconversion

.PHONY: all clean

# all: aufgabe-5.1 matrix_serial matrix_mpi
all: test

aufgabe-5.1: aufgabe-5.1.o
	$(CC) $(CFLAGS) -o $@ $^

matrix_serial: matrix_serial.o
	echo "foo"

matrix_mpi: matrix_mpi.o helpers.o
	echo "bar"

test: matrix_mpi.o helpers.o test.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf aufgabe-5.1
	rm -rf matrix_serial
	rm -rf test
	rm -rf *.o
