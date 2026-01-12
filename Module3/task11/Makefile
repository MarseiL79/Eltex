CC = gcc
CFLAGS = -Wall -Wextra -lrt -lpthread
TARGET = posix_shm
OBJECT = posix_shm.o

all: $(TARGET)

posix_shm: posix_shm.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECT)

run: posix_shm
	./posix_shm

.PHONY: all clean run