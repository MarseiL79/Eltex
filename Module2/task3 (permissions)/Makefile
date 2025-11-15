CC=gcc
CFLAGS=-Wall -Wextra

all: permissions_app

permissions_app: main.o permissions.o
	$(CC) main.o permissions.o -o permissions_app

main.o: main.c permissions.h
	$(CC) $(CFLAGS) -c main.c

permissions.o: permissions.c permissions.h
	$(CC) $(CFLAGS) -c permissions.c

clean:
	rm -rf *.o permissions_app

run: permissions_app
	./permissions_app