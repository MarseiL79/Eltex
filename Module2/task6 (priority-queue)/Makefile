CC=gcc
CFLAGS=-Wall -Wextra

all: priority_queue

priority_queue: main.o priority_queue.o
	$(CC) main.o priority_queue.o -o priority_queue

main.o: main.c priority_queue.h
	$(CC) $(CFLAGS) -c main.c

priority_queue.o: priority_queue.c priority_queue.h
	$(CC) $(CFLAGS) -c priority_queue.c

clean:
	rm -rf *.o priority_queue

run: priority_queue
	./priority_queue