# Makefile for new_curl project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -Iinclude

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = my_curl

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
