CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -Wconversion -fopenmp -O3 -ggdb

.PHONY: all clean

all: 1.1 1.2 1.3 1.4

1.1: 1.1.o time_difference.o
	$(CC) $(CFLAGS) -o $@.out $^

1.2: 1.2.o time_difference.o
	$(CC) $(CFLAGS) -o $@.out $^

1.3: 1.3.o time_difference.o
	$(CC) $(CFLAGS) -o $@.out $^

1.4: 1.4.o time_difference.o
	$(CC) $(CFLAGS) -o $@.out $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.out
	rm -rf *.o
