CC=gcc
CFLAGS=-Wall -Wextra

all: myshell sum concat max 

myshell: myshell.c
	$(CC) $(CFLAGS) myshell.c -o myshell

sum: sum.c
	$(CC) $(CFLAGS) sum.c -o sum

concat: concat.c
	$(CC) $(CFLAGS) concat.c -o concat

max: max.c
	$(CC) $(CFLAGS) max.c -o max

clean:
	rm -f myshell sum concat max

run: myshell
	./myshell