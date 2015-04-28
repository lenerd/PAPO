CC=clang
CFLAGS= -std=c11 -Wall -Wextra -pedantic

.PHONY: all

all: helloworld


helloworld: helloworld.o
	$(CC) $(CFLAGS) -o helloworld $^


%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -rf *.o helloworld
