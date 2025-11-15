CC=gcc
CFLAGS=-Wall -Wextra

all: phonebook_modified

phonebook_modified: main.o phonebook_modified.o
	$(CC) main.o phonebook_modified.o -o phonebook_modified

main.o: main.c phonebook_modified.h
	$(CC) $(CFLAGS) -c main.c

phonebook_modified.o: phonebook_modified.c phonebook_modified.h
	$(CC) $(CFLAGS) -c phonebook_modified.c

clean:
	rm -rf *.o phonebook_modified

run: phonebook_modified
	./phonebook_modified