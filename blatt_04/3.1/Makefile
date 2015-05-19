CC=vtcc -vt:inst compinst -vt:cc gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -Wconversion -fopenmp -O3 -ggdb

.PHONY: all clean

all: 1.2

1.2: 1.2.o time_difference.o
	$(CC) $(CFLAGS) -o $@.out $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.out
	rm -rf *.o
