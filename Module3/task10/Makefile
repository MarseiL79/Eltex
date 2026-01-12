CC = gcc
CFLAGS = -Wall -Wextra
TARGET = shm_prog
OBJECT = shared_memory.o

all: $(TARGET)

shm_prog: shared_memory.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECT)

run: shm_prog
	./shm_prog

.PHONY: all clean run