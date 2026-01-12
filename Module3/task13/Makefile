CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = tcp_client tcp_server
OBJECTS = tcp_client.o tcp_server.o

all: $(TARGETS)

tcp_client: tcp_client.o
	$(CC) $(CFLAGS) -o $@ $^

tcp_server: tcp_server.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS) sample.txt

.PHONY: all clean