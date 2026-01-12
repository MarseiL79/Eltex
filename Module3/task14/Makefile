CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGETS = udp_raw_client raw_sniffer
OBJECTS = udp_raw_client.o raw_sniffer.o

all: $(TARGETS)

udp_raw_client: udp_raw_client.o
	$(CC) $(CFLAGS) -o $@ $^

raw_sniffer: raw_sniffer.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: all clean