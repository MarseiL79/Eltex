CC=gcc
CFLAGS=-Wall -Wextra

all: calculator

calculator: main.o calculator.o
	$(CC) main.o calculator.o -o calculator -lm

main.o: main.c calculator.h
	$(CC) $(CFLAGS) -c main.c

calculator.o: calculator.c calculator.h
	$(CC) $(CFLAGS) -c calculator.c

clean:
	rm -rf *.o calculator

run: calculator
	./calculator