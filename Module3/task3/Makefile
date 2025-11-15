CC=gcc
CFLAGS=-Wall -Wextra

all: phonebook

phonebook: main.o phonebook.o
	$(CC) main.o phonebook.o -o phonebook

main.o: main.c phonebook.h
	$(CC) $(CFLAGS) -c main.c

phonebook.o: phonebook.c phonebook.h
	$(CC) $(CFLAGS) -c phonebook.c

clean:
	rm -rf *.o phonebook phonebook.dat

run: phonebook
	./phonebook