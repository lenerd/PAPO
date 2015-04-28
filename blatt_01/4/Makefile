CC=mpicc
CFLAGS= -std=c11 -Wall -Wextra -pedantic

.PHONY: all

all: hellompi


hellompi: hellompi.o
	$(CC) $(CFLAGS) -o hellompi $^


%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -rf *.o hellompi
