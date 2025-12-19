CC = gcc
CFLAGS = -std=c99 -Wall -Wextra
TARGET = phonebook
LIBRARY = libphonebook.a

LIB_SRCS = phonebook.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

MAIN_SRCS = main.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(MAIN_OBJS) $(LIBRARY)
	$(CC) $(CFLAGS) -o $(TARGET) $(MAIN_OBJS) -L. -lphonebook

$(LIBRARY): $(LIB_OBJS)
	ar rcs $(LIBRARY) $(LIB_OBJS)

phonebook.o: phonebook.c phonebook.h
	$(CC) $(CFLAGS) -c phonebook.c -o phonebook.o

main.o: main.c phonebook.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(TARGET) $(MAIN_OBJS) $(LIB_OBJS) $(LIBRARY)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run