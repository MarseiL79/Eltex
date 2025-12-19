CC=gcc
CFLAGS=-Wall -Wextra

all: phonebook

phonebook: phonebook.o main.o
	$(CC) phonebook.o main.o -o phonebook

phonebook.o: phonebook.c phonebook.h
	$(CC) $(CFLAGS) -c phonebook.c

main.o: main.c phonebook.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -rf *.o phonebook

run: phonebook
	./phonebook