CC = gcc
CFLAGS = -Wall -Wextra
TARGETS = tcp_multiplex_client tcp_multiplex_server
OBJECTS = tcp_multiplex_client.o tcp_multiplex_server.o

all: $(TARGETS)

tcp_multiplex_client: tcp_multiplex_client.o
	$(CC) $(CFLAGS) -o $@ $^

tcp_multiplex_server: tcp_multiplex_server.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS) datafile.txt

.PHONY: all clean