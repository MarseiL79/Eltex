CC=gcc
CFLAGS=-Wall -Wextra

all: task1

task1: task1.o
	$(CC) task1.o -o task1

task1.o: task1.c
	$(CC) $(CFLAGS) -c task1.c

clean:
	rm -rf *.o task1

test1: task1
	./task1 1 2 3 a b c

test2: task1
	./task1 25abc 2.333 5.01 0