CC=gcc
CFLAGS=-Wall -Wextra

all: ip_simulator

ip_simulator: main.o ipv4.o
	$(CC) main.o ipv4.o -o ip_simulator

main.o: main.c ipv4.h
	$(CC) $(CFLAGS) -c main.c

ipv4.o: ipv4.c ipv4.h
	$(CC) $(CFLAGS) -c ipv4.c

clean:
	rm -rf *.o ip_simulator

run: ip_simulator
	./ip_simulator