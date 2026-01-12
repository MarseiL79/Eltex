CC=gcc
CFLAGS=-Wall -Wextra

all: signal_program

signal_program: signal_program.o
	$(CC) signal_program.o -o signal_program

signal_program.o: signal_program.c
	$(CC) $(CFLAGS) -c signal_program.c

clean:
	rm -rf *.o signal_program output.txt

run: signal_program
	./signal_program &

.PHONY: all clean run