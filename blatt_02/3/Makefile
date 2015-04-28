CC=mpicc
CFLAGS=-std=c11 -Wall -Wextra -pedantic -ggdb -O0 -Wconversion

.PHONY: all clean

all: aufgabe-3.1 aufgabe-3.2 aufgabe-3.3 aufgabe-3.4

aufgabe-3.1: aufgabe-3.1.o
	$(CC) $(CFLAGS) -o $@ $^

aufgabe-3.2: aufgabe-3.2.o
	$(CC) $(CFLAGS) -o $@ $^

aufgabe-3.3: aufgabe-3.3.o
	$(CC) $(CFLAGS) -o $@ $^

aufgabe-3.4: aufgabe-3.4.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf aufgabe-3.{1,2,3,4}
	rm -rf *.o
