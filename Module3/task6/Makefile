CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = server client
OBJECTS = server.o client.o

all: $(TARGETS)

server: server.o
	$(CC) $(CFLAGS) -o $@ $^

client: client.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: all clean