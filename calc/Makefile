# Makefile for a C program

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
DEPFLAGS = -MMD -MP
INCLUDES = -I../../Inc

# Source files, object files, and dependency files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# Executable name
TARGET = calc

# Build rule
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean rule
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
