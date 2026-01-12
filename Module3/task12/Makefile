CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGETS = udp_client udp_server
OBJECTS = udp_client.o udp_server.o

all: $(TARGETS)

udp_client: udp_client.o
	$(CC) $(CFLAGS) -o $@ $^

udp_server: udp_server.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: all clean