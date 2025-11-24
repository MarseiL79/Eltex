CC=gcc
CFLAGS=-Wall -Wextra

all: myshell

myshell: myshell.o
	$(CC) myshell.o -o myshell

myshell.o: myshell.c
	$(CC) $(CFLAGS) -c myshell.c

clean:
	rm -rf *.o myshell

run: myshell
	./myshell