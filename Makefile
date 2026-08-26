# Target executable name
TARGET = main

# Automatically find all .c files in the current directory
SRCS = $(wildcard *.c)

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -I.

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Run the binary under macOS's native leak checker
leaks: $(TARGET)
	leaks --atExit -- ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean leaks