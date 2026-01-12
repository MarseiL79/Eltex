CC = gcc
CFLAGS = -Wall -Wextra -lrt
TARGETS = chat1 chat2
OBJECTS = chat1.o chat2.o

all: $(TARGETS)

chat1: chat1.o
	$(CC) $(CFLAGS) -o $@ $^

chat2: chat2.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGETS) $(OBJECTS)

.PHONY: all clean