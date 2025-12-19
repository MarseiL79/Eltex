CC = gcc
CFLAGS = -Wall -Wextra -fPIC
TARGET = calculator
LIBS_DIR = libs
LDFLAGS = -ldl -lm

all: $(TARGET) create_libs

$(TARGET): main.o calculator.o
	$(CC) main.o calculator.o -o $(TARGET) $(LDFLAGS)

main.o: main.c calculator.h
	$(CC) $(CFLAGS) -c main.c

calculator.o: calculator.c calculator.h
	$(CC) $(CFLAGS) -c calculator.c

create_libs:
	mkdir -p $(LIBS_DIR)
	$(CC) $(CFLAGS) -shared -o $(LIBS_DIR)/add.so add.c
	$(CC) $(CFLAGS) -shared -o $(LIBS_DIR)/subtract.so subtract.c
	$(CC) $(CFLAGS) -shared -o $(LIBS_DIR)/multiply.so multiply.c
	$(CC) $(CFLAGS) -shared -o $(LIBS_DIR)/divide.so divide.c
	$(CC) $(CFLAGS) -shared -o $(LIBS_DIR)/power.so power.c

clean:
	rm -rf *.o $(TARGET) $(LIBS_DIR)/*.so $(LIBS_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run create_libs