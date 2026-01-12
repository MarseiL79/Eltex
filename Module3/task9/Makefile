CC = gcc
CFLAGS = -Wall -Wextra -lrt -lpthread
TARGET = semap
OBJECT = semaphore_posix.o

all: $(TARGET)

semap: semaphore_posix.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECT) shared_data shared_data_offset

run: semap
	./semap

.PHONY: all clean run