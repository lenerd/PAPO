CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic -fopenmp

.PHONY: all

all: helloomp


helloomp: helloomp.o
	$(CC) $(CFLAGS) -o helloomp $^


%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -rf *.o helloomp
