CC = gcc
CFLAGS = -std=c99 -Wall -fPIC
TARGET = phonebook
LIBRARY = libphonebook.so
LDFLAGS = -L. -lphonebook -Wl,-rpath,.

all: $(TARGET)

$(TARGET): main.c $(LIBRARY)
	$(CC) $(CFLAGS) -o $(TARGET) main.c $(LDFLAGS)

$(LIBRARY): phonebook.c
	$(CC) $(CFLAGS) -shared -o $(LIBRARY) phonebook.c

clean:
	rm -f $(TARGET) $(LIBRARY)

run: $(TARGET)
	./$(TARGET)

deps:
	ldd $(TARGET)

.PHONY: all clean run deps