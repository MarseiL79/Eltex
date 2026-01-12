CC = gcc
CFLAGS = -Wall -Wextra
TARGET = prodcons
OBJECT = producer_consumer.o

all: $(TARGET)

prodcons: producer_consumer.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECT) *_position

.PHONY: all clean